#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "tcp_class.h"
#include <iostream>
#include <stdio.h>
#include <process.h>
#include <ws2tcpip.h>
#include <sstream>
#include <atomic>
// ���캯��
TcpServer::TcpServer(int maxClientCount) :clientCount(0), MaxClientCount(maxClientCount)
{
	WSADATA wsadata;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsadata))
	{
		std::cout << "\n�׽��ִ�ʧ��" << std::endl;
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

// ��������
TcpServer::~TcpServer()
{
	closesocket(TcpSocket);
	WSACleanup();
}

// ����ͻ�������
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
			_beginthread(TcpServer::handleRequests, 0, this); // ���ݵ�ǰ����ָ��
		}
	}
}

void TcpServer::handleRequests(LPVOID param)
{
	TcpServer* server = static_cast<TcpServer*>(param);
	SOCKET currentConnection = server->clientSockets.back(); // ��ȡ��ǰ���ӵ�socket
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
			closesocket(currentConnection); // �Ͽ�����
			{
				std::lock_guard<std::mutex> lock(clientMutex);
				clientCount--;
				clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), currentConnection), clientSockets.end());
			}
			return;
		}
		else
		{
			buf[bytesReceived] = '\0'; // �Կ��ַ���β��������

			std::cout << "\n�յ��ͻ�����Ϣ: " << buf << std::endl;

			getClientCount();

			std::cout << "\n��ǰ�豸����" << clientCount << std::endl;
			int laveClientCount = MaxClientCount - clientCount;

			if (MaxClientCount >= clientCount)
			{
				// ������������Ϣ
				std::string message = std::to_string(laveClientCount);
				// ���ͻ�ִ
				if (send(currentConnection, message.c_str(), message.length(), 0) == SOCKET_ERROR)
				{
					std::cout << "\n���ͻ�ִʧ��" << std::endl;
				}

			}
			else
			{
				std::cout << "\n��¼�豸�Ѵ�����\n";				// ������������Ϣ
				std::string message = "-1";
				// ���ͻ�ִ
				if (send(currentConnection, message.c_str(), message.length(), 0) == SOCKET_ERROR)
				{
					std::cout << "\n���ͻ�ִʧ��" << std::endl;
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

