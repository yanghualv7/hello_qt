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

	// ��ini�ļ��л�ȡ�ļ�·����ͼƬ·��
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
		// �����ı�������ΪUTF-8
		in.setCodec("UTF-8");
		QString fileContent = in.readAll();
		// ת���ı��еĿո��Ʊ���ͻ��з�
		QString htmlContent = QString("<img src='%1' /><br><br>").arg(imagePath) +
			fileContent.toHtmlEscaped()
			.replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;") // ���Ʊ��ת��Ϊ4���ո�
			.replace(" ", "&nbsp;")                   // �������ո�ת��ΪHTML�ո�ʵ��
			.replace("\n", "<br>");                   // �����з�ת��ΪHTML����
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

	// ���� ini �ļ���ȡ�ı���Ϊ UTF-8
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
	QString value = settings.value(key).toString();
	return value;
}
