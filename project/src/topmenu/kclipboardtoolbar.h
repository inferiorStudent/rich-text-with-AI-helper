#ifndef KCLIPBOARDTOOLBAR_H
#define KCLIPBOARDTOOLBAR_H

#ifdef KCLIPBOARDTOOLBAR_EXPORTS
    #define KCLIPBOARDTOOLBAR_API __declspec(dllexport)
#else
    #define KCLIPBOARDTOOLBAR_API __declspec(dllimport)
#endif

#include <QWidget>
class QToolButton;

class KCLIPBOARDTOOLBAR_API KClipboardToolbar : public QWidget
{
	Q_OBJECT
public:
	explicit KClipboardToolbar(QWidget* parent = nullptr);
	QToolButton* pasteButton() const;

signals:
	void expandRequested();

private slots:
	void onClipboardChanged();

private:
	QToolButton* m_pasteButton;
	QToolButton* m_expandButton;
};

#endif // KCLIPBOARDTOOLBAR_H