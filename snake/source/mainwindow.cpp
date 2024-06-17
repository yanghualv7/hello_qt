#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QTextCodec>
#include <QNetworkInterface>
#include <QDateTime>
#include "GameArea.h"

//Copyright 2022 Yang Y., All rights reserved.
//������ģ�������ο�https://blog.csdn.net/Pumpkin_Tung/article/details/121443351
#include<conio.h>
#include<string>
#include "AES.h"
#include <QMessageBox>
//����д��Կ��չ��
//���AES��Կ��256��Base64�ַ���ɵĵ��ַ���,����ת�����192����λ��������ֱ����Ϊ��Կ��,�����ܹ�����12������11�ְա�
//��Կ��summonAESKey.py����
std::string key = "1145141919810Shimokita5ZUgHJOK0BHEv6,AH3S,RrkQU58zxG,BLf/sguM9fAocN7xwUfb,v8L8eaTGpYzdy7HTiKqPXj6D6,7ej3NfqoL29Av7ZOWPj5l1HqiVNG60i+g1GF,J+qr3sa+32MZuuiZynOyJwFGP,bFEKFDDGSmb0QrRL3Pl8M3ay68o0inrPBYiz+Btm9fhWt1cQ9TZxTWQ0QQBsMqAIV9mB2Udx1DEaGfZPFpR8c148QnN0T";//AES��Կ,256λ

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

// ����/���ܺ���  ����ģʽ:����(1)����(2)
string encryptLicense(const QString& EncryptedLicense, int flag)
{
	string ss;
	init();

	//��ʹ���ڼ��ʱ����м�¼���ܻ�����Ľ���
	ss = aes(key, flag, EncryptedLicense.toStdString());

	return ss;

}


QString getMacAddress()
{
	QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces(); // ��ȡ��������ӿ��б�
	QStringList macAddresses;

	for (const QNetworkInterface& net : nets)
	{
		macAddresses.append(net.hardwareAddress());
	}

	return macAddresses.join("|");
}


// ��ini�ļ���ȡ·���ĺ���
QString getFilePathFromIni(const QString& iniFilePath, const QString& key)
{
	QSettings settings(iniFilePath, QSettings::IniFormat);

	// ���� ini �ļ���ȡ�ı���Ϊ UTF-8
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
	QString value = settings.value(key).toString();
	return value;
}

// ��ȡ���֤�ļ��ĺ���
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
	timer->start(60000); // �Ժ���Ϊ��λ

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

	//ÿ���ӷ������ж��Ƿ񳬵�ǰ��ߣ�������˾͸���
	//����׼������Ϸ����ʱ������߷ֱȽϣ�����������һ�浽�ܸ߷��ˣ��쳣�����������û���£��Ƚ��ź�
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
	// ��ȡ��ǰʱ��
	QString currentTime = QDateTime::currentDateTime().toString("yyyyMMddHHmmss") + "|";

	std::string encryptTime = encryptLicense(currentTime, 1);

	// �����ļ�·��
	QString fileName = "log/" + user + "_time_" + ".txt";
	QFile file(fileName);

	if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
		QTextStream out(&file);
		out << QString::fromStdString(encryptTime);
		file.close();
	}
	else {
		QMessageBox::warning(this, tr(u8"����"), tr(u8"�޷����ļ�����д��: %1").arg(fileName));
		return;
	}

	// ��ȡ��������
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream in(&file);
		QString fileContent = in.readAll();
		file.close();

		std::string decryptTime = encryptLicense(fileContent, 2);
		if (decryptTime.empty())
		{
			QMessageBox::warning(this, tr(u8"����"), tr(u8"ʱ������޸ģ����򼴽��˳�"));
			QApplication::quit(); // �رճ���
		}

		QString decryptTime_ = QString::fromStdString(decryptTime);

		// �ָ��ȡ�����ݲ�����Ƿ����
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
				QMessageBox::warning(this, tr(u8"����"), tr(u8"ʱ��δ��˳�����,�����޸�ʱ��\n���򼴽��˳�"));
				QApplication::quit(); // �رճ���
			}
		}
	}
}
