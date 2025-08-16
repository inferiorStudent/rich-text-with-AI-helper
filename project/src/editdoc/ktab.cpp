#include "ktab.h"
#include <QMouseEvent>
#include <QFont>
#include <QStyle>

KTabItem::KTabItem(int id, const QString& text, QWidget* parent)
	: QWidget(parent)
	, m_id(id)
{
	// 要让QWidget应用QSS中的背景色、边框等，必须设置此属性
	setAttribute(Qt::WA_StyledBackground, true);

	setFixedHeight(40);
	setMinimumWidth(100);
	setMaximumWidth(220);
	setCursor(Qt::PointingHandCursor);

	// 标签文字
	m_label = new QLabel(text, this);
	QFont font = m_label->font();
	font.setPointSize(10);
	m_label->setFont(font);

	// 关闭按钮
	m_closeButton = new QPushButton(this);
	m_closeButton->setFixedSize(16, 16);
	m_closeButton->setCursor(Qt::ArrowCursor);
	m_closeButton->setStyleSheet(QStringLiteral(R"(
        QPushButton {
            border-image: url(:/image/normal/close.svg);
            background-color: transparent;
            border-radius: 8px;
        }
        QPushButton:hover {
            border-image: url(:/image/hover/close.svg);
            background-color: transparent;
        }
    )"));
	// 这里我们将关闭按钮的信号连接到一个lambda表达式，
	// 该表达式捕获当前item的ID，并发出带有ID的closeRequested信号。
	// 这就解决了参数不匹配的问题。
	connect(m_closeButton, &QPushButton::clicked, this, [this]() {
		emit closeRequested(m_id);
		});

	// 布局
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(15, 0, 8, 0);
	layout->addWidget(m_label);
	layout->addStretch();
	layout->addWidget(m_closeButton);
}

void KTabItem::setActive(bool active)
{
	// 通过设置一个动态属性来改变样式，这是QSS选择器的强大之处
	this->setProperty("active", active);
	// 刷新样式表
	this->style()->unpolish(this);
	this->style()->polish(this);
}

void KTabItem::mousePressEvent(QMouseEvent* event)
{
	// 如果点击的不是关闭按钮区域，则认为是点击了标签本身
	if (!m_closeButton->geometry().contains(event->pos()))
	{
		emit clicked(m_id);
	}
	// 让父类处理事件，以防需要
	QWidget::mousePressEvent(event);
}


// --- KTab 实现 (完整的标签管理逻辑) ---
KTab::KTab(QWidget* parent)
	: QWidget(parent)
	, m_nextTabId(0)
	, m_activeTabId(-1)
{
	setFixedHeight(41);
	// setStyleSheet("background-color: #D0D0D0;");

	m_layout = new QHBoxLayout(this);
	m_layout->setContentsMargins(5, 1, 0, 0);
	m_layout->setSpacing(0); // 标签之间无间隔
	m_layout->addStretch();
}

int KTab::addTab(const QString& title)
{
	int currentId = m_nextTabId++;
	KTabItem* tab = new KTabItem(currentId, title, this);

	// 关键QSS：使用属性选择器来区分激活和非激活状态
	tab->setStyleSheet(R"(
        KTabItem {
            background-color: #F0F0F0;
            border: 1px solid #C0C0C0;
            border-bottom: none;
        }
        KTabItem[active="true"] {
            background-color: white; /* 激活时为白色 */
            border-top-right-radius: 8px; /* 仅右上角圆角 */
        }
        KTabItem[active="false"] {
            background-color: #E0E0E0; /* 非激活时为灰色 */
            border-top-right-radius: 8px; /* 仅右上角圆角 */
        }
    )");

	// 连接来自TabItem的信号到KTab的槽
	connect(tab, &KTabItem::clicked, this, &KTab::onTabClicked);
	connect(tab, &KTabItem::closeRequested, this, &KTab::onCloseTab);

	// 插入到布局和Map中
	m_layout->insertWidget(m_layout->count() - 1, tab);
	m_tabs[currentId] = tab;

	// 新添加的标签自动成为活动标签
	onTabClicked(currentId);

	return currentId;
}

void KTab::onTabClicked(int id)
{
	if (m_activeTabId == id) return; // 如果已经是激活的，则不操作

	// 取消之前激活的标签
	if (m_tabs.contains(m_activeTabId))
	{
		m_tabs[m_activeTabId]->setActive(false);
	}

	// 激活新点击的标签
	if (m_tabs.contains(id))
	{
		m_tabs[id]->setActive(true);
		m_activeTabId = id;
		emit currentChanged(id); // 发射信号，通知外部激活的标签已改变
	}
}

void KTab::onCloseTab(int id)
{
	if (!m_tabs.contains(id))
		return;

	KTabItem* tab_to_close = m_tabs[id];

	// 从Map和布局中移除
	m_tabs.remove(id);
	m_layout->removeWidget(tab_to_close);
	tab_to_close->deleteLater(); // 安全删除

	// 如果关闭的是活动标签，需要选择一个新的活动标签
	if (m_activeTabId == id)
	{
		if (!m_tabs.isEmpty())
		{
			// 简单地选择最后一个标签作为新的活动标签
			onTabClicked(m_tabs.lastKey());
		}
		else
		{
			m_activeTabId = -1; // 没有标签了
		}
	}

	emit tabClosed(id); // 发射信号，通知MainWindow这个ID对应的文件可以关闭了
}

int KTab::activeTabId() const
{
	return m_activeTabId;
}

QString KTab::tabText(int id) const
{
	if (m_tabs.contains(id))
	{
		QLabel* label = m_tabs[id]->findChild<QLabel*>();
		if (label)
		{
			return label->text();
		}
	}
	return QString();
}

void KTab::setTabText(int id, const QString& text)
{
	if (m_tabs.contains(id))
	{
		// KTabItem内部没有直接设置文本的公共接口，我们通过查找其子控件QLabel来修改
		QLabel* label = m_tabs[id]->findChild<QLabel*>();
		if (label)
		{
			label->setText(text);
		}
	}
}