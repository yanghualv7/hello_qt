#include "create_license.h"
#include <QtWidgets/QApplication>
#include <QDir>

int main(int argc, char* argv[])
{
	// ��ȡ��ִ���ļ�����Ŀ¼
	QString exeDir = QCoreApplication::applicationDirPath();

	// ����ǰ����Ŀ¼����Ϊ��ִ���ļ�����Ŀ¼
	QDir::setCurrent(exeDir);

	QApplication a(argc, argv);
	create_license w;
	w.show();
	return a.exec();
}
