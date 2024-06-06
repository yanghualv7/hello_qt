#include "hello.h"
#include <QtWidgets/QApplication>
#include <QMessageBox>
#include <QCoreApplication>
#include <QSettings>
#include <QTextCodec>
#include <QDateTime>
#include <string.h>
using namespace std;


string encryptLicense(const QString& storedEncryptedLicense);
QString getMacAddress();
QString getFilePathFromIni(const QString& iniFilePath, const QString& key);
QString readLicenseFile(const QString& licensePath);
QString extractHexDigits(const QString& input);

bool isLicenseExpired(const QString& currentDateStr, const QString& licenseDateStr)
{
	int currentYear = currentDateStr.mid(0, 4).toInt();
	int currentMonth = currentDateStr.mid(4, 2).toInt();
	int currentDay = currentDateStr.mid(6, 2).toInt();

	int licenseYear = licenseDateStr.mid(0, 4).toInt();
	int licenseMonth = licenseDateStr.mid(4, 2).toInt();
	int licenseDay = licenseDateStr.mid(6, 2).toInt();

	if (currentYear < licenseYear)
	{
		//δ����
		return true;
	}
	else if (currentMonth < licenseMonth)
	{
		//δ����
		return true;
	}
	else if (currentDay < licenseDay)
	{
		//δ����
		return true;
	}
	else
	{
		//�ѹ���
		return false;
	}
}

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	// ��ȡ����MAC��ַ
	QString macAddress = getMacAddress();

	// ��ȡini�ļ�
	QString iniFilePath = "config/cfg.ini";
	QString user = getFilePathFromIni(iniFilePath, "UserName");
	QString LicensePath = getFilePathFromIni(iniFilePath, "LicensePath");

	QString LicensePath_user = LicensePath + "/" + user + ".lic";

	// ��ȡ����֤�ļ�
	QString storedEncryptedLicense = readLicenseFile(LicensePath_user);

	// ���ܺ��licenseString
	string License = encryptLicense(storedEncryptedLicense);
	QString licenseString = QString::fromStdString(License);

	QDate date = QDate::currentDate(); // ��ȡ��ǰ����

	QString formattedDate = date.toString("yyyyMMdd");

	// ��ȡ License �е����ڲ���
	QString licenseDate = licenseString.section('|', 0, 0);
	QString License_nonDatePart = licenseString.section('|', 1, 1);

	//����Mac��ַ
	QString PcMAcAddress = getMacAddress();
	//ֻ�������ֲ���
	QString PcMAcAddress_ = extractHexDigits(PcMAcAddress);

	// ��� License �Ƿ����
	if (!isLicenseExpired(formattedDate, licenseDate))
	{
		QMessageBox::critical(nullptr, "License Error", QString(u8"license �ѹ���! ��ֹ����:%1\n �ļ�·��: %2").arg(licenseDate, LicensePath_user));
		return -1;
	}

	// �Ƚ���mac��ַ�Ƿ�ƥ�䣬ͨ��trimmedȷ���ַ���û��ǰ��ո�
	if (License_nonDatePart.trimmed().compare(PcMAcAddress_.trimmed(), Qt::CaseInsensitive) != 0)
	{
		// ������Ϣ��
		QMessageBox::critical(nullptr, "License Error", QString(u8"license mac��ַ��ƥ��! �ļ�·��: %1").arg(LicensePath_user));
		// �˳�����
		return -1;
	}

	hello w(storedEncryptedLicense, user);
	w.show();
	return a.exec();
}
