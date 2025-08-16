#include "aiconfigdialog.h"
#include "ui_aiconfigdialog.h"

AIConfigDialog::AIConfigDialog(const AIConfig& currentConfig, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::AIConfigDialog)
	, m_config(currentConfig)
	, m_configNameEdit(new QLineEdit(this))
	, m_apiUrlEdit(new QLineEdit(this))
	, m_modelTypeCombo(new QComboBox(this))
	, m_requestTemplateEdit(new QLineEdit(this))
	, m_responsePathEdit(new QLineEdit(this))
	, m_apiKeyEdit(new QLineEdit(this))
	, m_modelIdentifierEdit(new QLineEdit(this))
{
	ui->setupUi(this);
	setWindowTitle(tr("AI configure"));

	m_editStyle = R"(
		QLineEdit {
			font-size: 10pt;
			min-height: 20;
			border: 1px solid #c0c0c0; /* 默认边框为浅灰色 */
			background-color: white;   /* 默认背景为白色 */
			border-radius: 3px;
			padding: 2px;
		}

		/* 当 QLineEdit 获得焦点时的样式 */
		QLineEdit:focus {
			background-color: #e0f2ff; /* 淡蓝色背景 */
			border: 1px solid #b3e0ff; /* 淡蓝色边框 */
		})";

	m_labelStyle = R"(
		QLabel {
			font-family: "Microsoft YaHei";
			font-size: 10pt;
			min-height: 20;
			border: none;
			padding: 2px;
		})";

	// 添加模型类型选项
	m_modelTypeCombo->addItem(tr("self-define"), static_cast<int>(ModelType::Custom));
	m_modelTypeCombo->addItem(tr("Bloomz-560m (example)"), static_cast<int>(ModelType::Predefined_Bloom));
	m_modelTypeCombo->addItem(tr("general (OpenAI API)"), static_cast<int>(ModelType::OpenAI_Compatible));
	m_modelTypeCombo->setCurrentIndex(static_cast<int>(m_config.type));
	m_modelTypeCombo->setStyleSheet(R"(
		QComboBox {
			min-height: 30;
			font-size: 10pt;
			border: 1px solid #c0c0c0;
			border-radius: 3px;
		}
		QComboBox:hover { border-color: #6a6a6a; }
		QComboBox:focus { border-color: #0078d7; background: #f0f0f0; }
		QComboBox::drop-down {
			subcontrol-origin: padding;
			subcontrol-position: top right;
			width: 20px;

			border-left-width: 1px;
			border-left-color: #c0c0c0;
			border-left-style: solid; /* 绘制一个分隔线 */
			border-top-right-radius: 3px;
			border-bottom-right-radius: 3px;
		}
		QComboBox::drop-down:hover { background-color: #e0e0e0; }
		QComboBox::down-arrow {
			width: 20px; height: 20px;
			image: url(:/image/icons/arrow_down.svg); })");

	// 根据当前配置初始化UI
	m_configNameEdit->setText(m_config.configName);
	m_configNameEdit->setStyleSheet(m_editStyle);

	m_apiUrlEdit->setText(m_config.apiUrl.toString());
	m_apiUrlEdit->setStyleSheet(m_editStyle);

	m_requestTemplateEdit->setText(m_config.requestTemplate);
	m_requestTemplateEdit->setStyleSheet(m_editStyle);

	m_responsePathEdit->setText(m_config.responsePath);
	m_responsePathEdit->setStyleSheet(m_editStyle);

	m_apiKeyEdit->setText(m_config.apiKey);
	m_apiKeyEdit->setStyleSheet(m_editStyle);

	m_modelIdentifierEdit->setText(m_config.modelIdentifier);
	m_modelIdentifierEdit->setStyleSheet(m_editStyle);

	// 底部按钮
	m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
	QPushButton* okButton = m_buttonBox->button(QDialogButtonBox::Ok);
	QPushButton* cancelButton = m_buttonBox->button(QDialogButtonBox::Cancel);
	QString buttonStyle = R"(
		QPushButton {
			font-family: "Segoe UI", "Microsoft YaHei", sans-serif;
			font-size: 10pt;
			background-color: white;
			border: 1px solid grey;
			padding: 5px;
		}
		QPushButton:hover {
			background-color: #f1f1f1; /* 浅灰色背景 */
			border: 1px solid #c8c8c8; /* 浅灰色边框 */
		})";
	if (okButton)
	{
		okButton->setText(tr("save"));
		okButton->setStyleSheet(buttonStyle);
	}
	if (cancelButton)
	{
		cancelButton->setText(tr("cancel"));
		cancelButton->setStyleSheet(buttonStyle);
	}

	connect(m_modelTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AIConfigDialog::onModelTypeChanged);
	connect(m_buttonBox, &QDialogButtonBox::accepted, this, &AIConfigDialog::onAccepted);
	connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	QFormLayout* formLayout = new QFormLayout();
	m_requestLabel = new QLabel(tr("JSON request format"));
	m_requestLabel->setStyleSheet(m_labelStyle);
	m_responseLabel = new QLabel(tr("JSON response path"));
	m_responseLabel->setStyleSheet(m_labelStyle);

	QLabel* conf_name_label = new QLabel(tr("configure name"));
	conf_name_label->setStyleSheet(m_labelStyle);
	QLabel* urlLabel = new QLabel(tr("llm url"));
	urlLabel->setStyleSheet(m_labelStyle);
	QLabel* selectLabel = new QLabel(tr("select llm"));
	selectLabel->setStyleSheet(m_labelStyle);
	m_keyLabel = new QLabel(tr("api key"));
	m_keyLabel->setStyleSheet(m_labelStyle);
	m_modelIdentifyLabel = new QLabel(tr("model"));
	m_modelIdentifyLabel->setStyleSheet(m_labelStyle);

	formLayout->addRow(selectLabel, m_modelTypeCombo);
	formLayout->addRow(conf_name_label, m_configNameEdit);
	formLayout->addRow(m_modelIdentifyLabel, m_modelIdentifierEdit);
	formLayout->addRow(urlLabel, m_apiUrlEdit);
	formLayout->addRow(m_keyLabel, m_apiKeyEdit);
	formLayout->addRow(m_requestLabel, m_requestTemplateEdit);
	formLayout->addRow(m_responseLabel, m_responsePathEdit);

	formLayout->setLabelAlignment(Qt::AlignRight);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(formLayout);
	mainLayout->addWidget(m_buttonBox);

	// setLayout(mainLayout);

	onModelTypeChanged(m_modelTypeCombo->currentIndex()); // 初始化控件可见性
}

AIConfigDialog::~AIConfigDialog()
{
	delete ui;
}

void AIConfigDialog::onModelTypeChanged(int index)
{
	ModelType type = static_cast<ModelType>(m_modelTypeCombo->itemData(index).toInt());
	bool isCustom = (type == ModelType::Custom);
	bool isOpenAI = (type == ModelType::OpenAI_Compatible);

	// 只有自定义类型才显示JSON格式编辑框
	m_requestTemplateEdit->setVisible(isCustom);
	m_responsePathEdit->setVisible(isCustom);
	// ... 也需要显示对应的QLabel
	m_responseLabel->setVisible(isCustom);
	m_requestLabel->setVisible(isCustom);

	// 通用大模型
	m_apiKeyEdit->setVisible(isOpenAI);
	m_modelIdentifierEdit->setVisible(isOpenAI);
	m_keyLabel->setVisible(isOpenAI);
	m_modelIdentifyLabel->setVisible(isOpenAI);
}

void AIConfigDialog::onAccepted()
{
	// 从UI收集新配置
	m_config.configName = m_configNameEdit->text();
	m_config.apiUrl = QUrl(m_apiUrlEdit->text());
	m_config.type = static_cast<ModelType>(m_modelTypeCombo->currentData().toInt());

	if (m_config.type == ModelType::OpenAI_Compatible)
	{
		m_config.apiKey = m_apiKeyEdit->text();
		m_config.modelIdentifier = m_modelIdentifierEdit->text();
	}
	else if (m_config.type == ModelType::Custom)
	{
		m_config.requestTemplate = m_requestTemplateEdit->text();
		m_config.responsePath = m_responsePathEdit->text();
	}
	else if (m_config.type == ModelType::Predefined_Bloom)
	{
		// 为预定义模型硬编码请求/响应格式
		m_config.requestTemplate = "{\"text\": \"%1\"}";
		m_config.responsePath = "suggestion";
	}

	emit configApplied(m_config);
	accept();
}