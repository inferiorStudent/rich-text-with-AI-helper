#ifndef CLIPBOARDSIDEBAR_H
#define CLIPBOARDSIDEBAR_H

#ifdef CLIPBOARDSIDEBAR_EXPORTS
    #define CLIPBOARDSIDEBAR_API __declspec(dllexport)
#else
    #define CLIPBOARDSIDEBAR_API __declspec(dllimport)
#endif

#include <QWidget>

class QToolButton;

class CLIPBOARDSIDEBAR_API ClipboardSidebar : public QWidget
{
	Q_OBJECT
public:
	explicit ClipboardSidebar(QWidget* parent = nullptr);

signals:
	void closeRequested();
	void pasteAllRequested();
	void clearAllRequested();

private:
	QToolButton* m_closeButton;
};

#endif // CLIPBOARDSIDEBAR_H