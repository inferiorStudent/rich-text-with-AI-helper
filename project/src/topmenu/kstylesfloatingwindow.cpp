#include "kstylesfloatingwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QToolButton>
#include <QCheckBox>
#include <QMouseEvent>

KStylesFloatingWindow::KStylesFloatingWindow(QWidget* parent)
// 设置窗口标志，使其成为一个无边框的顶层窗口
	: QWidget(parent, Qt::Tool | Qt::FramelessWindowHint)
{
	setMinimumSize(280, 400);
	setStyleSheet("background-color: #F0F0F0; border: 1px solid #B0B0B0;");

	// --- 标题栏 ---
	QLabel* titleLabel = new QLabel(tr("style"));
	titleLabel->setStyleSheet("QLabel { border: none; }");
	QToolButton* closeButton = new QToolButton(this);
	closeButton->setIcon(QIcon(":/image/icons/close.svg"));
	closeButton->setStyleSheet(R"(
		QToolButton { border: none; }
		QToolButton:hover { background: #D0D0D0; border: flat; })");
	connect(closeButton, &QToolButton::clicked, this, &KStylesFloatingWindow::hide);

	QHBoxLayout* titleLayout = new QHBoxLayout();
	titleLayout->addWidget(titleLabel);
	titleLayout->addStretch();
	titleLayout->addWidget(closeButton);

	// --- 列表 (暂时用QListWidget占位) ---
	QListWidget* styleList = new QListWidget(this);
	styleList->addItems({
		tr("main text"),
		tr("title 1"),
		tr("title 2"),
		tr("title 3"),
		tr("title 4")});

	// --- 底部选项 ---
	QCheckBox* previewCheck = new QCheckBox(tr("preview show"));
	previewCheck->setStyleSheet("QCheckBox { border:none; }");
	// ... 未来可连接此信号以更新列表项的显示

	// --- 整体布局 ---
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(titleLayout);
	mainLayout->addWidget(styleList);
	mainLayout->addWidget(previewCheck);
}

void KStylesFloatingWindow::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_dragPosition = event->globalPos() - frameGeometry().topLeft();
		event->accept();
	}
}

void KStylesFloatingWindow::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		move(event->globalPos() - m_dragPosition);
		event->accept();
	}
}