#include "kfonttoolbar.h"
#include "kcolorpicker.h"
#include <QVBoxLayout>
#include <QToolButton>
#include <QFontComboBox>
#include <QComboBox>
#include <QLabel>
#include <QMenu>
#include <QWidgetAction>
#include <QPainter>
#include <QList>
#include <QDebug>


KFontToolbar::KFontToolbar(QWidget* parent)
	: QWidget(parent)
	, m_currentColor(Qt::black)
{
	QString comboStyle = R"(
        QComboBox { border: 1px solid #D0D0D0; min-width: 6em; padding: 5px; }
        QComboBox::drop-down { border: grey; }
        QComboBox::down-arrow { image: url(:/image/icons/arrow_down.svg);
			width: 20px;
			height: 30px;
		}
		QComboBox::down-arrow:hover { border-style: flat; background: #D0D0D0; })";

	QString toolButtonStyle = R"(
		QToolButton { border-style: flat; background: transparent; }
		QToolButton:hover { border-style: flat; background: #D0D0D0; }
		QToolButton:checked { border-style: flat; background: #D0D0D0; })";

	// 第一行
	m_fontCombo = new QFontComboBox(this);
	m_fontCombo->setStyleSheet(comboStyle);
	m_fontSizeCombo = new QComboBox(this);
	// m_fontSizeCombo->setEditable(true);
	m_fontSizeCombo->setStyleSheet(comboStyle);
	QList<int> standardSizes = { 9, 10, 11, 12, 14, 16, 18, 20, 24, 28, 32, 36, 48, 72 };
	for (int size : standardSizes)
	{
		m_fontSizeCombo->addItem(QString::number(size));
	}
	m_fontSizeCombo->setCurrentIndex(2);


	QHBoxLayout* row1 = new QHBoxLayout();
	row1->addWidget(m_fontCombo);
	row1->addWidget(m_fontSizeCombo);
	row1->setAlignment(Qt::AlignLeft);
	//row1->addStretch();

	// 第二行
	m_boldButton = new QToolButton(this);
	m_boldButton->setStyleSheet(toolButtonStyle);
	m_boldButton->setIcon(QIcon(":/image/icons/bold.svg"));
	m_boldButton->setCheckable(true);
	m_italicButton = new QToolButton(this);
	m_italicButton->setStyleSheet(toolButtonStyle);
	m_italicButton->setIcon(QIcon(":/image/icons/italic.svg"));
	m_italicButton->setCheckable(true);
	m_underlineButton = new QToolButton(this);
	m_underlineButton->setStyleSheet(toolButtonStyle);
	m_underlineButton->setIcon(QIcon(":/image/icons/underline.svg"));
	m_underlineButton->setCheckable(true);

	m_colorButton = new QToolButton(this);
	m_colorButton->setStyleSheet(toolButtonStyle);
	m_colorButton->setPopupMode(QToolButton::InstantPopup);
	createColorIcon(m_currentColor);
	connect(m_colorButton, &QToolButton::clicked, this, &KFontToolbar::onColorButtonClicked);

	QHBoxLayout* row2 = new QHBoxLayout();
	row2->setSpacing(5);
	row2->addWidget(m_boldButton);
	row2->addWidget(m_italicButton);
	row2->addWidget(m_underlineButton);
	row2->addWidget(m_colorButton);
	row2->setAlignment(Qt::AlignLeft);

	// 整体布局
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(row1);
	mainLayout->addLayout(row2);
	mainLayout->addStretch();
	QLabel* label = new QLabel(tr("font"), this);
	label->setStyleSheet("QLabel { border: none; }");
	mainLayout->addWidget(label, 0, Qt::AlignHCenter);

	connect(m_boldButton, &QToolButton::toggled, this, &KFontToolbar::boldTriggered);
	
	connect(m_fontCombo, &QFontComboBox::currentFontChanged, this, [this](const QFont& font) { emit fontFamilyChanged(font.family()); });
	connect(m_fontSizeCombo, &QComboBox::currentTextChanged, this, [this](const QString& text) { emit fontSizeChanged(text.toInt()); });
	connect(m_italicButton, &QToolButton::toggled, this, &KFontToolbar::italicTriggered);
	connect(m_underlineButton, &QToolButton::toggled, this, &KFontToolbar::underlineTriggered);
}

void KFontToolbar::onColorButtonClicked()
{
	QMenu* menu = new QMenu(this);
	KColorPicker* picker = new KColorPicker(this);
	connect(picker, &KColorPicker::colorSelected, this, [this, menu](const QColor& color) {
		applyColor(color);
		menu->close();
	});

	QWidgetAction* action = new QWidgetAction(menu);
	action->setDefaultWidget(picker);
	menu->addAction(action);
	menu->exec(m_colorButton->mapToGlobal(QPoint(0, m_colorButton->height())));
}

void KFontToolbar::applyColor(const QColor& color)
{
	m_currentColor = color;
	createColorIcon(color);
	emit colorChanged(color);
}

void KFontToolbar::createColorIcon(const QColor& color)
{
	QPixmap pixmap(24, 24);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setFont(QFont("Times New Roman", 14, QFont::Bold));
	painter.drawText(pixmap.rect(), Qt::AlignCenter, "A");
	painter.fillRect(2, 19, 20, 5, color);
	m_colorButton->setIcon(QIcon(pixmap));
}

// updateFormat 函数保持不变，但需增加对颜色的更新
void KFontToolbar::updateFormat(const QTextCharFormat& format)
{
	// ... 更新 B, I, U, Font, Size ...
	// 使用blockSignals来防止在设置新值时触发信号，导致循环调用
	m_boldButton->blockSignals(true);
	m_boldButton->setChecked(format.fontWeight() == QFont::Bold);
	m_boldButton->blockSignals(false);
	
	m_italicButton->blockSignals(true);
	m_italicButton->setChecked(format.fontItalic());
	m_italicButton->blockSignals(false);
	
	m_underlineButton->blockSignals(true);
	m_underlineButton->setChecked(format.fontUnderline());
	m_underlineButton->blockSignals(false);
	
	m_fontCombo->blockSignals(true);
	m_fontCombo->setCurrentFont(format.font());
	m_fontCombo->blockSignals(false);
	
	m_fontSizeCombo->blockSignals(true);
	m_fontSizeCombo->setCurrentText(QString::number(format.fontPointSize()));
	m_fontSizeCombo->blockSignals(false);

	m_currentColor = format.foreground().color();
	createColorIcon(m_currentColor);
}