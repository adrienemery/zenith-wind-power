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


void ImageProcessing::on_hMinSlider_valueChanged(int value)
{

}

void ImageProcessing::on_sMinSlider_valueChanged(int value)
{

}

void ImageProcessing::on_vMinSlider_valueChanged(int value)
{

}

void ImageProcessing::on_hMaxSlider_valueChanged(int value)
{

}

void ImageProcessing::on_sMaxSlider_valueChanged(int value)
{

}

void ImageProcessing::on_vMaxSlider_valueChanged(int value)
{

}
