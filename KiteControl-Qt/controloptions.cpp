#include "controloptions.h"
#include "ui_controloptions.h"


controlOptions::controlOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::controlOptions)
{
    ui->setupUi(this);


    kitecontroller = new ControlAlgorithm();

}

controlOptions::~controlOptions()
{
    delete ui;
    delete kitecontroller;
}

void controlOptions::on_showTrace_pushButton_clicked()
{

}
