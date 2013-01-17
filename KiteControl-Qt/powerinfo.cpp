#include "powerinfo.h"
#include "ui_powerinfo.h"

PowerInfo::PowerInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PowerInfo)
{
    ui->setupUi(this);
}

PowerInfo::~PowerInfo()
{
    delete ui;
}
