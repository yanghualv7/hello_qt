#include "MyWidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);
	MyWidget widget;
	widget.resize(400, 400);
	widget.setWindowTitle(u8"功能主界面");
	widget.show();
	return app.exec();
}
