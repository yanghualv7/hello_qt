#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "tcp_class.h"
#include <fstream>
#include <sstream>
#include <iostream>
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
	std::string maxClientCount = readIPFromIni("config/cfg.ini", "MaxClientCount");

	// ����TcpServerʵ��
	TcpServer server(std::stoi(maxClientCount));

	std::cout << "�ȴ��ͻ�������......\n" << std::endl;

	// ��������ͻ������ӵĺ���
	server.handleConnections();

	return 0;
}

