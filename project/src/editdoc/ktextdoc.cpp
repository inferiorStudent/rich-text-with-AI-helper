#include "kpagecontainer.h"
#include "kheaderedit.h"
#include "ktextdoc.h"
#include "textstyle.h"
#include <QTextEdit>
#include <QTextCursor>
#include <QTextList>
#include <QTextImageFormat>
#include <QApplication>
#include "kaieditor.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

KTextDoc::KTextDoc(QWidget* parent)
	: QScrollArea(parent)
{
	setWidgetResizable(true);		// 允许滚动区域内的控件随区域大小变化
	setAlignment(Qt::AlignCenter);	// 滚动视图内容居中
	setStyleSheet("QScrollArea { background-color: #E0E0E0; border: none; }");

	m_currentPageSize = QPageSize(QPageSize::A4);
	m_currentOrientation = QPageLayout::Portrait;

	m_documentContainer = new QWidget(this);
	m_pageLayout = new QVBoxLayout(m_documentContainer);
	m_pageLayout->setSpacing(20); // 页面间距
	m_pageLayout->setAlignment(Qt::AlignHCenter);

	setWidget(m_documentContainer);
	// 创建第一页
	insertNewPage();
}

// 页面管理

void KTextDoc::insertNewPage()
{
	KPageContainer* newPage = new KPageContainer(m_documentContainer);
	// 应用当前文档的统一页面设置
	newPage->updateMetrics(m_currentPageSize, m_currentOrientation);
	// --- 关键：安装事件过滤器来追踪焦点 ---
	newPage->page()->installEventFilter(this);
	newPage->header()->installEventFilter(this);
	newPage->footer()->installEventFilter(this);

	connect(newPage->page(), &KAIEditor::cursorPositionChanged, this, &KTextDoc::onCursorPositionChanged);
	connect(newPage->page()->document(), &QTextDocument::modificationChanged, this, &KTextDoc::onPageModificationChanged);
	m_pages.append(newPage);
	m_pageLayout->addWidget(newPage);
	updatePageNumbers();

	if (!m_lastActiveTextEdit)
		m_lastActiveTextEdit = newPage->page();
}

void KTextDoc::onPageModificationChanged(bool m)
{
	// 任何一个页面被修改，整个文档就算被修改
	emit modificationChanged(isModified());
}

bool KTextDoc::saveToFile(const QString& filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		return false;
	}

	QTextStream out(&file);
	for (int i = 0; i < m_pages.count(); ++i)
	{
		out << m_pages[i]->page()->document()->toHtml();
		if (i < m_pages.count() - 1)
		{
			// 使用一个简单的分隔符来区分页面
			out << "\n---PAGE_BREAK---\n";
		}
	}

	// 保存成功后，将所有页面的修改状态重置为false
	for(auto page : m_pages)
	{
		page->page()->document()->setModified(false);
	}
	emit modificationChanged(false);
	setCurrentFilePath(filePath);
	return true;
}

bool KTextDoc::loadFromFile(const QString& filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return false;
	}

	// 清空当前所有页面
	qDeleteAll(m_pages);
	m_pages.clear();

	QTextStream in(&file);
	QString content = in.readAll();
	QStringList pageContents = content.split("\n---PAGE_BREAK---\n");

	for (const QString& pageHtml : pageContents)
	{
		insertNewPage(); // 创建一个新页面
		m_pages.last()->page()->setHtml(pageHtml); // 加载内容
		m_pages.last()->page()->document()->setModified(false);
	}
	
	emit modificationChanged(false);
	setCurrentFilePath(filePath);
	return true;
}

QString KTextDoc::currentFilePath() const
{
	return m_filePath;
}

void KTextDoc::setCurrentFilePath(const QString& filePath)
{
	m_filePath = filePath;
}

bool KTextDoc::isModified() const
{
	// 任何一个页面被修改过，就认为整个文档被修改了
	for (const auto& page : m_pages)
	{
		if (page->page()->document()->isModified())
		{
			return true;
		}
	}
	return false;
}

void KTextDoc::updatePageNumbers()
{
	for (int i = 0; i < m_pages.count(); ++i)
	{
		m_pages[i]->setPageNumber(i + 1);
	}
}

bool KTextDoc::eventFilter(QObject* watched, QEvent* event)
{
	if (event->type() == QEvent::FocusIn)
	{
		for (KPageContainer* page : m_pages)
		{
			if (watched == page->page() || watched == page->header() || watched == page->footer())
			{
				m_lastActiveTextEdit = qobject_cast<QTextEdit*>(watched);
				qDebug() << "Active editor changed!";
				break;
			}
		}
	}
	return QScrollArea::eventFilter(watched, event);
}

QTextEdit* KTextDoc::activeTextEdit() const
{
	// bug: 定位不到光标所在页面
	QWidget* focused = QApplication::focusWidget();
	if (m_lastActiveTextEdit)
		return m_lastActiveTextEdit;
	return m_pages.isEmpty() ? nullptr : m_pages.first()->page();
}

void KTextDoc::mergeFormatOnSelection(const QTextCharFormat& format)
{
	QTextEdit* editor = activeTextEdit();
	if (!editor)
		return;
	QTextCursor cursor = editor->textCursor();
	if (!cursor.hasSelection())
	{
		cursor.select(QTextCursor::WordUnderCursor);
	}
	cursor.mergeCharFormat(format);
	editor->mergeCurrentCharFormat(format);
}

