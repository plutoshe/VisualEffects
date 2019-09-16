#include "OpenglWidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OpenglWidget w;
	w.resize(800, 600);
	w.show();
	return a.exec();
}
