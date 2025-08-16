#ifndef TEXTSTYLE_H
#define TEXTSTYLE_H

#include <QString>
#include <QTextCharFormat>
#include <QTextBlockFormat>

struct TextStyle
{
	int id;
	QString name;
	QTextCharFormat charFormat;
	QTextBlockFormat blockFormat;
};

#endif // TEXTSTYLE_H