#include "OpenglWidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
	OpenglWidget window;
	window.resize(1200, 960);
	window.show();
	return application.exec();
}
