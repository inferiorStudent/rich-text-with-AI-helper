#ifndef KSTYLEBUTTON_H
#define KSTYLEBUTTON_H

#ifdef KSTYLEBUTTON_EXPORTS
    #define KSTYLEBUTTON_API __declspec(dllexport)
#else
    #define KSTYLEBUTTON_API __declspec(dllimport)
#endif

#include <QToolButton>
#include "textstyle.h"

class QLabel;

class KSTYLEBUTTON_API KStyleButton : public QToolButton
{
	Q_OBJECT
public:
	explicit KStyleButton(const TextStyle& style, QWidget* parent = nullptr);
	int styleId() const;

private:
	TextStyle m_style;
	QLabel* m_previewLabel;
	QLabel* m_nameLabel;
};

#endif // KSTYLEBUTTON_H