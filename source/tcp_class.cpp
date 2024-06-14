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
		throw std::exception("\n����winsock��̬���ӿ�ʧ�ܣ�");

	TcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TcpSocket == INVALID_SOCKET)
		throw std::exception("\n��Ч�׽��֣�");
}

// �ͻ�����������
TcpClient::~TcpClient()
{
	closesocket(TcpSocket);
	WSACleanup();
}

// ���ӵ�������
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

// ��������
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

// ��������
long TcpClient::RecvData(char* buf, int length)
{
	int bytesReceived = recv(TcpSocket, buf, length, 0);
	if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)
	{
		return -1;
	}
	else
	{
		buf[bytesReceived] = '\0'; // �Կ��ַ���β��������
		return 0;
	}
}

// �ر�����
void TcpClient::Close()
{
	closesocket(TcpSocket);
	WSACleanup();
	TcpSocket = INVALID_SOCKET; // �����׽����Է�ֹ�ظ��ر�
}


std::string TcpClient::receiveFile(const std::string& directory)
{
	// �����ļ�������
	int fileNameSize;
	recv(TcpSocket, reinterpret_cast<char*>(&fileNameSize), sizeof(fileNameSize), 0);

	// �����ļ���
	std::vector<char> fileNameBuffer(fileNameSize);
	recv(TcpSocket, fileNameBuffer.data(), fileNameSize, 0);
	std::string fileName(fileNameBuffer.begin(), fileNameBuffer.end());

	// �����ļ�
	std::string filePath = directory + "/" + fileName;
	std::ofstream file(filePath, std::ios::binary);
	if (!file.is_open())
		return u8"�޷������ļ�";

	// �����ļ���С
	int fileSize;
	recv(TcpSocket, reinterpret_cast<char*>(&fileSize), sizeof(fileSize), 0);

	// �����ļ�����
	char buffer[1024];
	int bytesReceived;
	int totalBytesReceived = 0;
	while (totalBytesReceived < fileSize)
	{
		bytesReceived = recv(TcpSocket, buffer, sizeof(buffer), 0);
		if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)
		{
			file.close();
			return u8"�ļ�����ʧ��";
		}

		file.write(buffer, bytesReceived);
		totalBytesReceived += bytesReceived;
	}

	file.close();

	// ���ͽ����źŸ�������
	std::string endMessage = "RECEIVE_COMPLETE";
	send(TcpSocket, endMessage.c_str(), endMessage.length(), 0);

	return u8"�ļ� " + fileName + u8" �������";
}


