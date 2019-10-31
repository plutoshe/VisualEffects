#include "StableFluidQt.h"
#include "StableFluidWidget.h"

#include <QtWidgets/QApplication>

#include <stdio.h>

void hello1();


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	StableFluidWidget window;
	window.resize(1200, 960);
	window.show();
	return a.exec();
}
