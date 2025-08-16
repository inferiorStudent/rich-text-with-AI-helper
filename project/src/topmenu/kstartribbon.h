#ifndef KSTARTRIBBON_H
#define KSTARTRIBBON_H

#ifdef KSTARTRIBBON_EXPORTS
    #define KSTARTRIBBON_API __declspec(dllexport)
#else
    #define KSTARTRIBBON_API __declspec(dllimport)
#endif


#include <QWidget>
#include <QFrame>

class KClipboardToolbar;
class KFontToolbar;
class KTextDoc;
class KStylesToolbar;
class KParagraphToolbar;
class StyleManager;

class KSTARTRIBBON_API KStartRibbon : public QWidget
{
	Q_OBJECT
public:
	explicit KStartRibbon(KTextDoc* textDoc, StyleManager* styleManager, QWidget* parent = nullptr);
	
signals:
	void clipboardExpandRequested();
	void stylePanelRequested(const QPoint& pos);
	void styleApplied(int styleId);

private:
	QFrame* getSeparateLine();

private:
	KClipboardToolbar* m_clipboardToolbar;
	KFontToolbar* m_fontToolbar;
	KParagraphToolbar* m_paragraphToolbar;
	KStylesToolbar* m_stylesToolbar;
};

#endif // KSTARTRIBBON_H