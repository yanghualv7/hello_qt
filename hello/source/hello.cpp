#include "hello.h"
#include "ui_hello.h"
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QNetworkInterface>
#include <QIcon>
#include <QPixmap>
#include <QSettings>

// 加密许可证的函数
QString encryptLicense(const QString& user, const QString& macAddress)
{
	// 提取 macAddress 中的数字部分
	QString numericMacAddress;
	for (QChar c : macAddress)
	{
		if (c.isDigit())
			numericMacAddress += c;
	}

	QString license = user + ":" + numericMacAddress;
	QByteArray encrypted = license.toUtf8().toBase64();
	return QString(encrypted);
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

hello::hello(const QString& user, QWidget* parent)
	: QMainWindow(parent),
	ui(new Ui::helloClass)
{
	ui->setupUi(this);

	// 获取本机MAC地址
	QString macAddress = getMacAddress();
	ui->macLabel->setText("MAC Address: " + macAddress);
	ui->statusLabel->setText(u8"读取 license 成功!");

	QString encryptedLicense = encryptLicense(user, macAddress);
	ui->macLabel_2->setText("License: " + encryptedLicense);

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

