#ifndef HELLO_H
#define HELLO_H

#include <QMainWindow>

namespace Ui
{
	class helloClass;
}

class hello : public QMainWindow
{
	Q_OBJECT

public:
	explicit hello(const QString& user, QWidget* parent = nullptr);
	~hello();

private:
	Ui::helloClass* ui;
	void loadFile(const QString& filePath, const QString& imagePath, const QString& iniFilePath);
};

#endif // HELLO_H

