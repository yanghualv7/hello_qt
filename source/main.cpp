#include "hello.h"
#include <QtWidgets/QApplication>
#include <QMessageBox>
#include <QCoreApplication>
#include <QSettings>
#include <QTextCodec>

QString encryptLicense(const QString& user, const QString& macAddress);
QString getMacAddress();
QString getFilePathFromIni(const QString& iniFilePath, const QString& key);
QString readLicenseFile(const QString& licensePath);

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	// 获取本机MAC地址
	QString macAddress = getMacAddress();

	// 读取ini文件
	QString iniFilePath = "config/cfg.ini";
	QString user = getFilePathFromIni(iniFilePath, "UserName");
	QString LicensePath = getFilePathFromIni(iniFilePath, "LicensePath");

	// 生成加密后的license
	QString encryptedLicense = encryptLicense(user, macAddress);

	QString LicensePath_user = LicensePath + "/" + user + ".lic";

	// 读取许可证文件
	QString storedEncryptedLicense = readLicenseFile(LicensePath_user);

	// 比较许可证是否匹配
	if (storedEncryptedLicense != encryptedLicense)
	{
		// 弹出消息框
		QMessageBox::critical(nullptr, "License Error", QString(u8"读取或匹配 license 失败! 文件路径: %1").arg(LicensePath_user));

		// 退出程序
		return -1; // 退出程序
	}

	hello w(user);
	w.show();
	return a.exec();
}

