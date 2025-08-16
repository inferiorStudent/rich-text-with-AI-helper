#include "kaieditor.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTextCursor>
#include <QDebug>

KAIEditor::KAIEditor(QWidget* parent)
	: QTextEdit(parent)
	, m_serviceEnabled(false) // Ĭ�Ϲر�AI����
{
	m_aiManager = new AIManager(this);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	// ���յ��½���ʱ������UI
	connect(m_aiManager, &AIManager::suggestionReady, this, &KAIEditor::onSuggestionReceived);

	// ��AIManager��״̬�ź�ת����ȥ
	connect(m_aiManager, &AIManager::connectionStatusChanged, this, &KAIEditor::connectionStatusChanged);

	// ���ı��ı�ʱ�������½���
	connect(this, &QTextEdit::textChanged, this, &KAIEditor::requestNewSuggestion);
}

void KAIEditor::applyConfig(const AIConfig& config)
{
	m_aiManager->setConfig(config);
	m_aiManager->testConnection(); // ���������ú�������������
}

void KAIEditor::setServiceEnabled(bool enabled)
{
	m_serviceEnabled = enabled;
	if (!m_serviceEnabled)
	{
		// ����رշ���������ս��鲢�ػ�
		m_suggestionText.clear();
		viewport()->update();
	}
}

// ���ģ����������ı�
void KAIEditor::paintEvent(QPaintEvent* event)
{
	QTextEdit::paintEvent(event);

	// �������رա�û�н�����겻���ı�ĩβ���򲻻���
	if (!m_serviceEnabled || m_suggestionText.isEmpty() || textCursor().position() != toPlainText().length())
		return;

	QPainter painter(viewport());
	QTextCursor cursor = textCursor();
	QRect cursorRect = this->cursorRect(cursor);

	painter.setFont(font());
	painter.setPen(Qt::gray);

	QString clippedSuggestion = m_suggestionText.split('\n').first();
	painter.drawText(cursorRect.left(), cursorRect.bottom() - fontMetrics().descent(), clippedSuggestion);
}

// ���ģ�����Tab����ȫ
void KAIEditor::keyPressEvent(QKeyEvent* e)
{
	// �������Tab�����ҷ��������н����ı������ҹ����ĩβ
	if (m_serviceEnabled && (e->key() == Qt::Key_Tab) && !m_suggestionText.isEmpty() && textCursor().position() == toPlainText().length())
	{
		e->accept();
		this->insertPlainText(m_suggestionText);
		m_suggestionText.clear();
		return;
	}

	QTextEdit::keyPressEvent(e);
}

void KAIEditor::onSuggestionReceived(const QString& suggestion)
{
	if (m_suggestionText != suggestion)
	{
		m_suggestionText = suggestion.trimmed();
		viewport()->update();
	}
}

void KAIEditor::requestNewSuggestion()
{
	// �������رգ��򲻷�������
	if (!m_serviceEnabled)
		return;

	QString currentText = toPlainText();
	if (currentText.isEmpty())
	{
		m_suggestionText.clear();
		viewport()->update();
		return;
	}

	m_aiManager->fetchSuggestion(currentText);
}