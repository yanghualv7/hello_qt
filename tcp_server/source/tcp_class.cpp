#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "tcp_class.h"
#include <iostream>
#include <stdio.h>
#include <process.h>
#include <ws2tcpip.h>
#include <sstream>
#include <atomic>
#include <fstream>
// ���캯��
TcpServer::TcpServer(int maxClientCount, std::string sendFilePath) :clientCount(0),
MaxClientCount(maxClientCount), SendFilePath(sendFilePath)
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
	addr.sin_port = htons(7880);
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

void TcpServer::sendFile(SOCKET clientSocket, const std::string& filePath)
{
	// �����ļ���
	std::string fileName = filePath.substr(filePath.find_last_of("/\\") + 1);
	int fileNameSize = fileName.size();
	// �����ļ�������
	send(clientSocket, reinterpret_cast<const char*>(&fileNameSize), sizeof(fileNameSize), 0);
	// �����ļ���
	send(clientSocket, fileName.c_str(), fileNameSize, 0);

	// ���ļ�
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "�޷����ļ�: " << filePath << std::endl;
		return;
	}

	// ��ȡ�ļ���С
	file.seekg(0, std::ios::end);
	int fileSize = file.tellg();
	file.seekg(0, std::ios::beg);
	// �����ļ���С
	send(clientSocket, reinterpret_cast<const char*>(&fileSize), sizeof(fileSize), 0);

	// �����ļ�����
	char buffer[1024];
	while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0)
	{
		int bytesToSend = file.gcount();
		int bytesSent = send(clientSocket, buffer, bytesToSend, 0);
		if (bytesSent == SOCKET_ERROR)
		{
			std::cerr << "�ļ�����ʧ��" << std::endl;
			break;
		}
	}

	file.close();

	// ���տͻ��˵Ľ����ź�
	char endBuffer[128];
	int bytesReceived = recv(clientSocket, endBuffer, sizeof(endBuffer), 0);
	if (bytesReceived > 0)
	{
		std::string endMessage(endBuffer, bytesReceived);
		if (endMessage == "RECEIVE_COMPLETE")
		{
			std::cout << "�ͻ����ļ��������: " << filePath << std::endl;
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
			std::cout << "\n�յ��ͻ�����Ϣ: " << receivedMessage << std::endl;

			if (receivedMessage == "SEND_FILE")
			{
				sendFile(currentConnection, SendFilePath);
			}
			else
			{
				// ���������߼�
				getClientCount();

				std::cout << "\n��ǰ�豸����" << clientCount << std::endl;
				int laveClientCount = MaxClientCount - clientCount;

				if (MaxClientCount >= clientCount)
				{
					std::string message = std::to_string(laveClientCount);
					if (send(currentConnection, message.c_str(), message.length(), 0) == SOCKET_ERROR)
					{
						std::cout << "\n���ͻ�ִʧ��" << std::endl;
					}

				}
				else
				{
					std::cout << "\n��¼�豸�Ѵ�����\n";
					std::string message = "-1";
					if (send(currentConnection, message.c_str(), message.length(), 0) == SOCKET_ERROR)
					{
						std::cout << "\n���ͻ�ִʧ��" << std::endl;
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

