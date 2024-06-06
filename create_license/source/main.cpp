#include "create_license.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	create_license w;
	w.show();
	return a.exec();
}
