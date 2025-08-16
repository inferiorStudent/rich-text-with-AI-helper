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
	// ���ڴ�UI����AI����Ŀ���
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
	bool m_serviceEnabled; // AI���񿪹�״̬
};

#endif // KAIEDITOR_H