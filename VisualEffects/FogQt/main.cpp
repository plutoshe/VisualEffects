#include "OpenglWidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OpenglWidget w;
	w.show();
	return a.exec();
}
