#ifndef KSTYLESFLOATINGWINDOW_H
#define KSTYLESFLOATINGWINDOW_H

#ifdef KSTYLESFLOATINGWINDOW_EXPORTS
    #define KSTYLESFLOATINGWINDOW_API __declspec(dllexport)
#else
    #define KSTYLESFLOATINGWINDOW_API __declspec(dllimport)
#endif

#include <QWidget>
#include <QPoint>

class KSTYLESFLOATINGWINDOW_API KStylesFloatingWindow : public QWidget
{
	Q_OBJECT
public:
	explicit KStylesFloatingWindow(QWidget* parent = nullptr);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	QPoint m_dragPosition;
};

#endif // KSTYLESFLOATINGWINDOW_H