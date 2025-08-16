#include "kstylestoolbar.h"
#include "kstylebutton.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QScrollArea>
#include <QButtonGroup>

KStylesToolbar::KStylesToolbar(StyleManager* styleManager, QWidget* parent)
	: QWidget(parent)
{
	// --- 样式预览滚动区 ---
	QScrollArea * scrollArea = new QScrollArea(this);
	scrollArea->setWidgetResizable(true);
	scrollArea->setFrameShape(QFrame::NoFrame);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->setStyleSheet("QScrollArea { border: 1px solid grey; }");
	// scrollArea->setFixedWidth(400);
	
	QWidget * galleryWidget = new QWidget(scrollArea);
	QHBoxLayout * galleryLayout = new QHBoxLayout(galleryWidget);
	QButtonGroup * styleGroup = new QButtonGroup(this);
	for (const auto& style : styleManager->getAllStyles())
	{
		KStyleButton * button = new KStyleButton(style, this);
		connect(button, &KStyleButton::clicked, this, [this, id = style.id](){
			emit styleApplied(id);
		});
		galleryLayout->addWidget(button);
		styleGroup->addButton(button);
	}
	scrollArea->setWidget(galleryWidget);

	QToolButton* expandButton = new QToolButton(this);
	expandButton->setIcon(QIcon(":/image/icons/expand-bottom-right.svg"));
	expandButton->setStyleSheet(R"(QToolButton { border: none; width: 15px; height: 15px; }
		QToolButton:hover { border-style: flat; background: #D0D0D0; })");
	connect(expandButton, &QToolButton::clicked, this, [this, expandButton]() {
		QPoint globalPos = expandButton->mapToGlobal(expandButton->rect().bottomLeft());
		emit stylePanelRequested(globalPos);
	});


	QHBoxLayout* bottomLayout = new QHBoxLayout();
	QLabel* label = new QLabel(tr("style"), this);
	label->setStyleSheet("QLabel { border: none; }");
	bottomLayout->addWidget(label);
	bottomLayout->addWidget(expandButton);
	bottomLayout->setAlignment(Qt::AlignCenter);

	// --- 整体布局 ---
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(scrollArea, 1);
	mainLayout->addStretch();
	mainLayout->addLayout(bottomLayout);;
}