#include "kstylebutton.h"
#include <QVBoxLayout>
#include <QLabel>

KStyleButton::KStyleButton(const TextStyle& style, QWidget* parent)
	: QToolButton(parent)
	, m_style(style)
{
	setCheckable(true);
	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	setFixedSize(100, 80); // 设定一个合适的大小
	setStyleSheet(R"(
		QToolButton { border: none; }
		QToolButton:hover { border: 10px solid #D0D0D0; }
		QToolButton:checked { border: 10px solid #C0C0C0; })");

	m_previewLabel = new QLabel("AaBbCcDd");
	m_previewLabel->setStyleSheet("QLabel { border: none; }");
	m_previewLabel->setAlignment(Qt::AlignLeft);

	// 应用样式到预览标签
	m_previewLabel->setFont(m_style.charFormat.font());
	QPalette pal = m_previewLabel->palette();
	pal.setColor(QPalette::WindowText, m_style.charFormat.foreground().color());
	m_previewLabel->setPalette(pal);

	m_nameLabel = new QLabel(m_style.name);
	m_nameLabel->setStyleSheet("QLabel { border: none; }");
	m_nameLabel->setAlignment(Qt::AlignCenter);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(m_previewLabel);
	layout->addWidget(m_nameLabel);
	setLayout(layout);
}

int KStyleButton::styleId() const
{
	return m_style.id;
}