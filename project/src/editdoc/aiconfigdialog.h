#ifndef AICONFIGDIALOG_H
#define AICONFIGDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPlainTextEdit>	// ∂‡––±‡º≠
#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QFont>
#include <QSize>
#include <QPushButton>
#include "aiconfig.h"

namespace Ui { class AIConfigDialog; }

class AIConfigDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AIConfigDialog(const AIConfig& currentConfig, QWidget* parent = nullptr);
	~AIConfigDialog();

signals:
	void configApplied(const AIConfig& newConfig);

private slots:
	void onModelTypeChanged(int index);
	void onAccepted();

private:
	Ui::AIConfigDialog* ui;
	AIConfig m_config;

	QLineEdit* m_configNameEdit;
	QLineEdit* m_apiUrlEdit;
	QComboBox* m_modelTypeCombo;
	QLineEdit* m_requestTemplateEdit;
	QLineEdit* m_responsePathEdit;
	QLineEdit* m_apiKeyEdit;
	QLineEdit* m_modelIdentifierEdit;
	QDialogButtonBox* m_buttonBox;

	QLabel* m_requestLabel;
	QLabel* m_responseLabel;
	QLabel* m_keyLabel;
	QLabel* m_modelIdentifyLabel;

	QString m_editStyle;
	QString m_labelStyle;
};

#endif // AICONFIGDIALOG_H