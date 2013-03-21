#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QPen>
#include <QtCore>
#include "controlalgorithm.h"

namespace Ui {
class ControlWindow;
}

class ControlWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit ControlWindow(QWidget *parent = 0);
    ~ControlWindow();
    
private slots:
    void on_showTargetsButton_clicked();

    void on_showKiteButton_clicked();

    void updateGraphics();

private:
    Ui::ControlWindow *ui;

    ControlAlgorithm *controlAlgorithm;

    // Graphics objects
    QGraphicsScene *scene;
    QGraphicsEllipseItem *startPoint,*endPoint;
    QGraphicsEllipseItem *kite;
    QGraphicsRectItem *Q1,*Q2,*Q3,*Q4,*Q5;

    int height,width;

    // Control variables
    bool bTargetVisibility;
    bool bKiteVisibility;

    // Brushes
    QBrush redBrush;
    QBrush blueBrush;
    QBrush greenBrush;
    QBrush whiteBrush;

    // Pens
    QPen blackPen;
    QPen redPen;

};

#endif // CONTROLWINDOW_H
