#ifndef HELLO_H
#define HELLO_H

#include <QtWidgets/QMainWindow>
#include "ui_hello.h"
#include <QTextBrowser>
#include <QSettings>

namespace Ui {
	class helloClass;
}

class hello : public QMainWindow
{
	Q_OBJECT

public:
	hello(QWidget* parent = nullptr);
	~hello();

private:
	Ui::helloClass* ui;
	QTextBrowser* textBrowser;

	void loadFile(const QString& filePath, const QString& imagePath, const QString& iniFilePath);
	QString getFilePathFromIni(const QString& iniFilePath, const QString& key);
};
#endif
