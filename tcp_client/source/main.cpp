#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "tcp_class.h"
#include <iostream>
#include <fstream>
#include <sstream>
std::string readFromIni(const std::string& iniFile, const std::string keyName)
{
	std::ifstream file(iniFile);
	std::string line;
	std::string valueName = "0";

	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string key, value;
		if (std::getline(iss, key, '=') && std::getline(iss, value))
		{
			if (key == keyName)
			{
				valueName = value;
				break;
			}
		}
	}

	return valueName;
}
int main()
{
	std::string serverIP = readFromIni("config/cfg.ini", "ip");

	// ����TcpClientʵ��
	TcpClient client;
	std::cout << "�ȴ�����������.....\n" << std::endl;
	int num = 10;

	// ���ӵ�������
	while (num)
	{
		if (client.Connect(serverIP.c_str()) == 0)
		{
			num = 0;
			std::cout << "\n���ӵ��������ɹ�" << std::endl;

			while (true)
			{
				// ���û������ȡ����
				std::string message;
				std::cout << "\n����Ҫ���͵���Ϣ: ";
				std::getline(std::cin, message);

				// ��������
				if (client.SendData(message.c_str(), message.length()) == 0)
				{
					std::cout << "\n���ݷ��ͳɹ�" << std::endl;
				}
				else
				{
					std::cout << "\n���ݷ���ʧ��" << std::endl;
				}

				// ��������
				char buffer[128];
				if (client.RecvData(buffer, sizeof(buffer)) == 0)
				{
					std::cout << "\n��������Ӧ: " << buffer << std::endl;
				}
				else
				{
					std::cout << "\n���ݽ���ʧ��" << std::endl;
					break;
				}
			}
		}
		else
		{
			std::cout << "\n���ӵ�������ʧ�ܣ����ڳ����������ӷ�����" << std::endl;
			std::cout << "ʣ�ࣺ" << num - 1 << "��" << std::endl;
			num--;
		}
	}

	return 0;
}

