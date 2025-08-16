#include "kparagraphtoolbar.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QComboBox>
#include <QLabel>
#include <QButtonGroup>
#include <QIcon>

KParagraphToolbar::KParagraphToolbar(QWidget* parent)
	: QWidget(parent)
{
	QString toolButtonStyle = R"(
		QToolButton { border-style: flat; background: transparent; }
		QToolButton:hover { border-style: flat; background: #D0D0D0; }
		QToolButton:checked { border-style: flat; background: #D0D0D0; })";
	// --- 第一行：对齐和列表 ---
	QHBoxLayout* row1 = new QHBoxLayout();
	row1->setSpacing(5);

	// 对齐按钮组
	m_alignGroup = new QButtonGroup(this);
	m_alignGroup->setExclusive(true);

	m_leftAlignButton = new QToolButton(this);
	m_leftAlignButton->setIcon(QIcon(":/image/icons/paragraph/left_align.svg"));
	m_leftAlignButton->setCheckable(true);
	m_leftAlignButton->setStyleSheet(toolButtonStyle);
	m_alignGroup->addButton(m_leftAlignButton, static_cast<int>(Qt::AlignLeft));

	m_centerAlignButton = new QToolButton(this);
	m_centerAlignButton->setIcon(QIcon(":/image/icons/paragraph/center_align.svg"));
	m_centerAlignButton->setCheckable(true);
	m_centerAlignButton->setStyleSheet(toolButtonStyle);
	m_alignGroup->addButton(m_centerAlignButton, static_cast<int>(Qt::AlignCenter));

	m_rightAlignButton = new QToolButton(this);
	m_rightAlignButton->setIcon(QIcon(":/image/icons/paragraph/right_align.svg"));
	m_rightAlignButton->setCheckable(true);
	m_rightAlignButton->setStyleSheet(toolButtonStyle);
	m_alignGroup->addButton(m_rightAlignButton, static_cast<int>(Qt::AlignRight));

	m_justifyButton = new QToolButton(this);
	m_justifyButton->setIcon(QIcon(":/image/icons/paragraph/justify.svg"));
	m_justifyButton->setCheckable(true);
	m_justifyButton->setStyleSheet(toolButtonStyle);
	m_alignGroup->addButton(m_justifyButton, static_cast<int>(Qt::AlignJustify));

	connect(m_alignGroup, QOverload<int>::of(&QButtonGroup::idClicked), this, [this](int id) {
		emit alignmentChanged(static_cast<Qt::Alignment>(id));
		});

	row1->addWidget(m_leftAlignButton);
	row1->addWidget(m_centerAlignButton);
	row1->addWidget(m_rightAlignButton);
	row1->addWidget(m_justifyButton);

	// 列表按钮
	m_bulletListButton = new QToolButton(this);
	m_bulletListButton->setIcon(QIcon(":/image/icons/paragraph/bullet_list.svg"));
	m_bulletListButton->setStyleSheet(toolButtonStyle);;
	connect(m_bulletListButton, &QToolButton::clicked, this, [this]() {
		emit listStyleChanged(QTextListFormat::ListDisc);
		});

	m_numberListButton = new QToolButton(this);
	m_numberListButton->setIcon(QIcon(":/image/icons/paragraph/number_list.svg"));
	m_numberListButton->setStyleSheet(toolButtonStyle);
	connect(m_numberListButton, &QToolButton::clicked, this, [this]() {
		emit listStyleChanged(QTextListFormat::ListDecimal);
		});

	row1->addWidget(m_bulletListButton);
	row1->addWidget(m_numberListButton);
	row1->setAlignment(Qt::AlignLeft);

	// --- 第二行：行间距 ---
	QHBoxLayout* row2 = new QHBoxLayout();
	m_lineHeightCombo = new QComboBox(this);
	m_lineHeightCombo->addItem("1.0", 100);
	m_lineHeightCombo->addItem("1.15", 115);
	m_lineHeightCombo->addItem("1.5", 150);
	m_lineHeightCombo->addItem("2.0", 200);
	m_lineHeightCombo->addItem("2.5", 250);
	m_lineHeightCombo->addItem("3.0", 300);
	m_lineHeightCombo->setStyleSheet(R"(
		QComboBox::drop-down {
			image: url(:/image/icons/arrow_down.svg);
			width: 23px;
		}
		QComboBox:hover { border-style: flat; background: #D0D0D0; }
		QComboBox {
			image: url(:/image/icons/paragraph/line_dist.svg);
			border: none; color: transparent; background-position: 4px center;
			width: 2px; background-repeat: no-repeat; height: 23px;
			padding-right: 20px;
		})");

	connect(m_lineHeightCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
		emit lineHeightChanged(m_lineHeightCombo->itemData(index).toInt() / 100.0);
		});
	row2->addWidget(m_lineHeightCombo);
	row2->setAlignment(Qt::AlignLeft);
	// row2->addStretch();

	// --- 整体布局 ---
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(row1);
	mainLayout->addLayout(row2);
	mainLayout->addStretch();
	QLabel* label = new QLabel(tr("paragraph"), this);
	label->setStyleSheet(R"(QLabel { background-color: transparent; border: none; })");
	mainLayout->addWidget(label, 0, Qt::AlignHCenter);
}

void KParagraphToolbar::updateFormat(const QTextBlockFormat & blockFormat, const QTextListFormat & listFormat)
{
	// 更新对齐按钮的状态
	Qt::Alignment alignment = blockFormat.alignment();
	QAbstractButton * button = m_alignGroup->button(static_cast<int>(alignment));
	if (button)
	{
		button->setChecked(true);
	}
	// listFormat.style() 现在可用，如果未来需要根据光标是否在列表中来更新UI，可以在这里添加逻辑
}