#include "kstylesfloatingwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QToolButton>
#include <QCheckBox>
#include <QMouseEvent>

KStylesFloatingWindow::KStylesFloatingWindow(QWidget* parent)
// ���ô��ڱ�־��ʹ���Ϊһ���ޱ߿�Ķ��㴰��
	: QWidget(parent, Qt::Tool | Qt::FramelessWindowHint)
{
	setMinimumSize(280, 400);
	setStyleSheet("background-color: #F0F0F0; border: 1px solid #B0B0B0;");

	// --- ������ ---
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

	// --- �б� (��ʱ��QListWidgetռλ) ---
	QListWidget* styleList = new QListWidget(this);
	styleList->addItems({
		tr("main text"),
		tr("title 1"),
		tr("title 2"),
		tr("title 3"),
		tr("title 4")});

	// --- �ײ�ѡ�� ---
	QCheckBox* previewCheck = new QCheckBox(tr("preview show"));
	previewCheck->setStyleSheet("QCheckBox { border:none; }");
	// ... δ�������Ӵ��ź��Ը����б������ʾ

	// --- ���岼�� ---
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