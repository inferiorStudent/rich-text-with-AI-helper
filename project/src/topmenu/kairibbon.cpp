#include "kairibbon.h"
#include "kaieditor.h"
#include "aiconfigdialog.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QMenu>
#include <QIcon>

KAIRibbon::KAIRibbon(KAIEditor* editor, QWidget* parent)
	: QWidget(parent)
	, m_editor(editor)
	, m_isOpen(new QToolButton(this))
{
	QHBoxLayout* layout = new QHBoxLayout(this);

	QString buttonStyle = R"(
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

	QString menuStyle = R"(
		QMenu::item { background-color: transparent; color: black; }
		QMenu::item:selected { background-color: #66B2FF; color: black; })";

	QToolButton* menuButton = new QToolButton(this);
	menuButton->setText(tr("AI helper"));
	menuButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	menuButton->setStyleSheet(buttonStyle);
	menuButton->setIcon(QIcon(":/image/icons/ai-link.svg"));
	menuButton->setPopupMode(QToolButton::InstantPopup);

	QMenu* menu = new QMenu(this);
	menu->setStyleSheet(menuStyle);
	QAction* configAction = menu->addAction(tr("link config"));
	QAction* enableAction = menu->addAction(tr("AI open"));
	QAction* disableAction = menu->addAction(tr("AI close"));

	connect(configAction, &QAction::triggered, this, &KAIRibbon::openConfigDialog);
	connect(enableAction, &QAction::triggered, this, [this](){
		m_isOpen->setIcon(QIcon(":/image/icons/AI-open.svg"));
		m_isOpen->setText(tr("AI open"));
		m_editor->setServiceEnabled(true);
		});
	connect(disableAction, &QAction::triggered, this, [this](){
		m_isOpen->setIcon(QIcon(":/image/icons/AI-close.svg"));
		m_isOpen->setText(tr("AI close"));
		m_editor->setServiceEnabled(false);
		});

	menuButton->setMenu(menu);
	layout->addWidget(menuButton);

	QFrame* sep = getSeparateLine();
	layout->addWidget(sep);

	// 状态显示区
	m_isOpen->setText(tr("AI close"));
	m_isOpen->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	m_isOpen->setStyleSheet(R"(
		QToolButton {
			border: none;
			background-color: transparent;
			padding: 5px;
			height: 90px;
			toolbutton-style: vertical;		/* 设置图标在文字上方 */
			spacing: 10px;					/* 设置图标和文字之间的间距 */
			font-size: 18px;
			font-family: "Microsoft YaHei";
		}
		QToolButton:hover { background-color: #D0D0D0; }
		QToolButton:pressed { background-color: #d0d0d0; border-style: inset; }
		QToolButton { icon-size: 48px 48px; })");
	m_isOpen->setIcon(QIcon(":/image/icons/AI-close.svg"));
	layout->addWidget(m_isOpen);

	m_statusIconLabel = new QLabel(this);
	m_statusTextLabel = new QLabel(tr("not yet config"), this);
	m_statusTextLabel->setStyleSheet("QLabel { border: none; }");
	m_statusIconLabel->setStyleSheet("QLabel { border: none; }");
	layout->addWidget(m_statusIconLabel);
	layout->addWidget(m_statusTextLabel);

	layout->addStretch();

	// 初始化状态
	updateConnectionStatus(false);
}

void KAIRibbon::openConfigDialog()
{
	if (!m_configDialog)
	{
		m_configDialog = new AIConfigDialog(m_currentConfig, this);
		connect(m_configDialog, &AIConfigDialog::configApplied, this, [this](const AIConfig& newConfig){
			m_currentConfig = newConfig;
			m_statusTextLabel->setText(m_currentConfig.configName + tr(" (disconnect)"));
			emit configChanged(m_currentConfig);
		});
	}
	m_configDialog->exec();
}

void KAIRibbon::updateConnectionStatus(bool success)
{
	if (success)
	{
		m_statusIconLabel->setPixmap(QPixmap(":/image/icons/connected.svg").scaled(24, 24));
		m_statusTextLabel->setText(m_currentConfig.configName + tr(" (connected)"));
	}
	else
	{
		m_statusIconLabel->setPixmap(QPixmap(":/image/icons/unconnected.svg").scaled(24, 24));
		m_statusTextLabel->setText(m_currentConfig.configName + tr(" (disconnect)"));
	}
}

QFrame* KAIRibbon::getSeparateLine()
{
	QFrame* separator = new QFrame(this);
	separator->setFrameShape(QFrame::VLine);
	separator->setFrameShadow(QFrame::Plain);
	separator->setStyleSheet(R"(QFrame { border: none; border-right: 1px solid #C0C0C0; })");
	return separator;
}