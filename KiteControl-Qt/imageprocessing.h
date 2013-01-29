#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QDialog>

namespace Ui {
class ImageProcessing;
}

class ImageProcessing : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImageProcessing(QWidget *parent = 0);
    ~ImageProcessing();
    
private:
    Ui::ImageProcessing *ui;
};

#endif // IMAGEPROCESSING_H
