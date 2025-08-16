#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ktextdoc.h"
#include "kstartribbon.h"
#include "kinsertribbon.h"
#include "kpageribbon.h"
#include "kairibbon.h"
#include "kaieditor.h"
#include "clipboardsidebar.h"
#include "kstylesfloatingwindow.h"
#include "stylemanager.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QCloseEvent>
#include <QClipBoard>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setupMenuBar(); // 设置文件菜单
	statusBar()->showMessage(tr("ready"));
	m_styleManager = new StyleManager(this);

	// --- 创建主布局 ---
	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	// 1. 文件切换标签栏
	m_fileTabs = new KTab(this);
	connect(m_fileTabs, &KTab::tabClosed, this, &MainWindow::onDocumentTabClosed);
	connect(m_fileTabs, &KTab::currentChanged, this, &MainWindow::onTabChanged);
	mainLayout->addWidget(m_fileTabs);

	// 2. Ribbon 菜单栏
	m_ribbonMenu = new KTopMenu(this);
	mainLayout->addWidget(m_ribbonMenu);

	// 3. Ribbon 面板
	m_ribbonPanels = new QStackedWidget(this);
	m_ribbonPanels->setStyleSheet(R"(
		QStackedWidget {
			background-color: white;
			border: 1px solid gray;
			border-top: none;
			border-radius: 10px; })");
	mainLayout->addWidget(m_ribbonPanels);

	// --- 4. 主内容区 (侧边栏 + 文档堆栈) ---
	QWidget* mainContentArea = new QWidget(this);
	QHBoxLayout* hLayout = new QHBoxLayout(mainContentArea);
	hLayout->setSpacing(0);
	hLayout->setContentsMargins(0, 0, 0, 0);

	m_clipboardSidebar = new ClipboardSidebar(this);
	m_clipboardSidebar->hide();
	hLayout->addWidget(m_clipboardSidebar);

	m_documentStack = new QStackedWidget(this);
	m_documentStack->setStyleSheet("background-color: #E0E0E0;");
	hLayout->addWidget(m_documentStack, 1);
	mainLayout->addWidget(mainContentArea, 1);

	// --- 设置中央控件 ---
	QWidget* centralWidget = new QWidget(this);
	centralWidget->setLayout(mainLayout);
	this->setCentralWidget(centralWidget);

	this->resize(1200, 800);
	onFileNew(); // 启动时创建一个新文件
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::setupMenuBar()
{
	QMenu* fileMenu = menuBar()->addMenu(tr("file"));

	QAction* newAct = new QAction(tr("new"), this);
	newAct->setShortcut(QKeySequence::New);
	connect(newAct, &QAction::triggered, this, &MainWindow::onFileNew);
	fileMenu->addAction(newAct);

	QAction* openAct = new QAction(tr("open"), this);
	openAct->setShortcut(QKeySequence::Open);
	connect(openAct, &QAction::triggered, this, &MainWindow::onFileOpen);
	fileMenu->addAction(openAct);

	QAction* saveAct = new QAction(tr("save"), this);
	saveAct->setShortcut(QKeySequence::Save);
	connect(saveAct, &QAction::triggered, this, &MainWindow::onFileSave);
	fileMenu->addAction(saveAct);

	QAction* saveAsAct = new QAction(tr("save as..."), this);
	saveAsAct->setShortcut(QKeySequence::SaveAs);
	connect(saveAsAct, &QAction::triggered, this, &MainWindow::onFileSaveAs);
	fileMenu->addAction(saveAsAct);

	fileMenu->addSeparator();
	QAction* exitAct = new QAction(tr("exit"), this);
	exitAct->setShortcut(QKeySequence::Quit);
	connect(exitAct, &QAction::triggered, this, &QWidget::close);
	fileMenu->addAction(exitAct);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	// 必须从后往前遍历并关闭，因为onDocumentTabClosed会修改m_documents
	QList<int> tabIds = m_documents.keys();
	for (int i = tabIds.size() - 1; i >= 0; --i)
	{
		int tabId = tabIds[i];
		if (!maybeSave(tabId))
		{
			event->ignore(); // 用户点击了“取消”
			return;
		}
		// 如果用户没取消，maybeSave内部已经处理了保存。现在安全关闭标签页。
		m_fileTabs->onCloseTab(tabId);
	}
	event->accept();
}

