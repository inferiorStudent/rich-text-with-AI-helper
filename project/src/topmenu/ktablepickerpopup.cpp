#include "ktablepickerpopup.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>

KTablePickerPopup::KTablePickerPopup(int rows, int cols, QWidget* parent)
	: QWidget(parent)
	, m_rows(rows)
	, m_cols(cols)
{
	setMouseTracking(true); // 必须开启才能响应未按下鼠标的移动事件
	setFixedSize(m_cols * m_cellWidth + 20, m_rows * m_cellHeight + 50);

	m_titleLabel = new QLabel(tr("insert table"), this);
	m_titleLabel->setAlignment(Qt::AlignLeft);
	m_titleLabel->setStyleSheet(R"( QLabel { border: none; })");

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(m_titleLabel);
	layout->addStretch();
}

void KTablePickerPopup::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	for (int r = 0; r < m_rows; ++r)
	{
		for (int c = 0; c < m_cols; ++c)
		{
			QRect cellRect(10 + c * m_cellWidth, 40 + r * m_cellHeight, m_cellWidth - 2, m_cellHeight - 2);
			if (r < m_currentRows && c < m_currentCols)
			{
				// 高亮状态
				painter.fillRect(cellRect, QColor("#FDE2CC"));
				painter.setPen(QPen(QColor("#F57E0A"), 1));
			}
			else
			{
				// 普通状态
				painter.fillRect(cellRect, Qt::white);
				painter.setPen(QPen(Qt::gray, 1));
			}
			painter.drawRect(cellRect);
		}
	}
}

void KTablePickerPopup::mouseMoveEvent(QMouseEvent* event)
{
	int x = event->x() - 10;
	int y = event->y() - 40;

	int newCols = qBound(0, (x / m_cellWidth) + 1, m_cols);
	int newRows = qBound(0, (y / m_cellHeight) + 1, m_rows);

	if (newCols != m_currentCols || newRows != m_currentRows)
	{
		m_currentCols = newCols;
		m_currentRows = newRows;
		if (m_currentCols > 0 && m_currentRows > 0)
		{
			m_titleLabel->setText(tr("%1 plus %2 table").arg(m_currentCols).arg(m_currentRows));
		}
		else
		{
			m_titleLabel->setText(tr("insert table"));
		}
		update();
	}
}

void KTablePickerPopup::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && m_currentRows > 0 && m_currentCols > 0)
	{
		emit tableSizeSelected(m_currentRows, m_currentCols);
		window()->close();
	}
}

void KTablePickerPopup::leaveEvent(QEvent* event)
{
	Q_UNUSED(event);
	m_currentRows = 0;
	m_currentCols = 0;
	m_titleLabel->setText(tr("insert table"));
	update();
}