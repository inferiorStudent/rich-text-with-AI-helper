#include "kpagecontainer.h"
#include "kaieditor.h"
#include "kheaderedit.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QScreen>
#include <QGuiApplication>
#include <QMouseEvent>

KPageContainer::KPageContainer(QWidget* parent)
	: QWidget(parent)
	, m_page(new KAIEditor(this))
{
	qreal dpi = QGuiApplication::primaryScreen()->logicalDotsPerInch();
	auto convertMmToPx = [dpi](qreal mm) { return qRound((mm / 25.4) * dpi); };

	QPageSize pageSize(QPageSize::A4);
	QSizeF paperSizeMm = pageSize.size(QPageSize::Millimeter);

	m_paperPixelSize.setWidth(convertMmToPx(paperSizeMm.width()));
	m_paperPixelSize.setHeight(convertMmToPx(paperSizeMm.height()));

	m_pageMargin = QMargins(
		convertMmToPx(25.4), convertMmToPx(31.8),
		convertMmToPx(25.4), convertMmToPx(31.8));
	m_editSize.setWidth(m_paperPixelSize.width() - m_pageMargin.left() - m_pageMargin.right());
	m_editSize.setHeight(m_paperPixelSize.height() - m_pageMargin.top() - m_pageMargin.bottom());

	// --- ����ҳü��ҳ�� ---
	m_headerEdit = new KHeaderEdit(this);
	m_footerEdit = new KHeaderEdit(this);
	m_headerEdit->setFixedHeight(m_pageMargin.top() / 2);   // �̶��߶�
	m_footerEdit->setFixedHeight(m_pageMargin.bottom() / 2);

	// �����źţ���ҳü/��ʧȥ����ʱ�˳��༭ģʽ
	connect(m_headerEdit, &KHeaderEdit::editingFinished, this, &KPageContainer::exitHeaderEditMode);
	connect(m_footerEdit, &KHeaderEdit::editingFinished, this, &KPageContainer::exitHeaderEditMode);

	exitHeaderEditMode(); // ��ʼ״̬Ϊ�Ǳ༭ģʽ

	m_page->setStyleSheet("KAIEditor { background-color: transparent; border: none; font-size: 11pt; }");
	m_page->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(m_page, 0, Qt::AlignCenter);
	setFixedSize(m_paperPixelSize);
}

void KPageContainer::updateMetrics(const QPageSize& pageSize, QPageLayout::Orientation orientation)
{
	qreal dpi = QGuiApplication::primaryScreen()->logicalDotsPerInch();
	auto convertMmToPx = [dpi](qreal mm) { return qRound((mm / 25.4) * dpi); };

	QSizeF paperSizeMm = pageSize.size(QPageSize::Millimeter);
	if (orientation == QPageLayout::Landscape)
	{
		paperSizeMm.transpose();
	}

	m_paperPixelSize.setWidth(convertMmToPx(paperSizeMm.width()));
	m_paperPixelSize.setHeight(convertMmToPx(paperSizeMm.height()));

	m_editSize.setWidth(m_paperPixelSize.width() - m_pageMargin.left() - m_pageMargin.right());
	m_editSize.setHeight(m_paperPixelSize.height() - m_pageMargin.top() - m_pageMargin.bottom());
	m_page->setFixedSize(m_editSize);

	setFixedSize(m_paperPixelSize);
	// resizeEvent(nullptr);
	update();
}

void KPageContainer::setPageNumber(int num)
{
	m_pageNumber = num;
}

KHeaderEdit* KPageContainer::header() const
{
	return m_headerEdit;
}

KHeaderEdit* KPageContainer::footer() const
{
	return m_footerEdit;
}

KAIEditor* KPageContainer::page() const
{
	return m_page;
}

void KPageContainer::resizeEvent(QResizeEvent* event)
{
	// �ֶ���λҳüҳ��
	m_headerEdit->setGeometry(m_pageMargin.left(), m_pageMargin.top() / 2 - 5, m_editSize.width(), m_pageMargin.top() - 10);
	// m_page->setGeometry(m_pageMargin.left(), m_pageMargin.top(), m_editSize.width(), m_editSize.height());
	m_footerEdit->setGeometry(m_pageMargin.left(), height() - m_pageMargin.bottom() + 5, m_editSize.width(), m_pageMargin.bottom() - 10);
	QWidget::resizeEvent(event);
}

