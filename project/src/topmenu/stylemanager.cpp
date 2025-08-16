#include "stylemanager.h"
#include <QFont>

StyleManager::StyleManager(QObject* parent) : QObject(parent)
{
	createDefaultStyles();
}

void StyleManager::createDefaultStyles()
{
	// 正文
	TextStyle normal;
	normal.id = m_nextId++;
	normal.name = QStringLiteral("正文");
	normal.charFormat.setFont(QFont("DengXian", 11));
	normal.charFormat.setForeground(Qt::black);
	// normal.blockFormat.setLineHeight(1.5, QTextBlockFormat::ProportionalHeight);
	normal.blockFormat.setBottomMargin(0);
	normal.blockFormat.setTopMargin(0);
	m_styles[normal.id] = normal;

	// 标题 1
	TextStyle h1;
	h1.id = m_nextId++;
	h1.name = QStringLiteral("标题 1");
	h1.charFormat.setFont(QFont("DengXian Light", 16, QFont::Bold));
	h1.charFormat.setForeground(QColor("#2E74B5"));
	h1.blockFormat.setTopMargin(24);
	h1.blockFormat.setBottomMargin(12);
	m_styles[h1.id] = h1;

	// 标题 2
	TextStyle h2;
	h2.id = m_nextId++;
	h2.name = QStringLiteral("标题 2");
	h2.charFormat.setFont(QFont("DengXian Light", 13, QFont::Bold));
	h2.charFormat.setForeground(QColor("#2E74B5"));
	h2.blockFormat.setTopMargin(12);
	h2.blockFormat.setBottomMargin(6);
	m_styles[h2.id] = h2;

	TextStyle h3;
	h3.id = m_nextId++;
	h3.name = QStringLiteral("标题 3");
	h3.charFormat.setFont(QFont("DengXian Light", 11, QFont::Bold));
	h3.charFormat.setForeground(QColor("#2E74B5"));
	h3.blockFormat.setTopMargin(6);
	h3.blockFormat.setBottomMargin(3);
	m_styles[h3.id] = h3;
}

TextStyle StyleManager::getStyle(int id) const
{
	return m_styles.value(id, TextStyle()); // 返回默认构造的TextStyle如果ID不存在
}

QList<TextStyle> StyleManager::getAllStyles() const
{
	return m_styles.values();
}