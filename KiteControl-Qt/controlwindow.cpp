#include "controlwindow.h"
#include "ui_controlwindow.h"

ControlWindow::ControlWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControlWindow)
{
    ui->setupUi(this);

    controlAlgorithm = new ControlAlgorithm(this);

    ui->webcamCheckbox->setChecked(true);

    height = 480/2;
    width = 640/2;

    Q1 = new QuadrantItem(0,-height,width-2,height-2,1);
    Q2 = new QuadrantItem(-width,-height,width-2,height-2,2);
    Q3 = new QuadrantItem(-width,0,width-2,height-2,3);
    Q4 = new QuadrantItem(0,0,width-2,height-2,4);
    Q5 = new QuadrantItem(-width/2,-height/2,width,height,5);

    scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);

    // initialize pens and brushes
    blackPen.setWidth(3);
    redPen.setWidth(3);

    blackPen.setColor(Qt::black);
    redPen.setColor(Qt::red);

    redBrush.setColor(Qt::red);
    blueBrush.setColor(Qt::blue);
    greenBrush.setColor(Qt::green);
    whiteBrush.setColor(Qt::white);

    // Add items to the scene
    scene->addItem(Q1);
    scene->addItem(Q2);
    scene->addItem(Q3);
    scene->addItem(Q4);
    scene->addItem(Q5);

    startPoint = scene->addEllipse(-50,50,10,10,blackPen,QBrush(Qt::green));
    endPoint = scene->addEllipse(10,-10,10,10,blackPen,QBrush(Qt::red));
    kite = scene->addEllipse(0,0,20,20,blackPen,QBrush(Qt::blue));

    // make all quadrants selectable
    Q1->setFlag(QGraphicsItem::ItemIsSelectable);
    Q2->setFlag(QGraphicsItem::ItemIsSelectable);
    Q3->setFlag(QGraphicsItem::ItemIsSelectable);
    Q4->setFlag(QGraphicsItem::ItemIsSelectable);
    Q5->setFlag(QGraphicsItem::ItemIsSelectable);

    startPoint->setFlag(QGraphicsItem::ItemIsMovable);
    endPoint->setFlag(QGraphicsItem::ItemIsMovable);
    kite->setFlag(QGraphicsItem::ItemIsMovable);

    // initialize control variables
    bTargetVisibility = true;
    bKiteVisibility = true;

}

ControlWindow::~ControlWindow()
{
    delete ui;
}

void ControlWindow::on_showTargetsButton_clicked()
{
    bTargetVisibility = !bTargetVisibility;

    startPoint->setVisible(bTargetVisibility);
    endPoint->setVisible(bTargetVisibility);
}

void ControlWindow::on_showKiteButton_clicked()
{
    bKiteVisibility = !bKiteVisibility;

    kite->setVisible(bKiteVisibility);
}

void ControlWindow::updateGraphics()
{

}

void ControlWindow::on_widthSlider_valueChanged(int value)
{
    Q1->setWidth(value);
    Q2->setWidth(value);
    Q3->setWidth(value);
    Q4->setWidth(value);
    Q5->setWidth(value);
    scene->update();

}

void ControlWindow::on_heightSlider_valueChanged(int value)
{
    Q1->setHeight(value);
    Q2->setHeight(value);
    Q3->setHeight(value);
    Q4->setHeight(value);
    Q5->setHeight(value);
    scene->update();

}

void ControlWindow::on_defaultSizeButton_clicked()
{
    Q1->setWidth(width);
    Q2->setWidth(width);
    Q3->setWidth(width);
    Q4->setWidth(width);
    Q5->setWidth(width);

    Q1->setHeight(height);
    Q2->setHeight(height);
    Q3->setHeight(height);
    Q4->setHeight(height);
    Q5->setHeight(height);

    scene->update();

    ui->widthSlider->setValue(width);
    ui->heightSlider->setValue(height);
}

void ControlWindow::on_imageProcessorButton_clicked()
{
    controlAlgorithm->getImageProcessingHandle()->show();
}


void ControlWindow::on_webcamCheckbox_clicked(bool checked)
{
    if(checked){
        ui->imageProcessorButton->setEnabled(true);
    }else{
        ui->imageProcessorButton->setEnabled(false);

    }
}
