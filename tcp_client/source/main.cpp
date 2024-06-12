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

	// 创建TcpClient实例
	TcpClient client;
	std::cout << "等待服务器启动.....\n" << std::endl;
	int num = 10;

	// 连接到服务器
	while (num)
	{
		if (client.Connect(serverIP.c_str()) == 0)
		{
			num = 0;
			std::cout << "\n连接到服务器成功" << std::endl;

			while (true)
			{
				// 从用户输入读取数据
				std::string message;
				std::cout << "\n输入要发送的消息: ";
				std::getline(std::cin, message);

				// 发送数据
				if (client.SendData(message.c_str(), message.length()) == 0)
				{
					std::cout << "\n数据发送成功" << std::endl;
				}
				else
				{
					std::cout << "\n数据发送失败" << std::endl;
				}

				// 接收数据
				char buffer[128];
				if (client.RecvData(buffer, sizeof(buffer)) == 0)
				{
					std::cout << "\n服务器回应: " << buffer << std::endl;
				}
				else
				{
					std::cout << "\n数据接收失败" << std::endl;
					break;
				}
			}
		}
		else
		{
			std::cout << "\n连接到服务器失败，正在尝试重新连接服务器" << std::endl;
			std::cout << "剩余：" << num - 1 << "次" << std::endl;
			num--;
		}
	}

	return 0;
}

