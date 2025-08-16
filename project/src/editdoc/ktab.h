#ifndef KTAB_H
#define KTAB_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QMap>

// --- ������ǩ�� (�����) ---
class KTabItem : public QWidget
{
	Q_OBJECT
public:
	// ���캯��������Ҫһ��ID������ʶ��
	explicit KTabItem(int id, const QString& text, QWidget* parent = nullptr);
	void setActive(bool active);
	int id() const { return m_id; }

signals:
	void clicked(int id);
	void closeRequested(int id);

protected:
	// �� mousePressEvent �������ź�
	void mousePressEvent(QMouseEvent* event) override;

private:
	int m_id;
	QLabel* m_label;
	QPushButton* m_closeButton;
};


// --- ��ǩ������ (������ǿ) ---
class KTab : public QWidget
{
	Q_OBJECT
public:
	explicit KTab(QWidget* parent = nullptr);
	// addTab���ڷ�����������TabItem��ID
	int addTab(const QString& title);
	void setTabText(int id, const QString& text);
	int activeTabId() const;
	QString tabText(int id) const;

signals:
	// ����ǩ���رպ󣬷�����źţ�֪ͨMainWindow��������
	void tabClosed(int id);
	// �����ǩ�ı�ʱ��������ź�
	void currentChanged(int id);

public slots:
	// �ڲ��ۣ����ڴ�������KTabItem���ź�
	void onTabClicked(int id);
	void onCloseTab(int id);

private:
	QHBoxLayout* m_layout;
	QMap<int, KTabItem*> m_tabs; // ʹ��Mapͨ��ID�������ǩ
	int m_nextTabId;             // ��������Ψһ�ı�ǩID
	int m_activeTabId;           // ��ǰ����ı�ǩID
};

#endif // KTAB_H