#include "imageprocessing.h"
#include "ui_imageprocessing.h"

ImageProcessing::ImageProcessing(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageProcessing)
{
    ui->setupUi(this);
}

ImageProcessing::~ImageProcessing()
{
    delete ui;
}
