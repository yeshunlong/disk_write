#include "disk_write.h"
#include "write_func.h"

disk_write::disk_write(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_disk_write)
{
    ui->setupUi(this);
    speed_test();
}

disk_write::~disk_write()
{
    delete ui; 
}

void disk_write::speed_test(){
    double writeSpeed = 0.0;
    writeSpeed = write_using_textstream_to_file();
    ui->label_10->setText(QString::number(writeSpeed) + " MB/s");
    writeSpeed = write_using_writefile();
    ui->label_11->setText(QString::number(writeSpeed) + " MB/s");
    writeSpeed = write_using_fwrite();
    ui->label_12->setText(QString::number(writeSpeed) + " MB/s");
    writeSpeed = write_using_fwrite_diff_chunksize();
    ui->label_13->setText(QString::number(writeSpeed) + " MB/s");
    writeSpeed = write_using_memory_map();
    ui->label_14->setText(QString::number(writeSpeed) + " MB/s");
    writeSpeed = write_using_overlap_io();
    ui->label_15->setText(QString::number(writeSpeed) + " MB/s");
    writeSpeed = write_using_buffer_manager();
    ui->label_16->setText(QString::number(writeSpeed) + " MB/s");
    writeSpeed = write_using_multiple_threads();
    ui->label_17->setText(QString::number(writeSpeed) + " MB/s");
    writeSpeed = write_using_semaphore();
    ui->label_18->setText(QString::number(writeSpeed) + " MB/s");
}