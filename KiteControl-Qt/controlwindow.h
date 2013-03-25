#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QPen>
#include <QtCore>
#include "controlalgorithm.h"
#include "quadrantitem.h"
#include "targetpointitem.h"

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

    void on_widthSlider_valueChanged(int value);

    void on_heightSlider_valueChanged(int value);

    void on_defaultSizeButton_clicked();

    void on_imageProcessorButton_clicked();

    void on_webcamCheckbox_clicked(bool checked);

    void on_numTargetsSpinBox_valueChanged(int arg1);

    void on_targetDiameterSlider_valueChanged(int value);

private:
    Ui::ControlWindow *ui;

    ControlAlgorithm *controlAlgorithm;
    ImageProcessing *imageProcessing;
    KiteColorTracker *kiteColorTracker;

    // Graphics objects
    QGraphicsScene *scene;
    QGraphicsEllipseItem *kite;
    QuadrantItem *Q1,*Q2,*Q3,*Q4,*Q5;
    QList<TargetPointItem*> targets;

    QTimer *timer;

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