// --- 文件槽 ---
void MainWindow::onFileNew()
{
	createNewDocument();
}

void MainWindow::onFileOpen()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("open file"), tr(""), "Text Files (*.txt);;HTML Files (*.html *.htm)");
	if (!filePath.isEmpty())
	{
		createNewDocument(filePath);
	}
}

bool MainWindow::onFileSave()
{
	KTextDoc* doc = activeTextDoc();
	if (!doc)
		return false;

	if (doc->currentFilePath().isEmpty())
	{
		return onFileSaveAs();
	}
	else
	{
		return doc->saveToFile(doc->currentFilePath());
	}
}

bool MainWindow::onFileSaveAs()
{
	KTextDoc* doc = activeTextDoc();
	if (!doc)
		return false;

	QString filePath = QFileDialog::getSaveFileName(this, tr("save as..."), "", "Text Files (*.txt);;HTML Files (*.html *.htm)");
	if (filePath.isEmpty())
	{
		return false;
	}
	return doc->saveToFile(filePath);
}

// --- 文档/标签槽 ---
void MainWindow::onTabChanged(int id)
{
	if (m_documents.contains(id))
	{
		m_documentStack->setCurrentWidget(m_documents[id].docWidget);
		KTextDoc* textDoc = activeTextDoc();
		if (textDoc)
			reConnect(textDoc);
		updateTitles();
	}
}

void MainWindow::onDocumentTabClosed(int id)
{
	if (m_documents.contains(id))
	{
		maybeSave(id);
		KTextDoc* doc = m_documents[id].docWidget;
		m_documentStack->removeWidget(doc);
		doc->deleteLater();
		m_documents.remove(id);

		if (m_documents.isEmpty())
		{
			onFileNew(); // 如果所有标签都关闭了，新建一个
		}
	}
}

void MainWindow::onDocumentModified(int tabId, bool modified)
{
	if (m_documents.contains(tabId))
	{
		m_documents[tabId].isModified = modified;
		updateTitles();
	}
}

// --- 辅助函数 ---
void MainWindow::createNewDocument(const QString& filePath)
{
	KTextDoc* textDoc = new KTextDoc(this);
	int tabId = m_fileTabs->addTab(filePath.isEmpty() ? QString(tr("untitled-%1")).arg(m_untitledCount++) : QFileInfo(filePath).fileName());
	textDoc->setProperty("tabId", tabId);

	if (!filePath.isEmpty())
	{
		textDoc->loadFromFile(filePath);
	}

	m_documentStack->addWidget(textDoc);
	m_documentStack->setCurrentWidget(textDoc);

	connect(textDoc, &KTextDoc::modificationChanged, this, [this, tabId](bool modified) {
		onDocumentModified(tabId, modified);
		});

	DocumentInfo info;
	info.docWidget = textDoc;
	info.filePath = filePath;
	info.isModified = false;
	m_documents[tabId] = info;

	reConnect(textDoc);

	updateTitles();
}


KTextDoc* MainWindow::activeTextDoc() const
{
	int id = activeTabId();
	if (m_documents.contains(id))
	{
		return m_documents[id].docWidget;
	}
	return nullptr;
}

int MainWindow::activeTabId() const
{
	return m_fileTabs->activeTabId();
}

void MainWindow::updateTitles()
{
	int id = activeTabId();
	if (!m_documents.contains(id))
	{
		setWindowTitle(tr("MyWord"));
		return;
	}

	// 更新主窗口标题
	QString docName = m_documents[id].filePath;
	if (docName.isEmpty())
	{
		docName = m_fileTabs->tabText(id).remove('*');
	}
	else
	{
		docName = QFileInfo(docName).fileName();
	}
	setWindowTitle(QString("%1%2 - MyWord").arg(docName).arg(m_documents[id].isModified ? "*" : ""));

	// 更新所有标签页标题
	for (int tabId : m_documents.keys())
	{
		const DocumentInfo& info = m_documents[tabId];
		QString baseName = info.filePath.isEmpty() ? m_fileTabs->tabText(tabId).remove('*') : QFileInfo(info.filePath).fileName();
		m_fileTabs->setTabText(tabId, baseName + (info.isModified ? "*" : ""));
	}
}

