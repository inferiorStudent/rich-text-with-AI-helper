#include "aimanager.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QJsonValue>
#include <QJsonArray>

AIManager::AIManager(QObject* parent)
	: QObject(parent)
{
	m_networkManager = new QNetworkAccessManager(this);
	connect(m_networkManager, &QNetworkAccessManager::finished, this, &AIManager::onReplyFinished);
}

void AIManager::fetchSuggestion(const QString& currentText)
{
	// API��������ַ
	if (!m_config.apiUrl.isValid())
	{
		emit connectionStatusChanged(false);
		return;
	}

	QNetworkRequest request;
	QByteArray data;

	if (m_config.type == ModelType::OpenAI_Compatible)
	{
		// ΪOpenAI����ģ�͹�������
		QUrl url = m_config.apiUrl;
		// ͨ��API·���� /v1/chat/completions
		if (url.path().isEmpty()) url.setPath("/v1/chat/completions");
		request.setUrl(url);

		// ������֤ͷ
		request.setRawHeader("Authorization", ("Bearer " + m_config.apiKey).toUtf8());
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

		// ����JSON��
		QJsonObject rootObj;
		rootObj["model"] = m_config.modelIdentifier;
		QJsonArray messagesArray;
		QJsonObject systemMessage;
		systemMessage["role"] = "system";
		systemMessage["content"] = "You are a helpful assistant that completes user's text.";
		QJsonObject userMessage;
		userMessage["role"] = "user";
		userMessage["content"] = currentText;
		messagesArray.append(systemMessage);
		messagesArray.append(userMessage);
		rootObj["messages"] = messagesArray;
		rootObj["stream"] = false; // ����ʽ��Ӧ

		QJsonDocument doc(rootObj);
		data = doc.toJson();

	}
	else // �Զ������������
	{
		request.setUrl(m_config.apiUrl);
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
		QString requestBodyStr = m_config.requestTemplate.arg(currentText);
		data = requestBodyStr.toUtf8();
	}

	m_networkManager->post(request, data);
}

void AIManager::onReplyFinished(QNetworkReply* reply)
{
	bool success = (reply->error() == QNetworkReply::NoError);
	QString suggestion = "";

	if (reply->error() == QNetworkReply::NoError)
	{
		emit connectionStatusChanged(true);
		QByteArray responseData = reply->readAll();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
		QJsonObject jsonObj = jsonDoc.object();
		
		if (m_config.type == ModelType::OpenAI_Compatible)
		{
			// ����OpenAI��ʽ����Ӧ
			if (jsonObj.contains("choices") && jsonObj["choices"].isArray())
			{
				QJsonArray choices = jsonObj["choices"].toArray();
				if (!choices.isEmpty() && choices.first().isObject())
				{
					QJsonObject firstChoice = choices.first().toObject();
					if (firstChoice.contains("message") && firstChoice["message"].isObject())
					{
						QJsonObject message = firstChoice["message"].toObject();
						if (message.contains("content") && message["content"].isString())
						{
							suggestion = message["content"].toString();
						}
					}
				}
			}
		}
		else // �Զ������������
		{
			// ����responsePath����JSON
			QStringList pathKeys = m_config.responsePath.split('.');
			QJsonValue val = jsonObj;
			for (const QString& key : pathKeys)
			{
				if (val.isObject())
				{
					val = val.toObject().value(key);
				}
				else
				{
					val = QJsonValue(); // ·����Ч
					break;
				}
			}
			if (val.isString())
				suggestion = val.toString();
			else
				success = false; // �ɹ����ӵ��޷�����
		}
	}
	else
	{
		qWarning("Network Error: %s", qPrintable(reply->errorString()));
		emit connectionStatusChanged(false);
	}

	if (m_isTestingConnection)
	{
		emit connectionStatusChanged(success);
		m_isTestingConnection = false;
	}
	else
	{
		emit suggestionReady(suggestion);
	}

	reply->deleteLater(); // ����reply����
}

void AIManager::setConfig(const AIConfig& config)
{
	m_config = config;
}

void AIManager::testConnection()
{
	if (!m_config.apiUrl.isValid())
	{
		emit connectionStatusChanged(false);
		return;
	}
	m_isTestingConnection = true;
	// ʹ��һ���򵥵��ı�������
	fetchSuggestion("hello");
}