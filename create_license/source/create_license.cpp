#include "create_license.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QDateTime>

//Copyright 2022 Yang Y., All rights reserved.
//������ģ�������ο�https://blog.csdn.net/Pumpkin_Tung/article/details/121443351
#include<conio.h>
#include<string>
#include "AES.h"
//����д��Կ��չ��
//���AES��Կ��256��Base64�ַ���ɵĵ��ַ���,����ת�����192����λ��������ֱ����Ϊ��Կ��,�����ܹ�����12������11�ְա�
//��Կ��summonAESKey.py����
std::string key = "1145141919810Shimokita5ZUgHJOK0BHEv6,AH3S,RrkQU58zxG,BLf/sguM9fAocN7xwUfb,v8L8eaTGpYzdy7HTiKqPXj6D6,7ej3NfqoL29Av7ZOWPj5l1HqiVNG60i+g1GF,J+qr3sa+32MZuuiZynOyJwFGP,bFEKFDDGSmb0QrRL3Pl8M3ay68o0inrPBYiz+Btm9fhWt1cQ9TZxTWQ0QQBsMqAIV9mB2Udx1DEaGfZPFpR8c148QnN0T";//AES��Կ,256λ



QString extractHexDigits(const QString& input)
{
	QString result;
	QRegularExpression hexDigitRegex("[0-9A-Fa-f]");
	QRegularExpressionMatchIterator i = hexDigitRegex.globalMatch(input);
	while (i.hasNext()) {
		QRegularExpressionMatch match = i.next();
		result += match.captured(0);
	}
	return result;
}

// ����/���ܺ���
string encryptLicense(const QString times, const QString& user, const QString& macAddress)
{
	QString numericMacAddress = extractHexDigits(macAddress);

	QString plaintext = times + "|" + numericMacAddress;
	//QString plaintext = numericMacAddress;

	init();

init:
	cout << "ѡ����ģʽ:����(1)����(2)�˳�(0)" << endl;

	string inp = plaintext.toStdString();
	//����
	string opt = aes(key, 1, inp);
	//����
	string ss = aes(key, 2, opt);
	cout << "\n���ģ�" << ss.c_str();

	return opt;

}

// ���캯������������
create_license::create_license(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	// �����������֤�ļ���ť�ĵ���źŵ��ۺ���
	connect(ui.toolButton, &QToolButton::clicked, this, &create_license::generateLicenseFile);
	connect(ui.addButton, &QPushButton::clicked, this, &create_license::addRow);

}

create_license::~create_license()
{}

// �ۺ������������֤�ļ�
void create_license::generateLicenseFile()
{
	int rowCount = ui.tableWidget->rowCount();

	// ����Ŀ¼
	QDir dir("License");
	if (!dir.exists())
		dir.mkpath(".");

	for (int row = 0; row < rowCount; ++row)
	{
		QTableWidgetItem* userItem = ui.tableWidget->item(row, 0);
		QTableWidgetItem* macItem = ui.tableWidget->item(row, 1);

		if (userItem && macItem)
		{
			QString user = userItem->text();
			QString macAddress = macItem->text();
			QDate endDate = ui.end_Time->date();
			QString endDateString = endDate.toString("yyyyMMdd");

			// ��������Ƿ�Ϊ��
			if (user.isEmpty() || macAddress.isEmpty())
			{
				// Ϊ��ʱֱ������
				continue;
			}

			string Licence = encryptLicense(endDateString, user, macAddress);

			// �����ļ�����ʹ���û�������Ϊ�ļ�����һ����
			QString fileName = "License/" + user + ".lic";
			QFile file(fileName);
			if (file.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				QTextStream out(&file);
				//��string ת�� QString
				QString qLicence = QString::fromStdString(Licence);
				out << qLicence;
				file.close();
			}
			else
				QMessageBox::warning(this, u8"����", u8"�޷��������֤�ļ���" + fileName);
		}
	}

	QMessageBox::information(this, u8"�ɹ�", u8"�������֤�ļ������ɣ�");
}

// �ۺ������������
void create_license::addRow()
{
	int rowCount = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(rowCount);
}
