#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QTextCodec>
#include <QNetworkInterface>
#include <QDateTime>
#include "GameArea.h"

//Copyright 2022 Yang Y., All rights reserved.
//快速幂模运算代码参考https://blog.csdn.net/Pumpkin_Tung/article/details/121443351
#include<conio.h>
#include<string>
#include "AES.h"
#include <QMessageBox>
//懒得写密钥扩展了
//这个AES密钥是256个Base64字符组成的的字符串,经过转换变成192个八位二进制数直接作为密钥组,加密总共加密12轮甚至11轮罢。
//密钥用summonAESKey.py生成
std::string key = "1145141919810Shimokita5ZUgHJOK0BHEv6,AH3S,RrkQU58zxG,BLf/sguM9fAocN7xwUfb,v8L8eaTGpYzdy7HTiKqPXj6D6,7ej3NfqoL29Av7ZOWPj5l1HqiVNG60i+g1GF,J+qr3sa+32MZuuiZynOyJwFGP,bFEKFDDGSmb0QrRL3Pl8M3ay68o0inrPBYiz+Btm9fhWt1cQ9TZxTWQ0QQBsMqAIV9mB2Udx1DEaGfZPFpR8c148QnN0T";//AES密钥,256位

QString extractHexDigits(const QString& input)
{
	QString result;
	QRegularExpression hexDigitRegex("[0-9A-Fa-f]");
	QRegularExpressionMatchIterator i = hexDigitRegex.globalMatch(input);
	while (i.hasNext())
	{
		QRegularExpressionMatch match = i.next();
		result += match.captured(0);
	}
	return result;
}

// 加密/解密函数  工作模式:加密(1)解密(2)
string encryptLicense(const QString& EncryptedLicense, int flag)
{
	string ss;
	init();

	//在使用期间对时间进行记录加密或对密文解密
	ss = aes(key, flag, EncryptedLicense.toStdString());

	return ss;

}


QString getMacAddress()
{
	QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces(); // 获取所有网络接口列表
	QStringList macAddresses;

	for (const QNetworkInterface& net : nets)
	{
		macAddresses.append(net.hardwareAddress());
	}

	return macAddresses.join("|");
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


mainwindow::mainwindow(const QString& user, QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::mainwindow),
	user(user)
{
	// Initialize the timer
	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &mainwindow::writeTimeToFile);
	timer->start(60000); // 以毫秒为单位

	ui->setupUi(this);
	//    setFixedSize(1000,800);
	resize(RECT_COLUMES * RECT_WIDTH + 300, RECT_ROWS * RECT_HEIGHT);

	int nMaxScore = GetHistoryMaxScore();
	ui->labelMaxScore->setText(QString::number(nMaxScore));

	connect(ui->widgetGameArea, &GameArea::sigUpdateScore, this, &mainwindow::slotUpdateScore);

	ui->widgetGameArea->NewGame();
}

mainwindow::~mainwindow()
{
	delete ui;
}

void mainwindow::keyPressEvent(QKeyEvent* e)
{
	ui->widgetGameArea->KeyPressed(e->key());
	QMainWindow::keyPressEvent(e);
}

void mainwindow::slotUpdateScore(int nScore)
{
	ui->labelScore->setText(QString::number(nScore));

	//每增加分数都判断是否超当前最高，如果超了就更新
	//本来准备在游戏结束时，与最高分比较，但是这样万一玩到很高分了，异常结束结果导致没更新，比较遗憾
	if (nScore > GetHistoryMaxScore())
	{
		ui->labelMaxScore->setText(QString::number(nScore));
		SaveHistoryMaxScore(nScore);
	}
}

int mainwindow::GetHistoryMaxScore()
{
	QString path = QCoreApplication::applicationDirPath() + "/snake.ini";
	QSettings s(path, QSettings::IniFormat);
	return s.value("Main/MaxScore", 0).toInt();
}

void mainwindow::SaveHistoryMaxScore(int nScore)
{
	QString path = QCoreApplication::applicationDirPath() + "/snake.ini";
	QSettings s(path, QSettings::IniFormat);
	s.setValue("Main/MaxScore", nScore);
}

void mainwindow::writeTimeToFile()
{
	// 获取当前时间
	QString currentTime = QDateTime::currentDateTime().toString("yyyyMMddHHmmss") + "|";

	std::string encryptTime = encryptLicense(currentTime, 1);

	// 定义文件路径
	QString fileName = "log/" + user + "_time_" + ".txt";
	QFile file(fileName);

	if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
		QTextStream out(&file);
		out << QString::fromStdString(encryptTime);
		file.close();
	}
	else {
		QMessageBox::warning(this, tr(u8"错误"), tr(u8"无法打开文件进行写入: %1").arg(fileName));
		return;
	}

	// 读取完整数据
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream in(&file);
		QString fileContent = in.readAll();
		file.close();

		std::string decryptTime = encryptLicense(fileContent, 2);
		if (decryptTime.empty())
		{
			QMessageBox::warning(this, tr(u8"警告"), tr(u8"时间戳被修改，程序即将退出"));
			QApplication::quit(); // 关闭程序
		}

		QString decryptTime_ = QString::fromStdString(decryptTime);

		// 分割读取的内容并检查是否递增
		QStringList timeList = decryptTime_.split("| ", QString::SkipEmptyParts);
		bool isIncreasing = true;

		if (timeList.size() > 1)
		{
			for (int i = 1; i < timeList.size(); ++i)
			{
				if (timeList[i] <= timeList[i - 1])
				{
					isIncreasing = false;
					break;
				}
			}

			if (!isIncreasing)
			{
				QMessageBox::warning(this, tr(u8"警告"), tr(u8"时间未按顺序递增,请勿修改时间\n程序即将退出"));
				QApplication::quit(); // 关闭程序
			}
		}
	}
}
