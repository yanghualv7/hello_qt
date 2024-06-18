#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class MyWidget; }
QT_END_NAMESPACE

class MyWidget : public QWidget
{
	Q_OBJECT

public:
	MyWidget(QWidget* parent = nullptr);
	~MyWidget();

private slots:
	void on_button1_clicked();
	void on_button2_clicked();
	void on_button3_clicked();
	void on_button4_clicked();

private:
	Ui::MyWidget* ui;
	void createAndExecuteBatFile(const QString& fileName, const QString& content, int AppNum);
};

#endif // MYWIDGET_H
