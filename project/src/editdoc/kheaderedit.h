#ifndef KHEADEREDIT_H
#define KHEADEREDIT_H

#include <QTextEdit>

class KHeaderEdit : public QTextEdit
{
	Q_OBJECT
public:
	explicit KHeaderEdit(QWidget* parent = nullptr);

signals:
	void editingFinished(); // 失去焦点时发出此信号

protected:
	void focusOutEvent(QFocusEvent* e) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
};

#endif // KHEADEREDIT_H