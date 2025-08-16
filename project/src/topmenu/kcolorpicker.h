#ifndef KCOLORPICKER_H
#define KCOLORPICKER_H

#ifdef KCOLORPICKER_EXPORTS
    #define KCOLORPICKER_API __declspec(dllexport)
#else
    #define KCOLORPICKER_API __declspec(dllimport)
#endif

#include <QWidget>
#include <QColor>

class KCOLORPICKER_API KColorPicker : public QWidget
{
    Q_OBJECT
public:
    explicit KColorPicker(QWidget *parent = nullptr);

signals:
    void colorSelected(const QColor &color);
};

#endif // KCOLORPICKER_H