void KPageContainer::mouseDoubleClickEvent(QMouseEvent* event)
{
	// ���˫��λ���Ƿ���ҳü����
	if (QRect(0, 0, width(), m_pageMargin.top()).contains(event->pos()))
	{
		enterHeaderEditMode();
	}
	else if (QRect(0, height() - m_pageMargin.bottom(), width(), m_pageMargin.bottom()).contains(event->pos()))
	{
		enterFooterEditMode();
	}
	else
	{
		QWidget::mouseDoubleClickEvent(event);
	}
}

void KPageContainer::enterHeaderEditMode()
{
	// ����༭ģʽ����ʾ�����ã����ý��㣬������Ӿ���ʾ
	m_headerEdit->setReadOnly(false);
	m_headerEdit->setStyleSheet("border: 1px dashed gray; background: transparent;");
	m_headerEdit->setFocus();
}

void KPageContainer::enterFooterEditMode()
{
	m_footerEdit->setReadOnly(false);
	m_footerEdit->setStyleSheet("border: 1px dashed gray; background: transparent;");
	m_footerEdit->setFocus();
}

void KPageContainer::exitHeaderEditMode()
{
	// �˳��༭ģʽ����Ϊֻ�������Ƴ��Ӿ���ʾ
	m_headerEdit->setReadOnly(true);
	m_headerEdit->setStyleSheet("border: none; background: transparent;");
	m_footerEdit->setReadOnly(true);
	m_footerEdit->setStyleSheet("border: none; background: transparent;");
}

void KPageContainer::paintEvent(QPaintEvent* event)
{
	QWidget::paintEvent(event);

	QPainter painter(this);
	painter.setPen(QPen(Qt::darkGray, 1, Qt::SolidLine));
	QBrush brush(Qt::white);
	painter.setBrush(brush);
	QRect pageRect = m_page->geometry();

	QPoint startPoint = pageRect.topLeft() - QPoint(m_pageMargin.left(), m_pageMargin.top());
	QPoint endPoint = pageRect.bottomRight() + QPoint(m_pageMargin.right(), m_pageMargin.bottom());
	painter.drawRect(QRect(startPoint, endPoint));

	int cornerSize = 40;

	// ����
	painter.drawLine(pageRect.topLeft() + QPoint(-cornerSize, 0), pageRect.topLeft());
	painter.drawLine(pageRect.topLeft() + QPoint(0, -cornerSize), pageRect.topLeft());

	// ����
	painter.drawLine(pageRect.topRight() + QPoint(cornerSize, 0), pageRect.topRight());
	painter.drawLine(pageRect.topRight() + QPoint(0, -cornerSize), pageRect.topRight());

	// ����
	painter.drawLine(pageRect.bottomLeft() + QPoint(-cornerSize, 0), pageRect.bottomLeft());
	painter.drawLine(pageRect.bottomLeft() + QPoint(0, cornerSize), pageRect.bottomLeft());

	// ����
	painter.drawLine(pageRect.bottomRight() + QPoint(cornerSize, 0), pageRect.bottomRight());
	painter.drawLine(pageRect.bottomRight() + QPoint(0, cornerSize), pageRect.bottomRight());

	// ҳ��
	if (m_drawNumber && m_pageNumber > 0)
	{
		QRect pageNumRect(m_pageMargin.left(), height() - m_pageMargin.bottom(),
			width() - m_pageMargin.left() - m_pageMargin.right(), m_pageMargin.bottom());
		painter.drawText(pageNumRect, Qt::AlignCenter, QString::number(m_pageNumber));
	}
}

void KPageContainer::updatePageMargin(const QMargins& marginSize)
{
	m_pageMargin = marginSize;
	m_editSize.setWidth(m_paperPixelSize.width() - m_pageMargin.left() - m_pageMargin.right());
	m_editSize.setHeight(m_paperPixelSize.height() - m_pageMargin.top() - m_pageMargin.bottom());
	m_page->setFixedSize(m_editSize);
	resizeEvent(nullptr);
	update();
}

void KPageContainer::insertPageNumber(bool flag)
{
	m_drawNumber = flag;
	update();
}