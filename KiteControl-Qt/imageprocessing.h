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
    
private slots:


private:
    Ui::ImageProcessing *ui;

    KiteColorTracker *colorTracker;

};

#endif // IMAGEPROCESSING_H
