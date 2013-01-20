#include "powerinfo.h"
#include "ui_powerinfo.h"
#include <QMovie>

PowerInfo::PowerInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PowerInfo)
{

    ui->setupUi(this);

    QMovie *movie = new QMovie(":/animations/kiteanimationGIF.gif");
    ui->motorDisplayLabel->setMovie(movie);
    movie->start();
}

PowerInfo::~PowerInfo()
{
    delete ui;
}
