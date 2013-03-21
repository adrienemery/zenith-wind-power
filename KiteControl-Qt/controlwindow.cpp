#include "controlwindow.h"
#include "ui_controlwindow.h"

ControlWindow::ControlWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControlWindow)
{
    ui->setupUi(this);

    controlAlgorithm = new ControlAlgorithm(this);

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

    height = 480/2;
    width = 640/2;

    // Add items to the scene
    Q1 = scene->addRect(0,-height,width-2,height-2,blackPen);
    Q2 = scene->addRect(-width,-height,width-2,height-2,blackPen);
    Q3 = scene->addRect(-width,0,width-2,height-2,blackPen);
    Q4 = scene->addRect(0,0,width-2,height-2,blackPen);
    Q5 = scene->addRect(-width/2,-height/2,width,height,blackPen,QBrush(Qt::white)); //white brush paints over other rects

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

    connect(scene,SIGNAL(selectionChanged()),this,SLOT(updateGraphics()));

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
    Q1->setPen(blackPen);
    Q2->setPen(blackPen);
    Q3->setPen(blackPen);
    Q4->setPen(blackPen);
    Q5->setPen(blackPen);

    if(Q1->isSelected()){
        Q1->setPen(redPen);
    }else if(Q2->isSelected()){
        Q2->setPen(redPen);
    }else if(Q3->isSelected()){
        Q3->setPen(redPen);
    }else if(Q4->isSelected()){
        Q4->setPen(redPen);
    }else if(Q5->isSelected()){
        Q5->setPen(redPen);
    }
}
