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

	// ��ȡ����MAC��ַ
	QString macAddress = getMacAddress();

	// ��ȡini�ļ�
	QString iniFilePath = "config/cfg.ini";
	QString user = getFilePathFromIni(iniFilePath, "UserName");
	QString LicensePath = getFilePathFromIni(iniFilePath, "LicensePath");

	// ���ɼ��ܺ��license
	QString encryptedLicense = encryptLicense(user, macAddress);

	QString LicensePath_user = LicensePath + "/" + user + ".lic";

	// ��ȡ���֤�ļ�
	QString storedEncryptedLicense = readLicenseFile(LicensePath_user);

	// �Ƚ����֤�Ƿ�ƥ��
	if (storedEncryptedLicense != encryptedLicense)
	{
		// ������Ϣ��
		QMessageBox::critical(nullptr, "License Error", QString(u8"��ȡ��ƥ�� license ʧ��! �ļ�·��: %1").arg(LicensePath_user));

		// �˳�����
		return -1; // �˳�����
	}

	hello w(user);
	w.show();
	return a.exec();
}

