#ifndef SUBAK_H
#define SUBAK_H

#include <QtWidgets/QMainWindow>
#include "ui_subak.h"

class subak : public QMainWindow
{
	Q_OBJECT

public:
	subak(QWidget *parent = 0);
	~subak();

private:
	Ui::subakClass ui;
};

#endif // SUBAK_H
