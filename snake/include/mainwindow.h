#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h" 
#include <QKeyEvent>
#include <QMainWindow>
#include <QTimer>

namespace Ui {
	class mainwindow;
}

class mainwindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit mainwindow(const QString& user, QWidget* parent = 0);
	void writeTimeToFile();
	~mainwindow();

protected:
	void keyPressEvent(QKeyEvent* e);

private slots:
	void slotUpdateScore(int nScore);

private:
	int GetHistoryMaxScore();
	void SaveHistoryMaxScore(int nScore);

private:
	Ui::mainwindow* ui;
	QTimer* timer;
	QString user;

};

#endif // MAINWINDOW_H
