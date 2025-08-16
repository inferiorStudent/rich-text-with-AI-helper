#ifndef KTEXTDOC_H
#define KTEXTDOC_H

#include "textstyle.h"
#include <QWidget>
#include <QPageSize>
#include <QPageLayout>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QTextCharFormat>
#include <QTextBlockFormat>
#include <QTextListFormat>
#include <kpagecontainer.h>

class KPageContainer;
class KAIEditor;
class QTextEdit;

class KTextDoc : public QScrollArea
{
	Q_OBJECT
public:
	explicit KTextDoc(QWidget* parent = nullptr);
	bool saveToFile(const QString& filePath);
	bool loadFromFile(const QString& filePath);
	QString currentFilePath() const;
	void setCurrentFilePath(const QString& filePath);
	bool isModified() const;
	QTextEdit* activeTextEdit() const;
	KAIEditor* editor() const
	{
		return m_pages.first()->page();
	}

public slots:
	// 页面
	void insertNewPage();
	void applyPageSizeToAllPages(const QPageSize& size);
	void applyPageOrientationToAllPages(QPageLayout::Orientation orientation);
	void applyPageMarginToAllPages(const QMargins& marginSize);
	void applyPageNumber(bool flag);

	// 字体
	void setTextBold(bool bold);
	void setTextItalic(bool italic);
	void setTextUnderline(bool underline);
	void setTextFontFamily(const QString& family);
	void setTextFontSize(int size);
	void setTextColor(const QColor& color);

	void setParagraphAlignment(Qt::Alignment alignment);
	void setParagraphListStyle(QTextListFormat::Style style);
	void setParagraphLineHeight(qreal factor);

	void applyStyle(const TextStyle& style);

	void paste();

	void insertTable(int rows, int cols);
	void insertImage(const QString & path);
	void insertSymbol(const QString & symbol);

signals:
	// 当光标位置改变，格式有变化时，发射此信号
	// void selectionFormatChanged(const QTextCharFormat& format);
	void selectionFormatChanged(const QTextCharFormat& charFormat,
	const QTextBlockFormat& blockFormat, const QTextListFormat& listFormat);

	void modificationChanged(bool modified);

private slots:
	// 内部槽，响应QTextEdit的光标变化
	void onCursorPositionChanged();
	void onPageModificationChanged(bool m);

protected:
	bool eventFilter(QObject* watched, QEvent* event) override;

private:
	void mergeFormatOnSelection(const QTextCharFormat& format);
	void updatePageNumbers();
	
	QWidget* m_documentContainer;
	QVBoxLayout* m_pageLayout;
	QList<KPageContainer*> m_pages;

	QTextEdit* m_lastActiveTextEdit = nullptr; // 记录最后活动的编辑器
	QString m_filePath;

	// 保存当前文档的页面设置
	QPageSize m_currentPageSize;
	QPageLayout::Orientation m_currentOrientation;
};

#endif // KTEXTDOC_H