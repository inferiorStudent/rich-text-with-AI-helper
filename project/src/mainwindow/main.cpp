#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QTranslator>

QString getQmlPath()
{
	//这里为调试目录
	QString appDir = QCoreApplication::applicationDirPath();
	QDir currentDir(appDir);

	QString qmlFilePath = QDir::cleanPath(currentDir.absolutePath() + QDir::separator() +
		QString("mui") + QDir::separator() +
		QString("zh_CN") + QDir::separator() +
		QString("translate_zh.qm")
	);

	if (QFile::exists(qmlFilePath))
	{
		return qmlFilePath;
	}

	return QString();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

// 	QTranslator translator;
// 	if (translator.load(getQmlPath()))
// 	{
// 		a.installTranslator(&translator);
// 	}

	QTranslator trans_topmenu;
	if (trans_topmenu.load(":/translation/topmenu_translate_zh.qm"))
		a.installTranslator(&trans_topmenu);

	QTranslator trans_editor;
	if (trans_editor.load(":/translation/editor_translate_zh.qm"))
		a.installTranslator(&trans_editor);

	QTranslator trans_main;
	if (trans_main.load(":/translation/main_translate_zh.qm"))
		a.installTranslator(&trans_main);

    MainWindow w;
    w.show();
    return a.exec();
}
