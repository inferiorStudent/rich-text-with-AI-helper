#ifndef KSTYLESTOOLBAR_H
#define KSTYLESTOOLBAR_H

#ifdef KSTYLESTOOLBAR_EXPORTS
    #define KSTYLESTOOLBAR_API __declspec(dllexport)
#else
    #define KSTYLESTOOLBAR_API __declspec(dllimport)
#endif

#include <QWidget>
#include "stylemanager.h"

class KSTYLESTOOLBAR_API KStylesToolbar : public QWidget
{
	Q_OBJECT
public:
	explicit KStylesToolbar(StyleManager* styleManager, QWidget* parent = nullptr);

signals:
	void stylePanelRequested(const QPoint& pos);
	void styleApplied(int styleId);
};

#endif // KSTYLESTOOLBAR_H