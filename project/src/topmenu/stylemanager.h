#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#ifdef STYLEMANAGER_EXPORTS
    #define STYLEMANAGER_API __declspec(dllexport)
#else
    #define STYLEMANAGER_API __declspec(dllimport)
#endif

#include <QObject>
#include <QMap>
#include "textstyle.h"

class STYLEMANAGER_API StyleManager : public QObject
{
	Q_OBJECT
public:
	explicit StyleManager(QObject* parent = nullptr);

	TextStyle getStyle(int id) const;
	QList<TextStyle> getAllStyles() const;

private:
	void createDefaultStyles();
	QMap<int, TextStyle> m_styles;
	int m_nextId = 0;
};

#endif // STYLEMANAGER_H