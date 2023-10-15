#pragma once
#include "ui_disk_write.h"
#include <QMainWindow>

class disk_write : public QMainWindow {
    Q_OBJECT
    
public:
    disk_write(QWidget* parent = nullptr);
    ~disk_write();

    void speed_test();

private:
    Ui_disk_write* ui;
};