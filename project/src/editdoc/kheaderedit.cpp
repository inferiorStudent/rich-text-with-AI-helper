#include "kheaderedit.h"

KHeaderEdit::KHeaderEdit(QWidget* parent) : QTextEdit(parent)
{
	// ��ʼ��ʽ
	setStyleSheet("background: transparent; border: none;");
}

void KHeaderEdit::focusOutEvent(QFocusEvent* e)
{
	QTextEdit::focusOutEvent(e);
	// ��ʧȥ����ʱ�������ź�֪ͨ�����˳��༭ģʽ
	emit editingFinished();
}

void KHeaderEdit::mouseDoubleClickEvent(QMouseEvent* event)
{
	setReadOnly(false);
	setStyleSheet("border: 1px dashed gray; background: transparent;");
	setFocus();
}