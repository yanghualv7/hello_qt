#pragma once

#include <winsock2.h>
#include <vector>
#include <mutex>
#pragma comment(lib,"ws2_32.lib")

class TcpServer
{
public:
	//服务端构造函数
	TcpServer(int maxClientCount, std::string sendFilePath);
	//服务端析构函数
	~TcpServer();
	//处理客户端链接
	void handleConnections();
	//处理请求
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

	void handleClientRequests(SOCKET currentConnection); // 新增非静态方法

};

