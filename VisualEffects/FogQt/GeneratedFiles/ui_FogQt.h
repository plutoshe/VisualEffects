/********************************************************************************
** Form generated from reading UI file 'FogQt.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FOGQT_H
#define UI_FOGQT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FogQtClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FogQtClass)
    {
        if (FogQtClass->objectName().isEmpty())
            FogQtClass->setObjectName(QString::fromUtf8("FogQtClass"));
        FogQtClass->resize(600, 400);
        menuBar = new QMenuBar(FogQtClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        FogQtClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FogQtClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        FogQtClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(FogQtClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        FogQtClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(FogQtClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        FogQtClass->setStatusBar(statusBar);

        retranslateUi(FogQtClass);

        QMetaObject::connectSlotsByName(FogQtClass);
    } // setupUi

    void retranslateUi(QMainWindow *FogQtClass)
    {
        FogQtClass->setWindowTitle(QCoreApplication::translate("FogQtClass", "FogQt", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FogQtClass: public Ui_FogQtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FOGQT_H
