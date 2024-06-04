#include "create_license.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QDateTime>

QString encryptLicense(const QString& user, const QString& macAddress)
{
	// ��ȡ macAddress �е����ֲ���
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

			// ��������Ƿ�Ϊ��
			if (user.isEmpty() || macAddress.isEmpty())
			{
				// Ϊ��ʱֱ������
				continue;
			}

			// ���ɼ��ܺ��license
			QString encryptedLicense = encryptLicense(user, macAddress);

			// �����ļ�����ʹ���û�������Ϊ�ļ�����һ����
			QString fileName = "License/" + user + ".lic";
			QFile file(fileName);
			if (file.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				QTextStream out(&file);
				out << encryptedLicense;
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
