#pragma once

#include <winsock2.h>
#include <vector>
#include <mutex>
#pragma comment(lib,"ws2_32.lib")

class TcpServer
{
public:
	//����˹��캯��
	TcpServer(int maxClientCount);
	//�������������
	~TcpServer();
	//����ͻ�������
	void handleConnections();
	//��������
	static void handleRequests(LPVOID);
	int getClientCount() const;
	std::vector<SOCKET> getClientSockets() const;

private:
	SOCKET TcpSocket;
	std::atomic<int> clientCount;
	std::vector<SOCKET> clientSockets;
	mutable std::mutex clientMutex;
	int MaxClientCount = 2;

	void handleClientRequests(SOCKET currentConnection); // �����Ǿ�̬����

};

