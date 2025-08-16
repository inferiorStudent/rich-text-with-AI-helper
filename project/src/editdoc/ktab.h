#ifndef KTAB_H
#define KTAB_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QMap>

// --- 单个标签项 (大幅简化) ---
class KTabItem : public QWidget
{
	Q_OBJECT
public:
	// 构造函数现在需要一个ID，用于识别
	explicit KTabItem(int id, const QString& text, QWidget* parent = nullptr);
	void setActive(bool active);
	int id() const { return m_id; }

signals:
	void clicked(int id);
	void closeRequested(int id);

protected:
	// 用 mousePressEvent 来发射信号
	void mousePressEvent(QMouseEvent* event) override;

private:
	int m_id;
	QLabel* m_label;
	QPushButton* m_closeButton;
};


// --- 标签栏容器 (功能增强) ---
class KTab : public QWidget
{
	Q_OBJECT
public:
	explicit KTab(QWidget* parent = nullptr);
	// addTab现在返回它创建的TabItem的ID
	int addTab(const QString& title);
	void setTabText(int id, const QString& text);
	int activeTabId() const;
	QString tabText(int id) const;

signals:
	// 当标签被关闭后，发射此信号，通知MainWindow清理数据
	void tabClosed(int id);
	// 当活动标签改变时，发射此信号
	void currentChanged(int id);

public slots:
	// 内部槽，用于处理来自KTabItem的信号
	void onTabClicked(int id);
	void onCloseTab(int id);

private:
	QHBoxLayout* m_layout;
	QMap<int, KTabItem*> m_tabs; // 使用Map通过ID来管理标签
	int m_nextTabId;             // 用于生成唯一的标签ID
	int m_activeTabId;           // 当前激活的标签ID
};

#endif // KTAB_H