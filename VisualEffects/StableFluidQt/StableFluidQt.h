#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_StableFluidQt.h"

class StableFluidQt : public QMainWindow
{
	Q_OBJECT

public:
	StableFluidQt(QWidget *parent = Q_NULLPTR);

private:
	Ui::StableFluidQtClass ui;
};
