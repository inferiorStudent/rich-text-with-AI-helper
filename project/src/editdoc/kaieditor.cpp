#include "kaieditor.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTextCursor>
#include <QDebug>

KAIEditor::KAIEditor(QWidget* parent)
	: QTextEdit(parent)
	, m_serviceEnabled(false) // 默认关闭AI服务
{
	m_aiManager = new AIManager(this);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	// 当收到新建议时，更新UI
	connect(m_aiManager, &AIManager::suggestionReady, this, &KAIEditor::onSuggestionReceived);

	// 将AIManager的状态信号转发出去
	connect(m_aiManager, &AIManager::connectionStatusChanged, this, &KAIEditor::connectionStatusChanged);

	// 当文本改变时，请求新建议
	connect(this, &QTextEdit::textChanged, this, &KAIEditor::requestNewSuggestion);
}

void KAIEditor::applyConfig(const AIConfig& config)
{
	m_aiManager->setConfig(config);
	m_aiManager->testConnection(); // 设置新配置后立即测试连接
}

void KAIEditor::setServiceEnabled(bool enabled)
{
	m_serviceEnabled = enabled;
	if (!m_serviceEnabled)
	{
		// 如果关闭服务，立即清空建议并重绘
		m_suggestionText.clear();
		viewport()->update();
	}
}

// 核心：绘制幽灵文本
void KAIEditor::paintEvent(QPaintEvent* event)
{
	QTextEdit::paintEvent(event);

	// 如果服务关闭、没有建议或光标不在文本末尾，则不绘制
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

// 核心：处理Tab键补全
void KAIEditor::keyPressEvent(QKeyEvent* e)
{
	// 如果按下Tab键，且服务开启、有建议文本，并且光标在末尾
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
	// 如果服务关闭，则不发送请求
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