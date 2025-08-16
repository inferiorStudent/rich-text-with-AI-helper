#ifndef AICONFIG_H
#define AICONFIG_H

#include <QString>
#include <QUrl>
#include <QObject>

// 定义AI模型的类型
enum class ModelType
{
	Predefined_Bloom, // 预定义的模型示例
	OpenAI_Compatible,
	Custom            // 用户自定义接口
};

struct AIConfig
{
	QString configName = QObject::tr("un-named");
	ModelType type = ModelType::Custom;
	QUrl apiUrl;
	QString apiKey;
	QString modelIdentifier;

	// 仅在 type == Custom 时使用
	// %1 将被替换为用户输入的文本
	QString requestTemplate = "{\"text\": \"%1\"}";
	// 用于从响应JSON中提取建议的路径，用'.'分隔
	QString responsePath = "suggestion";
};

#endif // AICONFIG_H