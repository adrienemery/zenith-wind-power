#ifndef ADDKITE_H
#define ADDKITE_H

#include <QDialog>
#include "kite.h"

namespace Ui {
class AddKite;
}

class AddKite : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddKite(QWidget *parent = 0);
    ~AddKite();

    QList<Kite*> kiteList;

    QString name;

    float size;


signals:
    void kiteAdded();

public slots:
    void on_buttonBox_accepted();

private:
    Ui::AddKite *ui;


};

#endif // ADDKITE_H
