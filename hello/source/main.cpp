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

//隐示调用静态库文件
#pragma comment(lib, "../Lib/checkDateValidity.lib")

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	// 获取本机MAC地址
	QString macAddress = getMacAddress();

	// 读取ini文件
	QString iniFilePath = "config/cfg.ini";
	QString user = getFilePathFromIni(iniFilePath, "UserName");
	QString LicensePath = getFilePathFromIni(iniFilePath, "LicensePath");
	QString serverIPQstr = getFilePathFromIni(iniFilePath, "ip");
	QString saveDatePath = getFilePathFromIni(iniFilePath, "SaveDatePath");

	std::string serverIP = serverIPQstr.toStdString();
	std::string directory = saveDatePath.toStdString();

	// 创建TcpClient实例
	TcpClient client;

	// 等待服务器启动.....
	int num = 3;

	// 连接到服务器
	while (num)
	{
		if (client.Connect(serverIP.c_str()) == 0)
		{
			num = 0;
			// 连接到服务器成功

			while (true)
			{
				std::string message = "SEND_FILE";
				// 发送数据

				if (client.SendData(message.c_str(), message.length()) != 0)
				{
					QMessageBox::critical(nullptr, "Error", QString(u8"数据发送失败\n程序即将退出"));
					// 退出程序
					return -1;
				}

				std::string receiveMessage = client.receiveFile(directory);
				QMessageBox::information(nullptr, "Message", QString::fromStdString(receiveMessage));

				message = "1";
				// 发送数据

				if (client.SendData(message.c_str(), message.length()) != 0)
				{
					QMessageBox::critical(nullptr, "Error", QString(u8"数据发送失败\n程序即将退出"));
					// 退出程序
					return -1;
				}

				// 接收数据
				char buffer[128];
				if (client.RecvData(buffer, sizeof(buffer)) == 0)
				{
					if (std::atoi(buffer) == -1)
					{
						QMessageBox::critical(nullptr, "Error", QString(u8"设备使用权限已达上限\n程序即将退出"));
						// 退出程序
						return -1;
					}
					else
					{
						QMessageBox::information(nullptr, "Message", QString(u8"还剩%1个设备可使用").arg(std::atoi(buffer)));
						num = 0;
						break;
					}
				}
				else
				{
					QMessageBox::critical(nullptr, "Error", QString(u8"接收服务器数据失败\n程序即将退出"));
					// 退出程序
					return -1;
				}
			}
		}
		else
		{
			num--;
			if (num == 0)
			{
				QMessageBox::critical(nullptr, "Error", QString(u8"连接到服务器失败，连接次数已达上限\n程序即将退出"));
				return -1;
			}
			QMessageBox::information(nullptr, "Message", QString(u8"连接到服务器失败，正在尝试重新连接服务器\n还剩%1次连接").arg(num));

		}
	}


	QString LicensePath_user = LicensePath + "/" + user + ".lic";

	// 读取许可证文件
	QString storedEncryptedLicense = readLicenseFile(LicensePath_user);

	// 解密后的licenseString
	string License = encryptLicense(storedEncryptedLicense, 2);
	QString licenseString = QString::fromStdString(License);

	QDate date = QDate::currentDate(); // 获取当前日期

	QString formattedDate = date.toString("yyyyMMdd");

	// 提取 License 中的日期部分
	QString licenseDate = licenseString.section('|', 0, 0);
	QString License_nonDatePart = licenseString.section('|', 1, 1);

	//本机Mac地址
	QString PcMAcAddress = getMacAddress();

	// 分割MAC地址
	QStringList macList = PcMAcAddress.split('|', QString::SkipEmptyParts);

	QString PcMAcAddress_;
	QString PcMAcAddressOnlyNum;
	bool isMatched = false;

	for (int i = 0; i < macList.size(); i++)
	{
		PcMAcAddress_ = macList[i];
		PcMAcAddressOnlyNum = extractHexDigits(PcMAcAddress_);
		// 比较mac地址是否匹配，通过trimmed确保字符串没有前后空格
		if (License_nonDatePart.trimmed().compare(PcMAcAddressOnlyNum.trimmed(), Qt::CaseInsensitive) == 0)
		{
			isMatched = true;
			// 如果找到匹配的MAC地址，则退出循环
			break;
		}
	}

	if (!isMatched)
	{
		// 弹出消息框
		QMessageBox::critical(nullptr, "License Error", QString(u8"license mac地址不匹配! 文件路径: %1").arg(LicensePath_user));
		// 退出程序
		return -1;
	}


	// 检查 License 是否过期
	if (!checkDateValidity(formattedDate.toStdString(), licenseDate.toStdString()))
	{
		QMessageBox::critical(nullptr, "License Error", QString(u8"license 已过期! 截止日期:%1\n 文件路径: %2").arg(licenseDate, LicensePath_user));
		return -1;
	}

	hello w(storedEncryptedLicense, user);
	w.show();
	return a.exec();
}

