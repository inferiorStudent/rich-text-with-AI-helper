#ifndef KTOPMENU_H
#define KTOPMENU_H

#ifdef KTOPMENU_EXPORTS
    #define KTOPMENU_API __declspec(dllexport)
#else
    #define KTOPMENU_API __declspec(dllimport)
#endif

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QHBoxLayout>

class KTOPMENU_API KTopMenu : public QWidget
{
	Q_OBJECT
public:
	explicit KTopMenu(QWidget* parent = nullptr);
	void addMenu(const QString& title);
	void addMenu(const QIcon& icon);

signals:
	void currentChanged(int index);

private slots:
	void onMenuButtonClicked(int id);

private:
	void updateButtonStyles(int activeId);

	QHBoxLayout* m_layout;
	QButtonGroup* m_buttonGroup;
	QString m_activeStyle;
	QString m_inactiveStyle;
};

#endif // KTOPMENU_H