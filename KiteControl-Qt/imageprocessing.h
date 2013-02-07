#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QDialog>
#include "kitecolortracker.h"

namespace Ui {
class ImageProcessing;
}

class ImageProcessing : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImageProcessing(QWidget *parent = 0);
    ~ImageProcessing();
signals:

     void writeToArduino(QString);


private slots:




    void reciveArduinoCommand(QString);

    void on_hMinSlider_valueChanged(int value);

    void on_sMinSlider_valueChanged(int value);

    void on_vMinSlider_valueChanged(int value);

    void on_hMaxSlider_valueChanged(int value);

    void on_sMaxSlider_valueChanged(int value);

    void on_vMaxSlider_valueChanged(int value);



private slots:



    void on_startStream_pushButton_clicked();

    void on_endStream_pushButton_clicked();

    void on_erode_dial_valueChanged(int value);

    void on_dilate_dial_valueChanged(int value);

    void on_erode_dial_sliderPressed();

    void on_erode_dial_sliderReleased();


    void on_minArea_spinBox_valueChanged(int arg1);

    void on_maxArea_spinbox_valueChanged(int arg1);

    void on_pushButton_clicked();

    void on_playPause_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ImageProcessing *ui;

    KiteColorTracker *colorTracker;

};

#endif // IMAGEPROCESSING_H
