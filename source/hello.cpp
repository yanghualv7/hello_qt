#include "hello.h"
#include "ui_hello.h"
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QTextCodec>

hello::hello(QWidget* parent)
	: QMainWindow(parent),
	ui(new Ui::helloClass)
{
	ui->setupUi(this);
	textBrowser = new QTextBrowser(this);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(textBrowser);
	ui->centralWidget->setLayout(layout);

	// 从ini文件中获取文件路径和图片路径
	QString iniFilePath = "config/cfg.ini";
	QString filePath = getFilePathFromIni(iniFilePath, "FilePath");
	QString imagePath = getFilePathFromIni(iniFilePath, "ImagePath");

	loadFile(filePath, imagePath, iniFilePath);
}

hello::~hello()
{
	delete ui;
}

void hello::loadFile(const QString& filePath, const QString& imagePath, const QString& iniFilePath)
{
	QFile file(filePath);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream in(&file);
		// 设置文本流编码为UTF-8
		in.setCodec("UTF-8");
		QString fileContent = in.readAll();
		// 转换文本中的空格、制表符和换行符
		QString htmlContent = QString("<img src='%1' /><br><br>").arg(imagePath) +
			fileContent.toHtmlEscaped()
			.replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;") // 将制表符转换为4个空格
			.replace(" ", "&nbsp;")                   // 将单个空格转换为HTML空格实体
			.replace("\n", "<br>");                   // 将换行符转换为HTML换行
		textBrowser->setHtml(htmlContent);

		file.close();
	}
	else
	{
		QString errorMessage = QString("<p>Failed to load file.</p>"
			"<p>File Path: %1</p>"
			"<p>Image Path: %2</p>"
			"<p>Config Path: %3</p>")
			.arg(filePath)
			.arg(imagePath)
			.arg(iniFilePath);
		textBrowser->setHtml(errorMessage);
	}
}

QString hello::getFilePathFromIni(const QString& iniFilePath, const QString& key)
{
	QSettings settings(iniFilePath, QSettings::IniFormat);

	// 设置 ini 文件读取的编码为 UTF-8
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
	QString value = settings.value(key).toString();
	return value;
}
