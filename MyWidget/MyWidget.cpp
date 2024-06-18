#include "mywidget.h"
#include "ui_mywidget.h"
#include <QFile>
#include <QTextStream>
#include <QProcess>

MyWidget::MyWidget(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::MyWidget)
{
	ui->setupUi(this);
	connect(ui->button1, &QPushButton::clicked, this, &MyWidget::on_button1_clicked);
	connect(ui->button2, &QPushButton::clicked, this, &MyWidget::on_button2_clicked);
	connect(ui->button3, &QPushButton::clicked, this, &MyWidget::on_button3_clicked);
	connect(ui->button4, &QPushButton::clicked, this, &MyWidget::on_button4_clicked);
}

MyWidget::~MyWidget()
{
	delete ui;
}

void MyWidget::createAndExecuteBatFile(const QString& fileName, const QString& content, int AppNum)
{
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&file);
		out << content;
		file.close();

		// Ensure any previous instances are terminated before starting a new one
		if (AppNum == 1)
			QProcess::execute("cmd.exe", QStringList() << "/c" << "taskkill /f /im tcp_server.exe");
		else if (AppNum == 2)
			QProcess::execute("cmd.exe", QStringList() << "/c" << "taskkill /f /im hello.exe");
		else if (AppNum == 3)
			QProcess::execute("cmd.exe", QStringList() << "/c" << "taskkill /f /im snake.exe");
		else if (AppNum == 4)
			QProcess::execute("cmd.exe", QStringList() << "/c" << "taskkill /f /im create_license.exe");

		QProcess::startDetached("cmd.exe", QStringList() << "/c" << fileName);
	}
}

void MyWidget::on_button1_clicked()
{
	createAndExecuteBatFile("start.bat", "cd tcp_server && start tcp_server.exe", 1);
}

void MyWidget::on_button2_clicked()
{
	createAndExecuteBatFile("start.bat", "cd hello && start hello.exe", 2);
}

void MyWidget::on_button3_clicked()
{
	createAndExecuteBatFile("start.bat", "cd snake && start snake.exe", 3);
}

void MyWidget::on_button4_clicked()
{
	createAndExecuteBatFile("start.bat", "cd create_license && start create_license.exe", 4);
}
