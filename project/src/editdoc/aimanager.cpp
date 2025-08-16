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
	// API服务器地址
	if (!m_config.apiUrl.isValid())
	{
		emit connectionStatusChanged(false);
		return;
	}

	QNetworkRequest request;
	QByteArray data;

	if (m_config.type == ModelType::OpenAI_Compatible)
	{
		// 为OpenAI兼容模型构建请求
		QUrl url = m_config.apiUrl;
		// 通常API路径是 /v1/chat/completions
		if (url.path().isEmpty()) url.setPath("/v1/chat/completions");
		request.setUrl(url);

		// 设置认证头
		request.setRawHeader("Authorization", ("Bearer " + m_config.apiKey).toUtf8());
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

		// 构建JSON体
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
		rootObj["stream"] = false; // 非流式响应

		QJsonDocument doc(rootObj);
		data = doc.toJson();

	}
	else // 自定义或其他类型
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
			// 解析OpenAI格式的响应
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
		else // 自定义或其他类型
		{
			// 根据responsePath解析JSON
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
					val = QJsonValue(); // 路径无效
					break;
				}
			}
			if (val.isString())
				suggestion = val.toString();
			else
				success = false; // 成功连接但无法解析
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

	reply->deleteLater(); // 清理reply对象
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
	// 使用一个简单的文本来测试
	fetchSuggestion("hello");
}