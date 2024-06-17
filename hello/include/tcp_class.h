#pragma once

#include <winsock2.h>
#include <iostream>
#include <string>
#pragma comment(lib,"ws2_32.lib")

class TcpClient
{

public:
	//�ͻ��˹��캯��
	TcpClient();
	//�ͻ�����������
	~TcpClient();

	/*
		�������ƣ�Connect
		������cIP����ip��ַ��iPort�ǿͻ������ӵĶ˿�
		���أ�����0�ɹ���-1ʧ��
		���ܣ��ͻ������������������
	*/
	long Connect(const char* cIP);

	/*
		�������ƣ�SendData
		������buf�Ǵ洢Ҫ�������ݣ�lengthΪ����ĳ���
		���أ�����0�ɹ���-1ʧ��
		���ܣ���������
	*/
	long SendData(const char* buf, int length);

	/*
		�������ƣ�RecvData
		������buf�Ǵ洢Ҫ�����������飬lengthΪ����ĳ���
		���أ�����0�ɹ���-1ʧ��
		���ܣ���������
	*/
	long RecvData(char* buf, int length);

	/*
		�������ƣ�receiveFile
		������filePath�Ǵ洢�����ļ���Ŀ¼
		���أ����ؽ��������Ƿ�ɹ�����Ϣ
		���ܣ���������
	*/
	std::string receiveFile(const std::string& filePath);

	/*
	�������ƣ�Close
	��������
	���أ���
	���ܣ��ر�����
	*/
	void Close();

private:
	SOCKET TcpSocket;//tcp�׽���    //���ӵĶ˿�
};

