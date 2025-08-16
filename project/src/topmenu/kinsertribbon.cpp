#include "kinsertribbon.h"
#include "ktextdoc.h"
#include "ktablepickerpopup.h"
#include <QHBoxLayout>
#include <QMenu>
#include <QWidgetAction>
#include <QFileDialog>

KInsertRibbon::KInsertRibbon(KTextDoc* textDoc, QWidget* parent)
	: QWidget(parent)
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setAlignment(Qt::AlignCenter);

	m_buttonStyle = R"(
		QToolButton {
			border: none;
			background-color: transparent;
			padding: 5px;
			height: 90px;
			toolbutton-style: vertical;		/* 设置图标在文字上方 */
			spacing: 4px;					/* 设置图标和文字之间的间距 */
			font-size: 18px;
			font-family: "Microsoft YaHei";
		}
		QToolButton:hover { background-color: #D0D0D0; }
		QToolButton:pressed { background-color: #d0d0d0; border-style: inset; }
		QToolButton::menu-indicator {		/* 调整下拉箭头的位置和大小 */
			width: 16px;
			height: 16px;
			subcontrol-position: bottom;	/* 放置在底部 */
			subcontrol-origin: margin;
			image: url(:/image/icons/arrow_down.svg);  /* 使用自定义的箭头图片 */
		}
		QToolButton { icon-size: 48px 48px; })";

	m_menuStyle = R"(
		QMenu::item { background-color: transparent; color: black; }
		QMenu::item:selected { background-color: #66B2FF; color: black; })";

	// --- 表格模块 ---
	QToolButton* tableButton =
		createButton(QIcon(":/image/icons/table.svg"), tr("table"));
	QMenu* tableMenu = new QMenu(this);
	tableMenu->setStyleSheet(m_menuStyle);
	KTablePickerPopup* tablePicker = new KTablePickerPopup(8, 10, this);
	connect(tablePicker, &KTablePickerPopup::tableSizeSelected, textDoc, &KTextDoc::insertTable);
	QWidgetAction* tableAction = new QWidgetAction(tableMenu);
	tableAction->setDefaultWidget(tablePicker);

	QAction* tableConf = tableMenu->addAction(tr("insert table"));
	tableMenu->addAction(tableAction);
	tableMenu->addAction(tableConf);
	tableButton->setMenu(tableMenu);
	layout->addWidget(tableButton);

	// --- 图片模块 ---
	QToolButton* imageButton =
		createButton(QIcon(":/image/icons/picture.svg"), tr("picture"));
	QMenu* imageMenu = new QMenu(this);
	imageMenu->setStyleSheet(m_menuStyle);
	QAction* fromDeviceAction = imageMenu->addAction(QIcon(":/image/icons/from_device.svg"), tr("from this device..."));
	connect(fromDeviceAction, &QAction::triggered, this, [=]() {
		QString filePath = QFileDialog::getOpenFileName(this, tr("select pictures"), "", "Images (*.png *.xpm *.jpg *.jpeg)");
		if (!filePath.isEmpty())
			textDoc->insertImage(filePath);
		});
	imageButton->setMenu(imageMenu);
	layout->addWidget(imageButton);

	// --- 符号模块 (基础版) ---
	QToolButton* symbolButton =
		createButton(QIcon(":/image/icons/omega.svg"), tr("symbol"));
	QMenu* symbolMenu = new QMenu(this);
	symbolMenu->setStyleSheet(m_menuStyle);
	QStringList commonSymbols = { tr("multiple"), tr("divide"), tr("non equal") };
	for (const QString& symbol : commonSymbols) {
		symbolMenu->addAction(symbol, [=]() {
			textDoc->insertSymbol(symbol);
			});
	}
	symbolButton->setMenu(symbolMenu);
	layout->addWidget(symbolButton);

	// layout->addStretch();
}

QToolButton* KInsertRibbon::createButton(const QIcon& icon, const QString& text)
{
	QToolButton* button = new QToolButton(this);
	button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	button->setIcon(icon);
	button->setText(text);
	button->setPopupMode(QToolButton::InstantPopup);
	button->setStyleSheet(m_buttonStyle);
	return button;
}