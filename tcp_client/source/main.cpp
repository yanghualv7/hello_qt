#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "tcp_class.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string readIPFromIni(const std::string& iniFile, const std::string keyName)
{
	std::ifstream file(iniFile);
	std::string line;
	std::string ip = "127.0.0.1"; // Ĭ��IP��ַ

	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string key, value;
		if (std::getline(iss, key, '=') && std::getline(iss, value))
		{
			if (key == keyName)
			{
				ip = value;
				break;
			}
		}
	}

	return ip;
}

int main()
{
	std::string serverIP = readIPFromIni("config/cfg.ini", "ip");

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

