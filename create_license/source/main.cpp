#include "create_license.h"
#include <QtWidgets/QApplication>
#include <QDir>

int main(int argc, char* argv[])
{
	// 获取可执行文件所在目录
	QString exeDir = QCoreApplication::applicationDirPath();

	// 将当前工作目录设置为可执行文件所在目录
	QDir::setCurrent(exeDir);

	QApplication a(argc, argv);
	create_license w;
	w.show();
	return a.exec();
}
