#include "create_license.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QDateTime>

//Copyright 2022 Yang Y., All rights reserved.
//快速幂模运算代码参考https://blog.csdn.net/Pumpkin_Tung/article/details/121443351
#include<conio.h>
#include<string>
#include "AES.h"
//懒得写密钥扩展了
//这个AES密钥是256个Base64字符组成的的字符串,经过转换变成192个八位二进制数直接作为密钥组,加密总共加密12轮甚至11轮罢。
//密钥用summonAESKey.py生成
std::string key = "1145141919810Shimokita5ZUgHJOK0BHEv6,AH3S,RrkQU58zxG,BLf/sguM9fAocN7xwUfb,v8L8eaTGpYzdy7HTiKqPXj6D6,7ej3NfqoL29Av7ZOWPj5l1HqiVNG60i+g1GF,J+qr3sa+32MZuuiZynOyJwFGP,bFEKFDDGSmb0QrRL3Pl8M3ay68o0inrPBYiz+Btm9fhWt1cQ9TZxTWQ0QQBsMqAIV9mB2Udx1DEaGfZPFpR8c148QnN0T";//AES密钥,256位



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

// 加密/解密函数
string encryptLicense(const QString times, const QString& user, const QString& macAddress)
{
	QString numericMacAddress = extractHexDigits(macAddress);

	QString plaintext = times + "|" + numericMacAddress;
	//QString plaintext = numericMacAddress;

	init();

init:
	cout << "选择工作模式:加密(1)解密(2)退出(0)" << endl;

	string inp = plaintext.toStdString();
	//密文
	string opt = aes(key, 1, inp);
	//明文
	string ss = aes(key, 2, opt);
	cout << "\n明文：" << ss.c_str();

	return opt;

}

// 构造函数和析构函数
create_license::create_license(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	// 连接生成许可证文件按钮的点击信号到槽函数
	connect(ui.toolButton, &QToolButton::clicked, this, &create_license::generateLicenseFile);
	connect(ui.addButton, &QPushButton::clicked, this, &create_license::addRow);

}

create_license::~create_license()
{}

// 槽函数：生成许可证文件
void create_license::generateLicenseFile()
{
	int rowCount = ui.tableWidget->rowCount();

	// 创建目录
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

			// 检查输入是否为空
			if (user.isEmpty() || macAddress.isEmpty())
			{
				// 为空时直接跳过
				continue;
			}

			string Licence = encryptLicense(endDateString, user, macAddress);

			// 创建文件名，使用用户名称作为文件名的一部分
			QString fileName = "License/" + user + ".lic";
			QFile file(fileName);
			if (file.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				QTextStream out(&file);
				//将string 转回 QString
				QString qLicence = QString::fromStdString(Licence);
				out << qLicence;
				file.close();
			}
			else
				QMessageBox::warning(this, u8"错误", u8"无法保存许可证文件：" + fileName);
		}
	}

	QMessageBox::information(this, u8"成功", u8"所有许可证文件已生成！");
}

// 槽函数：添加新行
void create_license::addRow()
{
	int rowCount = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(rowCount);
}
