#pragma once

#include <winsock2.h>
#include <vector>
#include <mutex>
#pragma comment(lib,"ws2_32.lib")

class TcpServer
{
public:
	//����˹��캯��
	TcpServer(int maxClientCount, std::string sendFilePath);
	//�������������
	~TcpServer();
	//����ͻ�������
	void handleConnections();
	//��������
	static void handleRequests(LPVOID);
	int getClientCount() const;
	std::vector<SOCKET> getClientSockets() const;
	void sendFile(SOCKET clientSocket, const std::string& filePath);

private:
	SOCKET TcpSocket;
	std::atomic<int> clientCount;
	std::vector<SOCKET> clientSockets;
	mutable std::mutex clientMutex;
	int MaxClientCount = 2;
	std::string SendFilePath;

	void handleClientRequests(SOCKET currentConnection); // �����Ǿ�̬����

};

