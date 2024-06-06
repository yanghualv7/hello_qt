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

	if (currentYear > licenseYear)
	{
		//已过期
		return false;
	}
	else if (currentMonth > licenseMonth)
	{
		//已过期
		return false;
	}
	else if (currentDay > licenseDay)
	{
		//已过期
		return false;
	}
	else
	{
		//未过期
		return true;
	}
}

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	// 获取本机MAC地址
	QString macAddress = getMacAddress();

	// 读取ini文件
	QString iniFilePath = "config/cfg.ini";
	QString user = getFilePathFromIni(iniFilePath, "UserName");
	QString LicensePath = getFilePathFromIni(iniFilePath, "LicensePath");

	QString LicensePath_user = LicensePath + "/" + user + ".lic";

	// 读取许可证文件
	QString storedEncryptedLicense = readLicenseFile(LicensePath_user);

	// 解密后的licenseString
	string License = encryptLicense(storedEncryptedLicense);
	QString licenseString = QString::fromStdString(License);

	QDate date = QDate::currentDate(); // 获取当前日期

	QString formattedDate = date.toString("yyyyMMdd");

	// 提取 License 中的日期部分
	QString licenseDate = licenseString.section('|', 0, 0);
	QString License_nonDatePart = licenseString.section('|', 1, 1);

	//本机Mac地址
	QString PcMAcAddress = getMacAddress();
	//只保留数字部分
	QString PcMAcAddress_ = extractHexDigits(PcMAcAddress);

	// 检查 License 是否过期
	if (!isLicenseExpired(formattedDate, licenseDate))
	{
		QMessageBox::critical(nullptr, "License Error", QString(u8"license 已过期! 截止日期:%1\n 文件路径: %2").arg(licenseDate, LicensePath_user));
		return -1;
	}

	// 比较许mac地址是否匹配，通过trimmed确保字符串没有前后空格
	if (License_nonDatePart.trimmed().compare(PcMAcAddress_.trimmed(), Qt::CaseInsensitive) != 0)
	{
		// 弹出消息框
		QMessageBox::critical(nullptr, "License Error", QString(u8"license mac地址不匹配! 文件路径: %1").arg(LicensePath_user));
		// 退出程序
		return -1;
	}

	hello w(storedEncryptedLicense, user);
	w.show();
	return a.exec();
}

