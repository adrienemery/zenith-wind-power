#ifndef POWERINFO_H
#define POWERINFO_H

#include <QDialog>

namespace Ui {
class PowerInfo;
}

class PowerInfo : public QDialog
{
    Q_OBJECT
    
public:
    explicit PowerInfo(QWidget *parent = 0);
    ~PowerInfo();
    
private:
    Ui::PowerInfo *ui;
};

#endif // POWERINFO_H
