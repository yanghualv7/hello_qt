#include "hello.h"
#include <QtWidgets/QApplication>
#include <QMessageBox>
#include <QCoreApplication>
#include <QSettings>
#include <QTextCodec>
#include <QDateTime>
#include <string.h>
#include <iostream>
#include "checkDateValidity.h"
#include "tcp_class.h"

using namespace std;

string encryptLicense(const QString& EncryptedLicense, int flag);
QString getMacAddress();
QString getFilePathFromIni(const QString& iniFilePath, const QString& key);
QString readLicenseFile(const QString& licensePath);
QString extractHexDigits(const QString& input);

//��ʾ���þ�̬���ļ�
#pragma comment(lib, "../Lib/checkDateValidity.lib")

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	// ��ȡ����MAC��ַ
	QString macAddress = getMacAddress();

	// ��ȡini�ļ�
	QString iniFilePath = "config/cfg.ini";
	QString user = getFilePathFromIni(iniFilePath, "UserName");
	QString LicensePath = getFilePathFromIni(iniFilePath, "LicensePath");
	QString serverIPQstr = getFilePathFromIni(iniFilePath, "ip");
	QString saveDatePath = getFilePathFromIni(iniFilePath, "SaveDatePath");

	std::string serverIP = serverIPQstr.toStdString();
	std::string directory = saveDatePath.toStdString();

	// ����TcpClientʵ��
	TcpClient client;

	// �ȴ�����������.....
	int num = 3;

	// ���ӵ�������
	while (num)
	{
		if (client.Connect(serverIP.c_str()) == 0)
		{
			num = 0;
			// ���ӵ��������ɹ�

			while (true)
			{
				std::string message = "SEND_FILE";
				// ��������

				if (client.SendData(message.c_str(), message.length()) != 0)
				{
					QMessageBox::critical(nullptr, "Error", QString(u8"���ݷ���ʧ��\n���򼴽��˳�"));
					// �˳�����
					return -1;
				}

				std::string receiveMessage = client.receiveFile(directory);
				QMessageBox::information(nullptr, "Message", QString::fromStdString(receiveMessage));

				message = "1";
				// ��������

				if (client.SendData(message.c_str(), message.length()) != 0)
				{
					QMessageBox::critical(nullptr, "Error", QString(u8"���ݷ���ʧ��\n���򼴽��˳�"));
					// �˳�����
					return -1;
				}

				// ��������
				char buffer[128];
				if (client.RecvData(buffer, sizeof(buffer)) == 0)
				{
					if (std::atoi(buffer) == -1)
					{
						QMessageBox::critical(nullptr, "Error", QString(u8"�豸ʹ��Ȩ���Ѵ�����\n���򼴽��˳�"));
						// �˳�����
						return -1;
					}
					else
					{
						QMessageBox::information(nullptr, "Message", QString(u8"��ʣ%1���豸��ʹ��").arg(std::atoi(buffer)));
						num = 0;
						break;
					}
				}
				else
				{
					QMessageBox::critical(nullptr, "Error", QString(u8"���շ���������ʧ��\n���򼴽��˳�"));
					// �˳�����
					return -1;
				}
			}
		}
		else
		{
			num--;
			if (num == 0)
			{
				QMessageBox::critical(nullptr, "Error", QString(u8"���ӵ�������ʧ�ܣ����Ӵ����Ѵ�����\n���򼴽��˳�"));
				return -1;
			}
			QMessageBox::information(nullptr, "Message", QString(u8"���ӵ�������ʧ�ܣ����ڳ����������ӷ�����\n��ʣ%1������").arg(num));

		}
	}


	QString LicensePath_user = LicensePath + "/" + user + ".lic";

	// ��ȡ���֤�ļ�
	QString storedEncryptedLicense = readLicenseFile(LicensePath_user);

	// ���ܺ��licenseString
	string License = encryptLicense(storedEncryptedLicense, 2);
	QString licenseString = QString::fromStdString(License);

	QDate date = QDate::currentDate(); // ��ȡ��ǰ����

	QString formattedDate = date.toString("yyyyMMdd");

	// ��ȡ License �е����ڲ���
	QString licenseDate = licenseString.section('|', 0, 0);
	QString License_nonDatePart = licenseString.section('|', 1, 1);

	//����Mac��ַ
	QString PcMAcAddress = getMacAddress();

	// �ָ�MAC��ַ
	QStringList macList = PcMAcAddress.split('|', QString::SkipEmptyParts);

	QString PcMAcAddress_;
	QString PcMAcAddressOnlyNum;
	bool isMatched = false;

	for (int i = 0; i < macList.size(); i++)
	{
		PcMAcAddress_ = macList[i];
		PcMAcAddressOnlyNum = extractHexDigits(PcMAcAddress_);
		// �Ƚ�mac��ַ�Ƿ�ƥ�䣬ͨ��trimmedȷ���ַ���û��ǰ��ո�
		if (License_nonDatePart.trimmed().compare(PcMAcAddressOnlyNum.trimmed(), Qt::CaseInsensitive) == 0)
		{
			isMatched = true;
			// ����ҵ�ƥ���MAC��ַ�����˳�ѭ��
			break;
		}
	}

	if (!isMatched)
	{
		// ������Ϣ��
		QMessageBox::critical(nullptr, "License Error", QString(u8"license mac��ַ��ƥ��! �ļ�·��: %1").arg(LicensePath_user));
		// �˳�����
		return -1;
	}


	// ��� License �Ƿ����
	if (!checkDateValidity(formattedDate.toStdString(), licenseDate.toStdString()))
	{
		QMessageBox::critical(nullptr, "License Error", QString(u8"license �ѹ���! ��ֹ����:%1\n �ļ�·��: %2").arg(licenseDate, LicensePath_user));
		return -1;
	}

	hello w(storedEncryptedLicense, user);
	w.show();
	return a.exec();
}

