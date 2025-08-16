#ifndef KFONTTOOLBAR_H
#define KFONTTOOLBAR_H

#include <QWidget>
#include <QTextCharFormat>

#ifdef KFONTTOOLBAR_EXPORTS
    #define KFONTTOOLBAR_API __declspec(dllexport)
#else
    #define KFONTTOOLBAR_API __declspec(dllimport)
#endif

class QToolButton;
class QFontComboBox;
class QComboBox;

class KFONTTOOLBAR_API KFontToolbar : public QWidget
{
	Q_OBJECT
public:
	explicit KFontToolbar(QWidget* parent = nullptr);

public slots:
	void updateFormat(const QTextCharFormat& format);

signals:
	void boldTriggered(bool checked);
	void italicTriggered(bool checked);
	void underlineTriggered(bool checked);
	void fontFamilyChanged(const QString& family);
	void fontSizeChanged(int size);
	void colorChanged(const QColor& color);

private slots:
	void onColorButtonClicked();
	void applyColor(const QColor& color);

private:
	void createColorIcon(const QColor& color);

	QToolButton* m_boldButton;
	QToolButton* m_italicButton;
	QToolButton* m_underlineButton;
	QToolButton* m_colorButton;
	QFontComboBox* m_fontCombo;
	QComboBox* m_fontSizeCombo;

	QColor m_currentColor;
};


#endif // KFONTTOOLBAR_H