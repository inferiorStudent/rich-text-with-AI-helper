#ifndef KHEADEREDIT_H
#define KHEADEREDIT_H

#include <QTextEdit>

class KHeaderEdit : public QTextEdit
{
	Q_OBJECT
public:
	explicit KHeaderEdit(QWidget* parent = nullptr);

signals:
	void editingFinished(); // ʧȥ����ʱ�������ź�

protected:
	void focusOutEvent(QFocusEvent* e) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
};

#endif // KHEADEREDIT_H