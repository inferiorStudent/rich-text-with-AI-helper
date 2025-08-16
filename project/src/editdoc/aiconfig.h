#ifndef AICONFIG_H
#define AICONFIG_H

#include <QString>
#include <QUrl>
#include <QObject>

// ����AIģ�͵�����
enum class ModelType
{
	Predefined_Bloom, // Ԥ�����ģ��ʾ��
	OpenAI_Compatible,
	Custom            // �û��Զ���ӿ�
};

struct AIConfig
{
	QString configName = QObject::tr("un-named");
	ModelType type = ModelType::Custom;
	QUrl apiUrl;
	QString apiKey;
	QString modelIdentifier;

	// ���� type == Custom ʱʹ��
	// %1 �����滻Ϊ�û�������ı�
	QString requestTemplate = "{\"text\": \"%1\"}";
	// ���ڴ���ӦJSON����ȡ�����·������'.'�ָ�
	QString responsePath = "suggestion";
};

#endif // AICONFIG_H