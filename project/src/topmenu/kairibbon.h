#ifndef KAIRIBBON_H
#define KAIRIBBON_H

#ifdef KAIRIBBON_EXPORTS
    #define KAIRIBBON_API __declspec(dllexport)
#else
    #define KAIRIBBON_API __declspec(dllimport)
#endif

#include <QWidget>
#include <QFrame>
#include <QToolButton>
#include "aiconfig.h"

class KAIEditor;
class AIConfigDialog;
class QLabel;

class KAIRIBBON_API KAIRibbon : public QWidget
{
	Q_OBJECT
public:
	explicit KAIRibbon(KAIEditor* editor, QWidget* parent = nullptr);

public slots:
	void updateConnectionStatus(bool success);

signals:
	void configChanged(const AIConfig& newConfig);

private slots:
	void openConfigDialog();

private:
	QFrame* getSeparateLine();

	KAIEditor* m_editor;
	AIConfigDialog* m_configDialog = nullptr;
	AIConfig m_currentConfig;

	QLabel* m_statusIconLabel;
	QLabel* m_statusTextLabel;

	QToolButton* m_isOpen;
};

#endif // KAIRIBBON_H