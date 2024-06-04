#include "create_license.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QDateTime>

QString encryptLicense(const QString& user, const QString& macAddress)
{
	// 提取 macAddress 中的数字部分
	QString numericMacAddress;
	for (QChar c : macAddress)
	{
		if (c.isDigit())
			numericMacAddress += c;
	}

	QString license = user + ":" + numericMacAddress;
	QByteArray encrypted = license.toUtf8().toBase64();
	return QString(encrypted);
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

			// 检查输入是否为空
			if (user.isEmpty() || macAddress.isEmpty())
			{
				// 为空时直接跳过
				continue;
			}

			// 生成加密后的license
			QString encryptedLicense = encryptLicense(user, macAddress);

			// 创建文件名，使用用户名称作为文件名的一部分
			QString fileName = "License/" + user + ".lic";
			QFile file(fileName);
			if (file.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				QTextStream out(&file);
				out << encryptedLicense;
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
