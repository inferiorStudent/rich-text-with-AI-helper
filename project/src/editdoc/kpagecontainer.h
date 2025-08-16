#ifndef KPAGECONTAINER_H
#define KPAGECONTAINER_H

#include <QWidget>
#include <QPageSize>
#include <QPageLayout>
#include <QRect>
#include <QMargins>

class KAIEditor;
class KHeaderEdit;

class KPageContainer : public QWidget
{
	Q_OBJECT
public:
	explicit KPageContainer(QWidget* parent = nullptr);

	KAIEditor* page() const;
	KHeaderEdit* header() const;
	KHeaderEdit* footer() const;

public slots:
	void updateMetrics(const QPageSize& pageSize, QPageLayout::Orientation orientation);
	void setPageNumber(int num);
	void updatePageMargin(const QMargins& marginSize);
	void insertPageNumber(bool flag);

protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent * event) override;

private slots:
	void enterHeaderEditMode();
	void enterFooterEditMode();
	void exitHeaderEditMode();

private:
	KAIEditor* m_page;
	KHeaderEdit* m_headerEdit;
	KHeaderEdit* m_footerEdit;
	QSize m_paperPixelSize;
	QSize m_editSize;
	QMargins m_pageMargin;
	int m_pageNumber = 0;
	int m_drawNumber = false;
};

#endif // KPAGECONTAINER_H