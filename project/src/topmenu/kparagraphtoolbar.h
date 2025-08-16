#ifndef KPARAGRAPHTOOLBAR_H
#define KPARAGRAPHTOOLBAR_H

#ifdef KPARAGRAPHTOOLBAR_EXPORTS
    #define KPARAGRAPHTOOLBAR_API __declspec(dllexport)
#else
    #define KPARAGRAPHTOOLBAR_API __declspec(dllimport)
#endif

#include <QWidget>
#include <QTextCharFormat>

class QToolButton;
class QComboBox;
class QButtonGroup;

class KPARAGRAPHTOOLBAR_API KParagraphToolbar : public QWidget
{
	Q_OBJECT
public:
	explicit KParagraphToolbar(QWidget* parent = nullptr);

public slots:
	void updateFormat(const QTextBlockFormat& blockFormat, const QTextListFormat& listFormat);

signals:
	void alignmentChanged(Qt::Alignment alignment);
	void listStyleChanged(QTextListFormat::Style style);
	void lineHeightChanged(qreal factor);

private:
	QButtonGroup* m_alignGroup;
	QToolButton* m_leftAlignButton;
	QToolButton* m_centerAlignButton;
	QToolButton* m_rightAlignButton;
	QToolButton* m_justifyButton;

	QToolButton* m_bulletListButton;
	QToolButton* m_numberListButton;

	QComboBox* m_lineHeightCombo;
};

#endif // KPARAGRAPHTOOLBAR_H