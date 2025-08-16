#ifndef AIMANAGER_H
#define AIMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QString>
#include "aiconfig.h"

class AIManager : public QObject
{
	Q_OBJECT

public:
	explicit AIManager(QObject* parent = nullptr);
	void fetchSuggestion(const QString& currentText);
	void testConnection();
	void setConfig(const AIConfig & config);

signals:
	void suggestionReady(const QString& suggestion);
	void connectionStatusChanged(bool success);

private slots:
	void onReplyFinished(QNetworkReply* reply);

private:
	QNetworkAccessManager* m_networkManager;
	AIConfig m_config;
	bool m_isTestingConnection = false; // 标志位，用于区分测试连接和正常请求
};

#endif // AIMANAGER_H