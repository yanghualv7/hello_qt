#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "tcp_class.h"
#include <fstream>
#include <sstream>
#include <iostream>
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
	std::string maxClientCount = readFromIni("config/cfg.ini", "MaxClientCount");

	// 创建TcpServer实例
	TcpServer server(std::stoi(maxClientCount));

	std::cout << "等待客户端连接......\n" << std::endl;

	// 启动处理客户端连接的函数
	server.handleConnections();

	return 0;
}

