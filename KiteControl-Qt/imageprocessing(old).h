#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QMainWindow>
#include "kitecolortracker.h"

namespace Ui {
class ImageProcessing;
}

class ImageProcessing : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ImageProcessing(QWidget *parent = 0);
    ~ImageProcessing();

private slots:

    void on_hMinSlider_valueChanged(int value);

    void on_sMinSlider_valueChanged(int value);

    void on_vMinSlider_valueChanged(int value);

    void on_hMaxSlider_valueChanged(int value);

    void on_sMaxSlider_valueChanged(int value);

    void on_vMaxSlider_valueChanged(int value);


private slots:



private:
    Ui::ImageProcessing *ui;

    KiteColorTracker *colorTracker;

    int testnum;

};

#endif // IMAGEPROCESSING_H