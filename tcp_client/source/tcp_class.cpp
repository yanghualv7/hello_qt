#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "tcp_class.h"
#include <iostream>
#include <stdio.h>
#include <process.h>
#include <ws2tcpip.h>
#include <sstream>

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
	servAddr.sin_port = htons(6000);
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
