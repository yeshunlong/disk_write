#include "disk_write.h"

#include <QApplication>
#pragma comment(lib, "user32.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    disk_write w;
    w.show();

    return a.exec();
}