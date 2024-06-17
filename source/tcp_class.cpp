#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "tcp_class.h"
#include <iostream>
#include <stdio.h>
#include <process.h>
#include <ws2tcpip.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <QProgressDialog.h>
#include <QApplication>


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


//std::string TcpClient::receiveFile(const std::string& directory)
//{
//	// 接收文件名长度
//	int fileNameSize;
//	recv(TcpSocket, reinterpret_cast<char*>(&fileNameSize), sizeof(fileNameSize), 0);
//
//	// 接收文件名
//	std::vector<char> fileNameBuffer(fileNameSize);
//	recv(TcpSocket, fileNameBuffer.data(), fileNameSize, 0);
//	std::string fileName(fileNameBuffer.begin(), fileNameBuffer.end());
//
//	// 创建文件
//	std::string filePath = directory + "/" + fileName;
//	std::ofstream file(filePath, std::ios::binary);
//	if (!file.is_open())
//		return u8"无法创建文件";
//
//	// 接收文件大小
//	int fileSize;
//	recv(TcpSocket, reinterpret_cast<char*>(&fileSize), sizeof(fileSize), 0);
//
//	// 接收文件内容
//	char buffer[1024];
//	int bytesReceived;
//	int totalBytesReceived = 0;
//
//	//progress.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);		//不显示进度条上的“最小化”“最大化”“关闭”
//	//progress.setWindowTitle("任务保存");	//窗口标题
//	//progress.setAutoClose(false);		//进度达到最大值时不关闭，默认为true
//	//progress.setLabelText("任务保存中...");	//显示的文本
//	//progress.setRange(0, 100);				//设置进度条的极值，默认为[0,100]
//	//progress.setCancelButton(NULL);			//不显示取消按钮
//	//progress.show();
//	while (totalBytesReceived < fileSize)
//	{
//		bytesReceived = recv(TcpSocket, buffer, sizeof(buffer), 0);
//		if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)
//		{
//			file.close();
//			return u8"文件接收失败";
//		}
//
//		file.write(buffer, bytesReceived);
//		totalBytesReceived += bytesReceived;
//	}
//
//	file.close();
//
//	// 发送结束信号给服务器
//	std::string endMessage = "RECEIVE_COMPLETE";
//	send(TcpSocket, endMessage.c_str(), endMessage.length(), 0);
//
//	return u8"文件 " + fileName + u8" 接收完成";
//}

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

	// 初始化进度条
	QProgressDialog* progress = new QProgressDialog;
	progress->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint); // 不显示进度条上的“最小化”“最大化”“关闭”
	progress->setWindowTitle(u8"任务保存"); // 窗口标题
	progress->setAutoClose(false); // 进度达到最大值时不关闭，默认为true
	progress->setLabelText(u8"任务保存中..."); // 显示的文本
	progress->setRange(0, 100); // 设置进度条的极值，默认为[0,100]
	progress->setCancelButton(nullptr); // 不显示取消按钮
	progress->show();

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

		// 更新进度条
		int progressValue = static_cast<int>((static_cast<double>(totalBytesReceived) / fileSize) * 100);
		progress->setValue(progressValue);

		// 处理事件队列以确保进度条更新
		QCoreApplication::processEvents();

		if (progress->wasCanceled())
		{
			file.close();
			delete progress; // 销毁进度条对象
			return u8"文件接收取消";
		}
	}

	file.close();

	// 发送结束信号给服务器
	std::string endMessage = "RECEIVE_COMPLETE";
	send(TcpSocket, endMessage.c_str(), endMessage.length(), 0);

	delete progress; // 销毁进度条对象

	return u8"文件 " + fileName + u8" 接收完成";
}


