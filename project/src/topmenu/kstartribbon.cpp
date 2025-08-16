#include "kstartribbon.h"
#include "kclipboardtoolbar.h"
#include "kfonttoolbar.h"
#include "ktextdoc.h"
#include "kparagraphtoolbar.h"
#include "kstylestoolbar.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <memory>

KStartRibbon::KStartRibbon(KTextDoc* textDoc, StyleManager* styleManager, QWidget* parent)
	: QWidget(parent)
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	// 1. 创建并添加子工具栏
	m_clipboardToolbar = new KClipboardToolbar(this);
	m_fontToolbar = new KFontToolbar(this);
	m_paragraphToolbar = new KParagraphToolbar(this);
	m_stylesToolbar = new KStylesToolbar(styleManager, this);

	layout->addWidget(m_clipboardToolbar);
	QFrame* separator1 = getSeparateLine();
	layout->addWidget(separator1);

	layout->addWidget(m_fontToolbar);
	QFrame* separator2 = getSeparateLine();
	layout->addWidget(separator2);

	layout->addWidget(m_paragraphToolbar);
	QFrame * separator3 = getSeparateLine();
	layout->addWidget(separator3);

	layout->addWidget(m_stylesToolbar);
	QFrame* separator4 = getSeparateLine();
	layout->addWidget(separator4);

	layout->addStretch();

	// 字体样式
	connect(m_fontToolbar, &KFontToolbar::boldTriggered, textDoc, &KTextDoc::setTextBold);
	connect(m_fontToolbar, &KFontToolbar::italicTriggered, textDoc, &KTextDoc::setTextItalic);
	connect(m_fontToolbar, &KFontToolbar::underlineTriggered, textDoc, &KTextDoc::setTextUnderline);
	connect(m_fontToolbar, &KFontToolbar::fontFamilyChanged, textDoc, &KTextDoc::setTextFontFamily);
	connect(m_fontToolbar, &KFontToolbar::fontSizeChanged, textDoc, &KTextDoc::setTextFontSize);
	connect(m_fontToolbar, &KFontToolbar::colorChanged, textDoc, &KTextDoc::setTextColor);

	// 剪贴板
	connect(m_clipboardToolbar->pasteButton(), &QToolButton::clicked, textDoc, &KTextDoc::paste);
	connect(m_clipboardToolbar, &KClipboardToolbar::expandRequested, this, &KStartRibbon::clipboardExpandRequested);

	// 段落
	connect(m_paragraphToolbar, &KParagraphToolbar::alignmentChanged, textDoc, &KTextDoc::setParagraphAlignment);
	connect(m_paragraphToolbar, &KParagraphToolbar::listStyleChanged, textDoc, &KTextDoc::setParagraphListStyle);
	connect(m_paragraphToolbar, &KParagraphToolbar::lineHeightChanged, textDoc, &KTextDoc::setParagraphLineHeight);

	// 样式
	connect(m_stylesToolbar, &KStylesToolbar::stylePanelRequested, this, &KStartRibbon::stylePanelRequested);
	connect(m_stylesToolbar, &KStylesToolbar::styleApplied, this, &KStartRibbon::styleApplied);

	// connect(textDoc, &KTextDoc::selectionFormatChanged, m_fontToolbar, &KFontToolbar::updateFormat);
	connect(textDoc, &KTextDoc::selectionFormatChanged, this, [this](const QTextCharFormat& charFmt, const QTextBlockFormat& blockFmt, const QTextListFormat& listFmt) {
		m_fontToolbar->updateFormat(charFmt);
		m_paragraphToolbar->updateFormat(blockFmt, listFmt);
		});
}

QFrame* KStartRibbon::getSeparateLine()
{
	QFrame* separator = new QFrame(this);
	separator->setFrameShape(QFrame::VLine);
	separator->setFrameShadow(QFrame::Plain);
	separator->setStyleSheet(R"(QFrame { border: none; border-right: 1px solid #C0C0C0; })");
	return separator;
}