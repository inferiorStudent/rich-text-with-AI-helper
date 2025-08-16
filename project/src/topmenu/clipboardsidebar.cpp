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
	setFixedWidth(300); // ����һ���̶��Ŀ��
	// ���ñ߿�ͱ���ɫ
	setStyleSheet("QWidget { background-color: #F0F0F0; border-right: 1px solid #D0D0D0; }");

	// --- ���������� ---
	QLabel* titleLabel = new QLabel(tr("clipboard"));
	m_closeButton = new QToolButton(this);
	m_closeButton->setIcon(QIcon(":/image/icons/close.svg"));
	m_closeButton->setStyleSheet(R"(
		QToolButton { border: none; }
		QToolButton:hover { border-style: flat; background: #D0D0D0; })");
	// m_closeButton->setAutoRaise(true); // ͸����������ͣʱ��ʾ�߿�

	connect(m_closeButton, &QToolButton::clicked, this, &ClipboardSidebar::closeRequested);

	QHBoxLayout* topLayout = new QHBoxLayout();
	topLayout->addWidget(titleLabel);
	topLayout->addStretch();
	topLayout->addWidget(m_closeButton);

	// --- ���ܰ�ť ---
	QPushButton * pasteAllButton = new QPushButton(tr("paste all"));
	QPushButton * clearAllButton = new QPushButton(tr("clear all"));
	connect(pasteAllButton, &QPushButton::clicked, this, &ClipboardSidebar::pasteAllRequested);
	connect(clearAllButton, &QPushButton::clicked, this, &ClipboardSidebar::clearAllRequested);
	
	QHBoxLayout * buttonLayout = new QHBoxLayout();
	buttonLayout->addWidget(pasteAllButton);
	buttonLayout->addWidget(clearAllButton);
	buttonLayout->addStretch();

	// --- ������ (��ʱ��һ����ǩռλ) ---
	QLabel* contentPlaceholder = new QLabel(tr("...."));
	contentPlaceholder->setWordWrap(true);
	contentPlaceholder->setAlignment(Qt::AlignTop);

	// --- ���岼�� ---
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(5, 5, 5, 5);
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(buttonLayout);
	mainLayout->addWidget(new QFrame()); // ��һ���յ�Frame���ʣ��ռ�
	mainLayout->addWidget(contentPlaceholder, 1); // 1��ʾռ�����п���չ�ռ�
}