bool MainWindow::maybeSave(int tabId)
{
	if (!m_documents.contains(tabId) || !m_documents[tabId].isModified)
	{
		return true; // 不需要保存
	}

	m_fileTabs->onTabClicked(tabId); // 先切换到要关闭的标签页

	QMessageBox::StandardButton ret;
	ret = QMessageBox::warning(this, "MyWord",
		QString(tr("file '%1' has been changed. \nwanna save the change?"))
		.arg(m_fileTabs->tabText(tabId)),
		QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

	if (ret == QMessageBox::Save)
	{
		return onFileSave();
	}
	else if (ret == QMessageBox::Cancel)
	{
		return false;
	}
	return true; // Discard
}

void MainWindow::applyStyleToCurrentDoc(int styleId)
{
	KTextDoc* doc = activeTextDoc();
	if (doc)
	{
		doc->applyStyle(m_styleManager->getStyle(styleId));
	}
}

void MainWindow::onClipboardExpandRequested()
{
	// 切换侧边栏的可见性
	m_clipboardSidebar->setVisible(!m_clipboardSidebar->isVisible());
}

void MainWindow::onStylePanelRequested(const QPoint& pos)
{
	// 惰性创建：只有在第一次点击时才创建窗口实例
	if (!m_stylesWindow)
	{
		m_stylesWindow = new KStylesFloatingWindow(this);
	}
	if (m_stylesWindow->isVisible())
	{
		m_stylesWindow->hide();
	}
	else
	{
		// m_stylesWindow->move(this->geometry().center() - m_stylesWindow->rect().center());
		m_stylesWindow->move(pos);
		m_stylesWindow->show();
	}
}

void MainWindow::reConnect(KTextDoc* textDoc)
{
	// 清空并重建Ribbon面板，确保它们连接到新的textDoc
	while (m_ribbonPanels->count() > 0)
	{
		QWidget* widget = m_ribbonPanels->widget(0);
		m_ribbonPanels->removeWidget(widget);
		widget->deleteLater();
	}

	KStartRibbon* startRibbon = new KStartRibbon(textDoc, m_styleManager, this);
	connect(startRibbon, &KStartRibbon::clipboardExpandRequested, this, &MainWindow::onClipboardExpandRequested);
	connect(startRibbon, &KStartRibbon::stylePanelRequested, this, &MainWindow::onStylePanelRequested);
	connect(startRibbon, &KStartRibbon::styleApplied, this, &MainWindow::applyStyleToCurrentDoc);
	m_ribbonPanels->addWidget(startRibbon);

	// 插入
	KInsertRibbon* insertRibbon = new KInsertRibbon(textDoc, this);
	m_ribbonPanels->addWidget(insertRibbon);

	// 页面
	KPageRibbon* pageRibbon = new KPageRibbon(this);
	connect(pageRibbon, &KPageRibbon::insertPageRequested, textDoc, &KTextDoc::insertNewPage);
	connect(pageRibbon, &KPageRibbon::pageSizeChanged, textDoc, &KTextDoc::applyPageSizeToAllPages);
	connect(pageRibbon, &KPageRibbon::pageOrientationChanged, textDoc, &KTextDoc::applyPageOrientationToAllPages);
	connect(pageRibbon, &KPageRibbon::pageMarginChanged, textDoc, &KTextDoc::applyPageMarginToAllPages);
	connect(pageRibbon, &KPageRibbon::insertPageNumber, textDoc, &KTextDoc::applyPageNumber);
	m_ribbonPanels->addWidget(pageRibbon);

	KAIRibbon* aiRibbon = new KAIRibbon(textDoc->editor(), this);
	m_ribbonPanels->addWidget(aiRibbon);

	connect(m_ribbonMenu, &KTopMenu::currentChanged, m_ribbonPanels, &QStackedWidget::setCurrentIndex);
	// 连接配置更改
	connect(aiRibbon, &KAIRibbon::configChanged, textDoc->editor(), &KAIEditor::applyConfig);
	// 连接状态更新
	connect(textDoc->editor(), &KAIEditor::connectionStatusChanged, aiRibbon, &KAIRibbon::updateConnectionStatus);
	connect(m_clipboardSidebar, &ClipboardSidebar::closeRequested, this, [this]() {
		m_clipboardSidebar->hide();
		});
	connect(m_clipboardSidebar, &ClipboardSidebar::pasteAllRequested, textDoc, &KTextDoc::paste);
	connect(m_clipboardSidebar, &ClipboardSidebar::clearAllRequested, this, []() {
		QApplication::clipboard()->clear();
		});
}