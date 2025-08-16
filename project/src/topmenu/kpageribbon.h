#ifndef KPAGERIBBON_H
#define KPAGERIBBON_H

#ifdef KPAGERIBBON_EXPORTS
    #define KPAGERIBBON_API __declspec(dllexport)
#else
    #define KPAGERIBBON_API __declspec(dllimport)
#endif

#include <QWidget>
#include <QPageSize>
#include <QPageLayout>
#include <QIcon>
#include <QToolButton>
#include <QMargins>

class KPAGERIBBON_API KPageRibbon : public QWidget
{
	Q_OBJECT
public:
	explicit KPageRibbon(QWidget* parent = nullptr);
	QToolButton* createButton(const QIcon& icon, const QString& text);

signals:
	void insertPageRequested();
	void pageSizeChanged(const QPageSize& size);
	void pageOrientationChanged(QPageLayout::Orientation orientation);
	void pageMarginChanged(const QMargins& marginSize);
	void insertPageNumber(bool flag);

private:
	QString m_buttonStyle;
	QString m_menuStyle;
};

#endif // KPAGERIBBON_H