void KTextDoc::applyPageSizeToAllPages(const QPageSize& size)
{
	m_currentPageSize = size;
	for (auto page : m_pages)
		page->updateMetrics(m_currentPageSize, m_currentOrientation);
}

void KTextDoc::applyPageOrientationToAllPages(QPageLayout::Orientation orientation)
{
	m_currentOrientation = orientation;
	for (auto page : m_pages)
		page->updateMetrics(m_currentPageSize, m_currentOrientation);
}

void KTextDoc::applyPageMarginToAllPages(const QMargins& marginSize)
{
	for (auto page : m_pages)
		page->updatePageMargin(marginSize);
}

void KTextDoc::applyPageNumber(bool flag)
{
	for (auto page : m_pages)
		page->insertPageNumber(flag);
}

// 字体

void KTextDoc::setTextBold(bool bold)
{
	QTextCharFormat fmt;
	fmt.setFontWeight(bold ? QFont::Bold : QFont::Normal);
	mergeFormatOnSelection(fmt);
}

void KTextDoc::setTextItalic(bool italic)
{
	QTextCharFormat fmt;
	fmt.setFontItalic(italic);
	mergeFormatOnSelection(fmt);
}

void KTextDoc::setTextUnderline(bool underline)
{
	QTextCharFormat fmt;
	fmt.setFontUnderline(underline);
	mergeFormatOnSelection(fmt);
}

void KTextDoc::setTextFontFamily(const QString& family)
{
	QTextCharFormat fmt;
	fmt.setFontFamily(family);
	mergeFormatOnSelection(fmt);
}

void KTextDoc::setTextFontSize(int size)
{
	QTextCharFormat fmt;
	fmt.setFontPointSize(size);
	mergeFormatOnSelection(fmt);
}

void KTextDoc::setParagraphAlignment(Qt::Alignment alignment)
{
	QTextEdit* editor = activeTextEdit();
	if (!editor)
		return;
	QTextCursor cursor = editor->textCursor();
	QTextBlockFormat blockFmt = cursor.blockFormat();
	blockFmt.setAlignment(alignment);
	cursor.mergeBlockFormat(blockFmt);
}

void KTextDoc::setParagraphListStyle(QTextListFormat::Style style)
{
	QTextEdit* editor = activeTextEdit();
	if (!editor)
		return;
	QTextCursor cursor = editor->textCursor();
	if (cursor.currentList())
	{
		// 如果当前是列表，检查样式是否相同，相同则取消列表
		if (cursor.currentList()->format().style() == style)
		{
			cursor.createList(QTextListFormat::ListStyleUndefined);
			return;
		}
	}
	cursor.createList(style);
}

void KTextDoc::setParagraphLineHeight(qreal factor)
{
	QTextEdit* editor = activeTextEdit();
	if (!editor)
		return;
	QTextCursor cursor = editor->textCursor();
	if (!cursor.hasSelection())
		cursor.select(QTextCursor::BlockUnderCursor);
	
	QTextBlockFormat blockFmt = cursor.blockFormat();
	blockFmt.setLineHeight(factor * 100, QTextBlockFormat::ProportionalHeight);
	cursor.mergeBlockFormat(blockFmt);
}

void KTextDoc::onCursorPositionChanged()
{
	// 当光标移动时，获取当前格式并向外发射信号
	// emit selectionFormatChanged(m_pageContainer->page()->currentCharFormat());
	QTextEdit* editor = activeTextEdit();
	if (!editor)
		return;
	QTextCursor cursor = editor->textCursor();
	QTextList * currentList = cursor.currentList();
	QTextListFormat listFmt; // 默认为无效列表格式
	if (currentList)
	{
		// 如果光标在列表中，则获取该列表的格式
		listFmt = currentList->format();
	}
	emit selectionFormatChanged(cursor.charFormat(), cursor.blockFormat(), listFmt);
}

void KTextDoc::setTextColor(const QColor& color)
{
	QTextCharFormat fmt;
	fmt.setForeground(color);
	mergeFormatOnSelection(fmt);
}

void KTextDoc::paste()
{
	activeTextEdit()->paste();
}

void KTextDoc::applyStyle(const TextStyle & style)
{
	QTextEdit* editor = activeTextEdit();
	if (!editor)
		return;
	QTextCursor cursor = editor->textCursor();
	cursor.setBlockFormat(style.blockFormat);
	cursor.setCharFormat(style.charFormat);
	editor->setTextCursor(cursor);
}

void KTextDoc::insertTable(int rows, int cols)
{
	QTextEdit* editor = activeTextEdit();
	if (!editor)
		return;
	QTextCursor cursor = editor->textCursor();
	QTextTableFormat tableFormat;
	tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
	tableFormat.setCellSpacing(0);
	tableFormat.setBorderBrush(QColor(Qt::black));
	tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));
	cursor.insertTable(rows, cols, tableFormat);
}

void KTextDoc::insertImage(const QString & path)
{
	QTextEdit* editor = activeTextEdit();
	if (!editor)
		return;
	QTextCursor cursor = editor->textCursor();
	QUrl imageUrl = QUrl::fromLocalFile(path);
	QTextImageFormat imageFormat;
	imageFormat.setName(imageUrl.toString());
	cursor.insertImage(imageFormat);
}

void KTextDoc::insertSymbol(const QString & symbol)
{
	QTextEdit* editor = activeTextEdit();
	if (!editor)
		return;
	QTextCursor cursor = editor->textCursor();
	cursor.insertText(symbol);
}