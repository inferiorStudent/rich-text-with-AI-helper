#ifndef KAIEDITOR_H
#define KAIEDITOR_H

#include <QTextEdit>
#include "aimanager.h"

class KAIEditor : public QTextEdit
{
	Q_OBJECT

public:
	explicit KAIEditor(QWidget* parent = nullptr);

public slots:
	// 用于从UI控制AI服务的开关
	void setServiceEnabled(bool enabled);
	void applyConfig(const AIConfig& config);

signals:
	void connectionStatusChanged(bool success);

protected:
	void paintEvent(QPaintEvent* event) override;
	void keyPressEvent(QKeyEvent* e) override;

private slots:
	void onSuggestionReceived(const QString& suggestion);
	void requestNewSuggestion();

private:
	AIManager* m_aiManager;
	QString m_suggestionText;
	bool m_serviceEnabled; // AI服务开关状态
};

#endif // KAIEDITOR_H