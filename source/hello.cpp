#include "hello.h"
#include "ui_hello.h"
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QNetworkInterface>
#include <QIcon>
#include <QPixmap>
#include <QSettings>
#include <QDateTime>
#include <AES.h>

//Copyright 2022 Yang Y., All rights reserved.
//快速幂模运算代码参考https://blog.csdn.net/Pumpkin_Tung/article/details/121443351
#include<conio.h>
#include<string>
#include "AES.h"
//懒得写密钥扩展了
//这个AES密钥是256个Base64字符组成的的字符串,经过转换变成192个八位二进制数直接作为密钥组,加密总共加密12轮甚至11轮罢。
//密钥用summonAESKey.py生成
std::string key = "1145141919810Shimokita5ZUgHJOK0BHEv6,AH3S,RrkQU58zxG,BLf/sguM9fAocN7xwUfb,v8L8eaTGpYzdy7HTiKqPXj6D6,7ej3NfqoL29Av7ZOWPj5l1HqiVNG60i+g1GF,J+qr3sa+32MZuuiZynOyJwFGP,bFEKFDDGSmb0QrRL3Pl8M3ay68o0inrPBYiz+Btm9fhWt1cQ9TZxTWQ0QQBsMqAIV9mB2Udx1DEaGfZPFpR8c148QnN0T";//AES密钥,256位

QString extractHexDigits(const QString& input)
{
	QString result;
	QRegularExpression hexDigitRegex("[0-9A-Fa-f]");
	QRegularExpressionMatchIterator i = hexDigitRegex.globalMatch(input);
	while (i.hasNext()) {
		QRegularExpressionMatch match = i.next();
		result += match.captured(0);
	}
	return result;
}

// 解密函数
string encryptLicense(const QString& storedEncryptedLicense)
{
	init();

init:
	//选择工作模式:加密(1)解密(2)

	string opt = storedEncryptedLicense.toStdString();
	////密文
	//string opt = aes(key, 1, inp);
	//明文
	string ss = aes(key, 2, opt);

	return ss;

}

// 获取MAC地址的函数
QString getMacAddress()
{
	QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces(); // 获取所有网络接口列表
	int nCnt = nets.count();
	QString strMacAddr = "";
	for (int i = 0; i < nCnt; i++)
	{
		// 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
		if (nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
		{
			strMacAddr = nets[i].hardwareAddress();
			break;
		}
	}
	return strMacAddr;
}

// 从ini文件获取路径的函数
QString getFilePathFromIni(const QString& iniFilePath, const QString& key)
{
	QSettings settings(iniFilePath, QSettings::IniFormat);

	// 设置 ini 文件读取的编码为 UTF-8
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
	QString value = settings.value(key).toString();
	return value;
}

// 读取许可证文件的函数
QString readLicenseFile(const QString& licensePath)
{
	QFile licenseFile(licensePath);
	if (licenseFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString storedEncryptedLicense = licenseFile.readAll().trimmed();
		licenseFile.close();
		return storedEncryptedLicense;
	}
	else
		return QString();
}

hello::hello(const QString& storedEncryptedLicense, const QString& user, QWidget* parent)
	: QMainWindow(parent),
	ui(new Ui::helloClass)
{
	ui->setupUi(this);

	// 获取本机MAC地址
	QString macAddress = getMacAddress();
	ui->macLabel->setText("MAC Address: " + macAddress);
	ui->statusLabel->setText(u8"读取 license 成功!");


	string encryptedLicense_ = encryptLicense(storedEncryptedLicense);


	ui->macLabel_2->setText("License: " + QString::fromStdString(encryptedLicense_));

	// 读取ini文件
	QString iniFilePath = "config/cfg.ini";
	QString filePath = getFilePathFromIni(iniFilePath, "FilePath");
	QString imagePath = getFilePathFromIni(iniFilePath, "ImagePath");
	QString icoPath = getFilePathFromIni(iniFilePath, "QiconPath");

	// 设置窗口图标
	QIcon icon(icoPath);
	this->setWindowIcon(icon);

	loadFile(filePath, imagePath, iniFilePath);

	// 加载图像
	QPixmap pixmap(imagePath);

	if (pixmap.isNull())
		ui->imageLabel->setText("图像无法加载！");
	else
	{
		// 设置图像的原始大小
		ui->imageLabel->setPixmap(pixmap);
		ui->imageLabel->adjustSize(); // 调整 QLabel 大小以适应图像大小
	}
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
		ui->textBrowser->setHtml(htmlContent);

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
		ui->textBrowser->setHtml(errorMessage);
	}
}

