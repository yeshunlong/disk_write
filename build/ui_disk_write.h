/********************************************************************************
** Form generated from reading UI file 'disk_write.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISK_WRITE_H
#define UI_DISK_WRITE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_disk_write
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *disk_write)
    {
        if (disk_write->objectName().isEmpty())
            disk_write->setObjectName(QString::fromUtf8("disk_write"));
        disk_write->resize(800, 600);
        centralwidget = new QWidget(disk_write);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        disk_write->setCentralWidget(centralwidget);
        menubar = new QMenuBar(disk_write);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        disk_write->setMenuBar(menubar);
        statusbar = new QStatusBar(disk_write);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        disk_write->setStatusBar(statusbar);

        retranslateUi(disk_write);

        QMetaObject::connectSlotsByName(disk_write);
    } // setupUi

    void retranslateUi(QMainWindow *disk_write)
    {
        disk_write->setWindowTitle(QApplication::translate("disk_write", "disk_write", nullptr));
    } // retranslateUi

};

namespace Ui {
    class disk_write: public Ui_disk_write {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISK_WRITE_H
