#include "addkite.h"
#include "ui_addkite.h"

AddKite::AddKite(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddKite)
{
    ui->setupUi(this);
}

AddKite::~AddKite()
{
    delete ui;
}

void AddKite::on_buttonBox_accepted()
{
    Kite *kite = new Kite(this);
    kite->setName(ui->kiteName->text());
    kite->setSize(ui->kiteSizeSpinBox->value());
    kiteList.push_back(kite);

    emit kiteAdded();
}


