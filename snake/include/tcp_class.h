#pragma once

#include <winsock2.h>
#include <iostream>
#include <string>
#pragma comment(lib,"ws2_32.lib")

class TcpClient
{

public:
	//客户端构造函数
	TcpClient();
	//客户端析构函数
	~TcpClient();

	/*
		函数名称：Connect
		参数：cIP输入ip地址，iPort是客户端链接的端口
		返回：返回0成功，-1失败
		功能：客户端与服务器进行连接
	*/
	long Connect(const char* cIP);

	/*
		函数名称：SendData
		参数：buf是存储要发送数据，length为数组的长度
		返回：返回0成功，-1失败
		功能：发送数据
	*/
	long SendData(const char* buf, int length);

	/*
		函数名称：RecvData
		参数：buf是存储要发送数据数组，length为数组的长度
		返回：返回0成功，-1失败
		功能：接受数据
	*/
	long RecvData(char* buf, int length);

	/*
		函数名称：receiveFile
		参数：filePath是存储接收文件的目录
		返回：返回接收数据是否成功的信息
		功能：接受数据
	*/
	std::string receiveFile(const std::string& filePath);

	/*
	函数名称：Close
	参数：无
	返回：无
	功能：关闭连接
	*/
	void Close();

private:
	SOCKET TcpSocket;//tcp套接字    //连接的端口
};

