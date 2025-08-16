#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QMap>
#include "ktab.h"
#include "ktopmenu.h"

// 前置声明以减少头文件依赖
class KTextDoc;
class ClipboardSidebar;
class KStylesFloatingWindow;
class StyleManager;
class QCloseEvent;

// 用于存储每个文档所有状态的结构体
struct DocumentInfo {
	KTextDoc* docWidget = nullptr;
	QString filePath;
	bool isModified = false;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

protected:
	// 重写关闭事件，以处理未保存的文档
	void closeEvent(QCloseEvent* event) override;

private slots:
	// --- 文件菜单槽 ---
	void onFileNew();
	void onFileOpen();
	bool onFileSave();
	bool onFileSaveAs();

	// --- 文档与标签联动槽 ---
	void onTabChanged(int id);
	void onDocumentTabClosed(int id);
	void onDocumentModified(int tabId, bool modified);

	// --- 其他UI槽 ---
	void onClipboardExpandRequested();
	void onStylePanelRequested(const QPoint& pos);
	void applyStyleToCurrentDoc(int styleId);

private:
	// --- 辅助函数 ---
	void setupMenuBar();
	KTextDoc* activeTextDoc() const;
	int activeTabId() const;
	void updateTitles();
	bool maybeSave(int tabId); // 关闭前检查是否需要保存
	void createNewDocument(const QString& filePath = QString()); // 工厂函数

	void reConnect(KTextDoc* textDoc);

	Ui::MainWindow* ui;

	// --- UI组件 ---
	KTab* m_fileTabs;
	KTopMenu* m_ribbonMenu;
	QStackedWidget* m_ribbonPanels;
	QStackedWidget* m_documentStack; // 用于存放多个KTextDoc
	ClipboardSidebar* m_clipboardSidebar;
	KStylesFloatingWindow* m_stylesWindow = nullptr;
	StyleManager* m_styleManager;

	// --- 核心数据结构 ---
	QMap<int, DocumentInfo> m_documents;
	int m_untitledCount = 1;
};

#endif // MAINWINDOW_H