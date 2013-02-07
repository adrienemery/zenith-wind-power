#include "imageprocessing.h"
#include "ui_imageprocessing.h"

ImageProcessing::ImageProcessing(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageProcessing)
{
    ui->setupUi(this);

    colorTracker = new KiteColorTracker(this);

    connect(colorTracker,SIGNAL(writeToArduino(QString)),this,SLOT(reciveArduinoCommand(QString)) );

    //init labels at startup.
    ui->hmin_label->setNum(colorTracker->getHmin());
    ui->hmax_label->setNum(colorTracker->getHmax());
    ui->smin_label->setNum(colorTracker->getSmin());
    ui->smax_label->setNum(colorTracker->getSmax());
    ui->vmin_label->setNum(colorTracker->getVmin());
    ui->vmax_label->setNum(colorTracker->getVmax());
    ui->erode_label->setNum(colorTracker->getErodeSize());
    ui->dilate_label->setNum(colorTracker->getDilateSize());

    ui->hMinSlider->setValue(colorTracker->getHmin());
    ui->hMaxSlider->setValue(colorTracker->getHmax());
    ui->sMinSlider->setValue(colorTracker->getSmin());
    ui->sMaxSlider->setValue(colorTracker->getSmax());
    ui->vMinSlider->setValue(colorTracker->getVmin());
    ui->vMaxSlider->setValue(colorTracker->getVmax());
    ui->erode_dial->setValue(colorTracker->getErodeSize());
    ui->dilate_dial->setValue(colorTracker->getDilateSize());
    ui->minArea_spinBox->setValue(colorTracker->getMinArea());
    ui->maxArea_spinbox->setValue(colorTracker->getMaxArea());


}

ImageProcessing::~ImageProcessing()
{
    delete ui;

    //save filter data
    colorTracker->save("filterData.txt");
        colorTracker->cleanup();
        delete colorTracker;


}

void ImageProcessing::reciveArduinoCommand(QString cmd){

    //don't shoot the messenger!
    emit writeToArduino(cmd);
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

void ImageProcessing::on_startStream_pushButton_clicked()
{
    //begin video capture
    colorTracker->beginCapture("camera");
    if(colorTracker->isPaused()){
        on_playPause_pushButton_clicked();
    }else{
        ui->playPause_pushButton->setIcon(QIcon(":/images/icons/png/64x64/pause.png"));
    }


}

void ImageProcessing::on_endStream_pushButton_clicked()
{
    //end video capture
    colorTracker->endCapture();
}

void ImageProcessing::on_erode_dial_valueChanged(int value)
{

    colorTracker->setErodeSize(value);

}

void ImageProcessing::on_dilate_dial_valueChanged(int value)
{

    colorTracker->setDilateSize(value);
}

void ImageProcessing::on_erode_dial_sliderPressed()
{
    //show dilate and erode frames
    colorTracker->setEDflag(true);
}


void ImageProcessing::on_erode_dial_sliderReleased()
{
    //don't show dilate and erode frames
    colorTracker->setEDflag(false);


}





void ImageProcessing::on_minArea_spinBox_valueChanged(int arg1)
{
    colorTracker->setMinArea(arg1);

}

void ImageProcessing::on_maxArea_spinbox_valueChanged(int arg1)
{
    colorTracker->setMaxArea(arg1);

}

void ImageProcessing::on_pushButton_clicked()
{
    colorTracker->beginCapture("movie");

}

void ImageProcessing::on_playPause_pushButton_clicked()
{
    colorTracker->playPause();

    if(colorTracker->isPaused()){
        ui->playPause_pushButton->setIcon(QIcon(":/images/icons/png/64x64/play.png"));
    }else{
        ui->playPause_pushButton->setIcon(QIcon(":/images/icons/png/64x64/pause.png"));
    }

}

void ImageProcessing::on_pushButton_2_clicked()
{
    colorTracker->setRFIflag();

}


void ImageProcessing::on_trackKite_pushButton_clicked()
{
    colorTracker->toggleTrackKite();
}

void ImageProcessing::on_propGain_spinBox_valueChanged(int arg1)
{
    colorTracker->setPropGain(arg1);
}

void ImageProcessing::on_errorBoundSlider_valueChanged(int value)
{
    colorTracker->setMinErrorX(value);
    colorTracker->setMinErrorY(value);
}
