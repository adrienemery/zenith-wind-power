#include "imageprocessing.h"
#include "ui_imageprocessing.h"

ImageProcessing::ImageProcessing(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageProcessing)
{
    ui->setupUi(this);
    colorTracker = new KiteColorTracker(this);

    //init labels at startup.
    ui->hmin_label->setNum(colorTracker->getHmin());
    ui->hmax_label->setNum(colorTracker->getHmax());
    ui->smin_label->setNum(colorTracker->getSmin());
    ui->smax_label->setNum(colorTracker->getSmax());
    ui->vmin_label->setNum(colorTracker->getVmin());
    ui->vmax_label->setNum(colorTracker->getVmax());
}

ImageProcessing::~ImageProcessing()
{
    delete ui;
    colorTracker->cleanup();
    delete colorTracker;

}


void ImageProcessing::on_hMinSlider_valueChanged(int value)
{

    colorTracker->setHmin(value);

}

void ImageProcessing::on_sMinSlider_valueChanged(int value)
{
    colorTracker->setSmin(value);
}

void ImageProcessing::on_vMinSlider_valueChanged(int value)
{
    colorTracker->setVmin(value);
}

void ImageProcessing::on_hMaxSlider_valueChanged(int value)
{
    colorTracker->setHmax(value);
}

void ImageProcessing::on_sMaxSlider_valueChanged(int value)
{
    colorTracker->setSmax(value);
}

void ImageProcessing::on_vMaxSlider_valueChanged(int value)
{
    colorTracker->setVmax(value);
}
