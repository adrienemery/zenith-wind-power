#ifndef CONTROLOPTIONS_H
#define CONTROLOPTIONS_H

#include <QWidget>
#include "controlalgorithm.h"

namespace Ui {
class controlOptions;
}

class controlOptions : public QWidget
{
    Q_OBJECT
    
public:
    explicit controlOptions(QWidget *parent = 0);
    ~controlOptions();

    ControlAlgorithm * getControlAlgorithmHandle(){

        return this->kitecontroller;
    }
    
private slots:
    void on_showTrace_pushButton_clicked();

private:
    Ui::controlOptions *ui;
    ControlAlgorithm* kitecontroller;
};

#endif // CONTROLOPTIONS_H
