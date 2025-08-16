#include "clipboardsidebar.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QPushButton>
#include <QLabel>
#include <QIcon>
#include <QFrame>

ClipboardSidebar::ClipboardSidebar(QWidget* parent)
	: QWidget(parent)
{
	setFixedWidth(300); // 设置一个固定的宽度
	// 设置边框和背景色
	setStyleSheet("QWidget { background-color: #F0F0F0; border-right: 1px solid #D0D0D0; }");

	// --- 顶部标题栏 ---
	QLabel* titleLabel = new QLabel(tr("clipboard"));
	m_closeButton = new QToolButton(this);
	m_closeButton->setIcon(QIcon(":/image/icons/close.svg"));
	m_closeButton->setStyleSheet(R"(
		QToolButton { border: none; }
		QToolButton:hover { border-style: flat; background: #D0D0D0; })");
	// m_closeButton->setAutoRaise(true); // 透明背景，悬停时显示边框

	connect(m_closeButton, &QToolButton::clicked, this, &ClipboardSidebar::closeRequested);

	QHBoxLayout* topLayout = new QHBoxLayout();
	topLayout->addWidget(titleLabel);
	topLayout->addStretch();
	topLayout->addWidget(m_closeButton);

	// --- 功能按钮 ---
	QPushButton * pasteAllButton = new QPushButton(tr("paste all"));
	QPushButton * clearAllButton = new QPushButton(tr("clear all"));
	connect(pasteAllButton, &QPushButton::clicked, this, &ClipboardSidebar::pasteAllRequested);
	connect(clearAllButton, &QPushButton::clicked, this, &ClipboardSidebar::clearAllRequested);
	
	QHBoxLayout * buttonLayout = new QHBoxLayout();
	buttonLayout->addWidget(pasteAllButton);
	buttonLayout->addWidget(clearAllButton);
	buttonLayout->addStretch();

	// --- 内容区 (暂时用一个标签占位) ---
	QLabel* contentPlaceholder = new QLabel(tr("...."));
	contentPlaceholder->setWordWrap(true);
	contentPlaceholder->setAlignment(Qt::AlignTop);

	// --- 整体布局 ---
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(5, 5, 5, 5);
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(buttonLayout);
	mainLayout->addWidget(new QFrame()); // 用一个空的Frame填充剩余空间
	mainLayout->addWidget(contentPlaceholder, 1); // 1表示占据所有可伸展空间
}