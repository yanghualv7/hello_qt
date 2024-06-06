#ifndef CREATE_LICENSE_H
#define CREATE_LICENSE_H

#include <QtWidgets/QMainWindow>
#include "ui_create_license.h"

class create_license : public QMainWindow
{
	Q_OBJECT

public:
	Ui::create_licenseClass ui;

	create_license(QWidget* parent = nullptr);
	void addRow();
	~create_license();

private slots:
	void generateLicenseFile();

};

#endif // CREATE_LICENSE_H