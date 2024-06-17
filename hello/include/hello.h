#ifndef HELLO_H
#define HELLO_H

#include <QMainWindow>
#include <QTimer>

namespace Ui
{
	class helloClass;
}

class hello : public QMainWindow
{
	Q_OBJECT

public:
	explicit hello(const QString& storedEncryptedLicense, const QString& user, QWidget* parent = nullptr);
	~hello();

private slots:
	void writeTimeToFile();

private:
	Ui::helloClass* ui;
	QTimer* timer;
	QString user;
	void loadFile(const QString& filePath, const QString& imagePath, const QString& iniFilePath);
};

#endif // HELLO_H

