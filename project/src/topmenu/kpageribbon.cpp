#include "kpageribbon.h"
#include <QHBoxLayout>
#include <QMenu>
#include <QGuiApplication>
#include <QScreen>

KPageRibbon::KPageRibbon(QWidget* parent)
	: QWidget(parent)
{
	m_buttonStyle = R"(
		QToolButton {
			border: none;
			background-color: transparent;
			padding: 5px;
			height: 90px;
			toolbutton-style: vertical;		/* 设置图标在文字上方 */
			spacing: 4px;					/* 设置图标和文字之间的间距 */
			font-size: 18px;
			font-family: "Microsoft YaHei";
		}
		QToolButton:hover { background-color: #D0D0D0; }
		QToolButton:pressed { background-color: #d0d0d0; border-style: inset; }
		QToolButton::menu-indicator {		/* 调整下拉箭头的位置和大小 */
			width: 16px;
			height: 16px;
			subcontrol-position: bottom;	/* 放置在底部 */
			subcontrol-origin: margin;
			image: url(:/image/icons/arrow_down.svg);  /* 使用自定义的箭头图片 */
		}
		QToolButton { icon-size: 48px 48px; })";

	m_menuStyle = R"(
		QMenu::item { background-color: transparent; color: black; }
		QMenu::item:selected { background-color: #66B2FF; color: black; })";

	QHBoxLayout* layout = new QHBoxLayout(this);

	// 插入页面
	QToolButton* insertPageButton
		= createButton(QIcon(":/image/icons/insert-page.svg"), tr("insert page"));
	insertPageButton->setStyleSheet(m_buttonStyle);
	connect(insertPageButton, &QToolButton::clicked, this, &KPageRibbon::insertPageRequested);
	layout->addWidget(insertPageButton);

	// 页边距
	QToolButton* pageMargin =
		createButton(QIcon(":/image/icons/margin.svg"), tr("margin"));
	pageMargin->setStyleSheet(m_buttonStyle);
	QMenu* marginMenu = new QMenu(this);
	marginMenu->setStyleSheet(m_menuStyle);
	qreal dpi = QGuiApplication::primaryScreen()->logicalDotsPerInch();
	auto convertMmToPx = [dpi](qreal mm) { return qRound((mm / 25.4) * dpi); };
	marginMenu->addAction(tr("normal"), [this, convertMmToPx]() {
		emit pageMarginChanged(QMargins(
			convertMmToPx(25.4), convertMmToPx(31.8),
			convertMmToPx(25.4), convertMmToPx(31.8))); });
	marginMenu->addAction(tr("narrow"), [this, convertMmToPx]() {
		emit pageMarginChanged(QMargins(
			convertMmToPx(12.7), convertMmToPx(12.7),
			convertMmToPx(12.7), convertMmToPx(12.7))); });
	marginMenu->addAction(tr("medium"), [this, convertMmToPx]() {
		emit pageMarginChanged(QMargins(
			convertMmToPx(19.1), convertMmToPx(25.4),
			convertMmToPx(19.1), convertMmToPx(25.4))); });
	marginMenu->addAction(tr("wide"), [this, convertMmToPx]() {
		emit pageMarginChanged(QMargins(
			convertMmToPx(50.8), convertMmToPx(25.4),
			convertMmToPx(50.8), convertMmToPx(25.4))); });
	marginMenu->addAction(tr("self-define"), [this]() {});
	pageMargin->setMenu(marginMenu);
	pageMargin->setPopupMode(QToolButton::InstantPopup);
	layout->addWidget(pageMargin);

	// 纸张大小
	QToolButton* pageSizeButton =
		createButton(QIcon(":/image/icons/page-size.svg"), tr("page size"));
	pageSizeButton->setStyleSheet(m_buttonStyle);
	QMenu* sizeMenu = new QMenu(this);
	sizeMenu->setStyleSheet(m_menuStyle);
	sizeMenu->addAction("A4", [this]() { emit pageSizeChanged(QPageSize(QPageSize::A4)); });
	sizeMenu->addAction("A3", [this]() { emit pageSizeChanged(QPageSize(QPageSize::A3)); });
	pageSizeButton->setMenu(sizeMenu);
	pageSizeButton->setPopupMode(QToolButton::InstantPopup);
	layout->addWidget(pageSizeButton);

	// 纸张方向
	QToolButton* pageDirection =
		createButton(QIcon(":/image/icons/page-direction.svg"), tr("page orientation"));
	pageDirection->setStyleSheet(m_buttonStyle);
	QMenu* directMenu = new QMenu(this);
	directMenu->setStyleSheet(m_menuStyle);
	directMenu->addAction(tr("landscape"), [this]() {
		emit pageOrientationChanged(QPageLayout::Orientation::Landscape); });
	directMenu->addAction(tr("portrait"), [this]() {
		emit pageOrientationChanged(QPageLayout::Orientation::Portrait); });
	pageDirection->setMenu(directMenu);
	pageDirection->setPopupMode(QToolButton::InstantPopup);
	layout->addWidget(pageDirection);

	// 页眉页脚页码
	QToolButton* pageHeader =
		createButton(QIcon(":/image/icons/header.svg"), tr("page header"));
	pageHeader->setStyleSheet(m_buttonStyle);
	QMenu* headerMenu = new QMenu(this);
	headerMenu->setStyleSheet(m_menuStyle);
	headerMenu->addAction(tr("double click"), [this]() {});
	pageHeader->setMenu(headerMenu);
	pageHeader->setPopupMode(QToolButton::InstantPopup);
	layout->addWidget(pageHeader);

	QToolButton* pageFooter =
		createButton(QIcon(":/image/icons/footer.svg"), tr("page footer"));
	pageFooter->setStyleSheet(m_buttonStyle);
	QMenu* footerMenu = new QMenu(this);
	footerMenu->addAction(tr("double click"), [this]() {});
	footerMenu->setStyleSheet(m_menuStyle);
	pageFooter->setMenu(footerMenu);
	pageFooter->setPopupMode(QToolButton::InstantPopup);
	layout->addWidget(pageFooter);

	QToolButton* pageNumber =
		createButton(QIcon(":/image/icons/page-number.svg"), tr("page number"));
	pageNumber->setStyleSheet(m_buttonStyle);
	QMenu* pagenumMenu = new QMenu(this);
	pagenumMenu->addAction(tr("insert number"), [this]() { emit insertPageNumber(true); });
	pagenumMenu->addAction(tr("delete number"), [this]() { emit insertPageNumber(false); });
	pagenumMenu->setStyleSheet(m_menuStyle);
	pageNumber->setMenu(pagenumMenu);
	pageNumber->setPopupMode(QToolButton::InstantPopup);
	layout->addWidget(pageNumber);


	//layout->addStretch();
}

QToolButton* KPageRibbon::createButton(const QIcon& icon, const QString& text)
{
	QToolButton* button = new QToolButton(this);
	button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	button->setIcon(icon);
	button->setText(text);
	button->setPopupMode(QToolButton::InstantPopup);
	button->setStyleSheet(m_buttonStyle);
	return button;
}