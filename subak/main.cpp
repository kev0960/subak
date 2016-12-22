#include "subak.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	subak w;
	w.show();
	return a.exec();
}
