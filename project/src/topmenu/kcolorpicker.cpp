#include "kcolorpicker.h"
#include <QGridLayout>
#include <QToolButton>
#include <QIcon>
#include <QPixmap>

KColorPicker::KColorPicker(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *grid = new QGridLayout(this);
    grid->setSpacing(2);
    // 主题颜色和标准颜色
    QList<QColor> colors = {
        // 标准色
        QColor("#C00000"), QColor("#FF0000"), QColor("#FFC000"), QColor("#FFFF00"),
        QColor("#92D050"), QColor("#00B050"), QColor("#00B0F0"), QColor("#0070C0"),
        QColor("#002060"), QColor("#7030A0"),
        // 主题颜色（部分示例）
        Qt::white, Qt::black, Qt::gray, QColor("#44546A"), QColor("#4472C4"),
        QColor("#ED7D31"), QColor("#A5A5A5"), QColor("#FFC000"), QColor("#5B9BD5"), QColor("#70AD47")
    };
    int col = 0;
    for (const QColor &color : colors)
    {
        QToolButton *btn = new QToolButton(this);
        btn->setFixedSize(20, 20);
        QPixmap pixmap(16, 16);
        pixmap.fill(color);
        btn->setIcon(QIcon(pixmap));
        // 点击按钮后，只发射信号，不再尝试关闭父级
        connect(btn, &QToolButton::clicked, this, [this, color]() {
            emit colorSelected(color);
        });
        grid->addWidget(btn, col / 10, col % 10);
        col++;
    }
}