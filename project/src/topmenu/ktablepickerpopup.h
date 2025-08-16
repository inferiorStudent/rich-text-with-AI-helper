#ifndef KTABLEPICKERPOPUP_H
#define KTABLEPICKERPOPUP_H

#ifdef KTABLEPICKERPOPUP_EXPORTS
    #define KTABLEPICKERPOPUP_API __declspec(dllexport)
#else
    #define KTABLEPICKERPOPUP_API __declspec(dllimport)
#endif

#include <QWidget>

class QLabel;

class KTABLEPICKERPOPUP_API KTablePickerPopup : public QWidget
{
	Q_OBJECT
public:
	explicit KTablePickerPopup(int rows = 8, int cols = 10, QWidget* parent = nullptr);

signals:
	void tableSizeSelected(int rows, int cols);

protected:
	void paintEvent(QPaintEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void leaveEvent(QEvent* event) override;

private:
	QLabel* m_titleLabel;
	int m_rows;
	int m_cols;
	int m_cellWidth = 18;
	int m_cellHeight = 18;
	int m_currentRows = 0;
	int m_currentCols = 0;
};

#endif // KTABLEPICKERPOPUP_H