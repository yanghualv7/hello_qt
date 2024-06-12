#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "tcp_class.h"
#include <iostream>
#include <stdio.h>
#include <process.h>
#include <ws2tcpip.h>
#include <sstream>
#include <atomic>
// 构造函数
TcpServer::TcpServer(int maxClientCount) :clientCount(0), MaxClientCount(maxClientCount)
{
	WSADATA wsadata;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsadata))
	{
		std::cout << "\n套接字打开失败" << std::endl;
		return;
	}

	TcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(6000);
	addr.sin_family = AF_INET;

	bind(TcpSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR));
	listen(TcpSocket, 5);
}

// 析构函数
TcpServer::~TcpServer()
{
	closesocket(TcpSocket);
	WSACleanup();
}

// 处理客户端链接
void TcpServer::handleConnections()
{
	sockaddr_in remoteAddr;
	int nAddrLen = sizeof(remoteAddr);
	SOCKET currentConnection;
	while (1)
	{
		Sleep(500);
		currentConnection = accept(TcpSocket, (SOCKADDR*)&remoteAddr, &nAddrLen);
		if (currentConnection != INVALID_SOCKET)
		{
			{
				std::lock_guard<std::mutex> lock(clientMutex);
				clientCount++;
				clientSockets.push_back(currentConnection);
			}
			_beginthread(TcpServer::handleRequests, 0, this); // 传递当前对象指针
		}
	}
}

void TcpServer::handleRequests(LPVOID param)
{
	TcpServer* server = static_cast<TcpServer*>(param);
	SOCKET currentConnection = server->clientSockets.back(); // 获取当前连接的socket
	server->handleClientRequests(currentConnection);
}

void TcpServer::handleClientRequests(SOCKET currentConnection)
{
	char buf[128];
	while (1)
	{
		int bytesReceived = recv(currentConnection, buf, 128, 0);
		if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)
		{
			closesocket(currentConnection); // 断开连接
			{
				std::lock_guard<std::mutex> lock(clientMutex);
				clientCount--;
				clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), currentConnection), clientSockets.end());
			}
			return;
		}
		else
		{
			buf[bytesReceived] = '\0'; // 以空字符结尾接收数据

			std::cout << "\n收到客户端消息: " << buf << std::endl;

			getClientCount();

			std::cout << "\n当前设备数：" << clientCount << std::endl;
			int laveClientCount = MaxClientCount - clientCount;

			if (MaxClientCount >= clientCount)
			{
				// 服务器发送消息
				std::string message = std::to_string(laveClientCount);
				// 发送回执
				if (send(currentConnection, message.c_str(), message.length(), 0) == SOCKET_ERROR)
				{
					std::cout << "\n发送回执失败" << std::endl;
				}

			}
			else
			{
				std::cout << "\n登录设备已达上限\n";				// 服务器发送消息
				std::string message = "-1";
				// 发送回执
				if (send(currentConnection, message.c_str(), message.length(), 0) == SOCKET_ERROR)
				{
					std::cout << "\n发送回执失败" << std::endl;
				}

			}
		}
	}
}

int TcpServer::getClientCount() const
{
	std::lock_guard<std::mutex> lock(clientMutex);
	return clientCount;
}

std::vector<SOCKET> TcpServer::getClientSockets() const
{
	std::lock_guard<std::mutex> lock(clientMutex);
	return clientSockets;
}

