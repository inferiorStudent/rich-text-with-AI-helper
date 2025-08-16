#include "kclipboardtoolbar.h"
#include <QVBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QClipboard>
#include <QApplication>
#include <QIcon>
#include <QMimeData>
#include <memory>

KClipboardToolbar::KClipboardToolbar(QWidget* parent)
	: QWidget(parent)
{
	m_pasteButton = new QToolButton(this);
	m_pasteButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	m_pasteButton->setFixedSize(64, 72);
	m_pasteButton->setStyleSheet(R"(
		QToolButton {
			border-style: flat;
			background: transparent;
		}
		QToolButton:hover {
			border-style: flat;
			background: #D0D0D0;
		})");

	m_expandButton = new QToolButton(this);
	m_expandButton->setIcon(QIcon(":/image/icons/expand-bottom-right.svg"));
	m_expandButton->setAutoRaise(true);
	m_expandButton->setStyleSheet(R"(QToolButton { border: none; width: 15px; height: 15px; }
		QToolButton:hover { border-style: flat; background: #D0D0D0; })");
	connect(m_expandButton, &QToolButton::clicked, this, &KClipboardToolbar::expandRequested);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(m_pasteButton, 0, Qt::AlignHCenter);
	// 用智能指针创建无法显示!
	QLabel* label = new QLabel(tr("clipboard"), this);
	label->setStyleSheet("QLabel { border: none; }");

	QHBoxLayout* bottomLayout = new QHBoxLayout(this);
	bottomLayout->addWidget(label);
	bottomLayout->addWidget(m_expandButton);
	layout->addLayout(bottomLayout);

	// 监视系统剪贴板的变化
	QClipboard* clipboard = QApplication::clipboard();
	connect(clipboard, &QClipboard::changed, this, &KClipboardToolbar::onClipboardChanged);

	// 初始化按钮状态
	onClipboardChanged();
}

void KClipboardToolbar::onClipboardChanged()
{
	if (QApplication::clipboard()->mimeData()->hasText())
	{
		m_pasteButton->setEnabled(true);
		m_pasteButton->setIcon(QIcon(":/image/icons/paste.svg"));
	}
	else
	{
		m_pasteButton->setEnabled(false);
		m_pasteButton->setIcon(QIcon(":/image/icons/paste_disabled.svg"));
	}
	m_pasteButton->setIconSize(QSize(50, 50));
}

QToolButton* KClipboardToolbar::pasteButton() const
{
	return m_pasteButton;
}