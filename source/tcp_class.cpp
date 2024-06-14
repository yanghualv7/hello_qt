#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "tcp_class.h"
#include <iostream>
#include <stdio.h>
#include <process.h>
#include <ws2tcpip.h>
#include <sstream>
#include <fstream>
#include <vector>


TcpClient::TcpClient()
{
	WSADATA wsa;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		throw std::exception("\n加载winsock动态链接库失败！");

	TcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TcpSocket == INVALID_SOCKET)
		throw std::exception("\n无效套接字！");
}

// 客户端析构函数
TcpClient::~TcpClient()
{
	closesocket(TcpSocket);
	WSACleanup();
}

// 连接到服务器
long TcpClient::Connect(const char* ipDest)
{
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(7880);
	inet_pton(AF_INET, ipDest, &servAddr.sin_addr);

	if (connect(TcpSocket, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

// 发送数据
long TcpClient::SendData(const char* buf, int length)
{
	if (SOCKET_ERROR == send(TcpSocket, buf, length, 0))
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

// 接收数据
long TcpClient::RecvData(char* buf, int length)
{
	int bytesReceived = recv(TcpSocket, buf, length, 0);
	if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)
	{
		return -1;
	}
	else
	{
		buf[bytesReceived] = '\0'; // 以空字符结尾接收数据
		return 0;
	}
}

// 关闭连接
void TcpClient::Close()
{
	closesocket(TcpSocket);
	WSACleanup();
	TcpSocket = INVALID_SOCKET; // 重置套接字以防止重复关闭
}


std::string TcpClient::receiveFile(const std::string& directory)
{
	// 接收文件名长度
	int fileNameSize;
	recv(TcpSocket, reinterpret_cast<char*>(&fileNameSize), sizeof(fileNameSize), 0);

	// 接收文件名
	std::vector<char> fileNameBuffer(fileNameSize);
	recv(TcpSocket, fileNameBuffer.data(), fileNameSize, 0);
	std::string fileName(fileNameBuffer.begin(), fileNameBuffer.end());

	// 创建文件
	std::string filePath = directory + "/" + fileName;
	std::ofstream file(filePath, std::ios::binary);
	if (!file.is_open())
		return u8"无法创建文件";

	// 接收文件大小
	int fileSize;
	recv(TcpSocket, reinterpret_cast<char*>(&fileSize), sizeof(fileSize), 0);

	// 接收文件内容
	char buffer[1024];
	int bytesReceived;
	int totalBytesReceived = 0;
	while (totalBytesReceived < fileSize)
	{
		bytesReceived = recv(TcpSocket, buffer, sizeof(buffer), 0);
		if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)
		{
			file.close();
			return u8"文件接收失败";
		}

		file.write(buffer, bytesReceived);
		totalBytesReceived += bytesReceived;
	}

	file.close();

	// 发送结束信号给服务器
	std::string endMessage = "RECEIVE_COMPLETE";
	send(TcpSocket, endMessage.c_str(), endMessage.length(), 0);

	return u8"文件 " + fileName + u8" 接收完成";
}


