#ifndef KINSERTRIBBON_H
#define KINSERTRIBBON_H

#ifdef KINSERTRIBBON_EXPORTS
    #define KINSERTRIBBON_API __declspec(dllexport)
#else
    #define KINSERTRIBBON_API __declspec(dllimport)
#endif

#include <QWidget>
#include <QToolButton>

class KTextDoc;

class KINSERTRIBBON_API KInsertRibbon : public QWidget
{
	Q_OBJECT
public:
	explicit KInsertRibbon(KTextDoc* textDoc, QWidget* parent = nullptr);
	QToolButton* createButton(const QIcon& icon, const QString& text);

private:
	QString m_buttonStyle;
	QString m_menuStyle;
};

#endif // KINSERTRIBBON_H