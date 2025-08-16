#include "ktopmenu.h"

KTopMenu::KTopMenu(QWidget* parent)
	: QWidget(parent)
{
	setFixedHeight(35);
	setStyleSheet("QWidget { background-color: #F0F0F0; border: none; }");


	m_layout = new QHBoxLayout(this);
	m_layout->setContentsMargins(2, 0, 0, 0);
	m_layout->setSpacing(2);
	m_layout->setAlignment(Qt::AlignCenter);

	m_buttonGroup = new QButtonGroup(this);
	m_buttonGroup->setExclusive(true);

	connect(m_buttonGroup, SIGNAL(idClicked(int)), this, SLOT(onMenuButtonClicked(int)));

	m_activeStyle = R"(
        QPushButton {
            background-color: white;
            color: #3399FF;
            border: none;
            border-bottom: 2px solid #3399FF;
            padding: 5px 15px;
        }
    )";

	// 非激活状态
	m_inactiveStyle = R"(
        QPushButton {
            background-color: transparent;
            color: black;
            border: none;
            padding: 5px 15px;
        }
        QPushButton:hover {
            background-color: #E0E0E0;
        }
    )";
	addMenu(tr("start"));
	addMenu(tr("insert"));
	addMenu(tr("page"));
	addMenu(QIcon(":/image/icons/ai.svg"));
	// m_layout->addStretch();
	if (!m_buttonGroup->buttons().isEmpty())
		m_buttonGroup->button(0)->click();
}

void KTopMenu::addMenu(const QString& title)
{
	QPushButton* button = new QPushButton(title, this);
	button->setCheckable(true);
	button->setCursor(Qt::PointingHandCursor);
	button->setFlat(true);
	button->setFixedWidth(75);

	// 调整字体大小
	QFont font = button->font();
	font.setPointSize(10);
	button->setFont(font);

	int id = m_buttonGroup->buttons().size();
	m_buttonGroup->addButton(button, id);
	// 在弹簧伸展项之前插入按钮
	m_layout->insertWidget(id, button);

	if (id == 0)
		button->click();
	else
		button->setStyleSheet(m_inactiveStyle);
}

void KTopMenu::addMenu(const QIcon& icon)
{
	QPushButton* button = new QPushButton;
	button->setIcon(icon);
	button->setCheckable(true);
	button->setCursor(Qt::PointingHandCursor);
	button->setFlat(true);
	button->setFixedWidth(75);

	int id = m_buttonGroup->buttons().size();
	m_buttonGroup->addButton(button, id);
	m_layout->insertWidget(id, button);

	if (id == 0)
		button->click();
	else
		button->setStyleSheet(m_inactiveStyle);
}

void KTopMenu::onMenuButtonClicked(int id)
{
	updateButtonStyles(id);
	emit currentChanged(id);
}

void KTopMenu::updateButtonStyles(int activeId)
{
	for (auto button : m_buttonGroup->buttons())
	{
		int currentId = m_buttonGroup->id(button);
		if (currentId == activeId)
		{
			button->setStyleSheet(m_activeStyle);
		}
		else
		{
			button->setStyleSheet(m_inactiveStyle);
		}
	}
}