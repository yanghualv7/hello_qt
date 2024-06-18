#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "tcp_class.h"
#include <iostream>
#include <stdio.h>
#include <process.h>
#include <ws2tcpip.h>
#include <sstream>
#include <atomic>
#include <fstream>
// 构造函数
TcpServer::TcpServer(int maxClientCount, std::string sendFilePath) :clientCount(0),
MaxClientCount(maxClientCount), SendFilePath(sendFilePath)
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
	addr.sin_port = htons(7880);
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

void TcpServer::sendFile(SOCKET clientSocket, const std::string& filePath)
{
	// 发送文件名
	std::string fileName = filePath.substr(filePath.find_last_of("/\\") + 1);
	int fileNameSize = fileName.size();
	// 发送文件名长度
	send(clientSocket, reinterpret_cast<const char*>(&fileNameSize), sizeof(fileNameSize), 0);
	// 发送文件名
	send(clientSocket, fileName.c_str(), fileNameSize, 0);

	// 打开文件
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "无法打开文件: " << filePath << std::endl;
		return;
	}

	// 获取文件大小
	file.seekg(0, std::ios::end);
	int fileSize = file.tellg();
	file.seekg(0, std::ios::beg);
	// 发送文件大小
	send(clientSocket, reinterpret_cast<const char*>(&fileSize), sizeof(fileSize), 0);

	// 发送文件内容
	char buffer[1024];
	while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0)
	{
		int bytesToSend = file.gcount();
		int bytesSent = send(clientSocket, buffer, bytesToSend, 0);
		if (bytesSent == SOCKET_ERROR)
		{
			std::cerr << "文件发送失败" << std::endl;
			break;
		}
	}

	file.close();

	// 接收客户端的结束信号
	char endBuffer[128];
	int bytesReceived = recv(clientSocket, endBuffer, sizeof(endBuffer), 0);
	if (bytesReceived > 0)
	{
		std::string endMessage(endBuffer, bytesReceived);
		if (endMessage == "RECEIVE_COMPLETE")
		{
			std::cout << "客户端文件接收完成: " << filePath << std::endl;
		}
	}
}


void TcpServer::handleClientRequests(SOCKET currentConnection)
{
	char buf[128];
	while (1)
	{
		int bytesReceived = recv(currentConnection, buf, 128, 0);
		if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)
		{
			closesocket(currentConnection);
			{
				std::lock_guard<std::mutex> lock(clientMutex);
				clientCount--;
				clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), currentConnection), clientSockets.end());
			}
			return;
		}
		else
		{
			buf[bytesReceived] = '\0';

			std::string receivedMessage(buf);
			std::cout << "\n收到客户端消息: " << receivedMessage << std::endl;

			if (receivedMessage == "SEND_FILE")
			{
				sendFile(currentConnection, SendFilePath);
			}
			else
			{
				// 其他处理逻辑
				getClientCount();

				std::cout << "\n当前设备数：" << clientCount << std::endl;
				int laveClientCount = MaxClientCount - clientCount;

				if (MaxClientCount >= clientCount)
				{
					std::string message = std::to_string(laveClientCount);
					if (send(currentConnection, message.c_str(), message.length(), 0) == SOCKET_ERROR)
					{
						std::cout << "\n发送回执失败" << std::endl;
					}

				}
				else
				{
					std::cout << "\n登录设备已达上限\n";
					std::string message = "-1";
					if (send(currentConnection, message.c_str(), message.length(), 0) == SOCKET_ERROR)
					{
						std::cout << "\n发送回执失败" << std::endl;
					}

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

