#include "kheaderedit.h"

KHeaderEdit::KHeaderEdit(QWidget* parent) : QTextEdit(parent)
{
	// 初始样式
	setStyleSheet("background: transparent; border: none;");
}

void KHeaderEdit::focusOutEvent(QFocusEvent* e)
{
	QTextEdit::focusOutEvent(e);
	// 当失去焦点时，发出信号通知父级退出编辑模式
	emit editingFinished();
}

void KHeaderEdit::mouseDoubleClickEvent(QMouseEvent* event)
{
	setReadOnly(false);
	setStyleSheet("border: 1px dashed gray; background: transparent;");
	setFocus();
}