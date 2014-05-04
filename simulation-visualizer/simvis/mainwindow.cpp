#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../crowdgeneration/CrowGenCpp/src/CrowdParser.hpp"

#include <QGraphicsEllipseItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene);

    CrowdParser cp;
    cp.parseFile("default_sim_out.xml", simulation);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    static QGraphicsEllipseItem* e = new QGraphicsEllipseItem(0,0,10,10);
    static QGraphicsEllipseItem* d = new QGraphicsEllipseItem(0,0,10,10);
    static QGraphicsLineItem* f = new QGraphicsLineItem(0,0,100,100);
    static bool first = true;
    if (first)
    {
        ui->graphicsView->scene()->addItem(e);
        ui->graphicsView->scene()->addItem(d);
        ui->graphicsView->scene()->addItem(f);
        first = false;
    }
    else
    {
        e->moveBy(5,5);
        d->moveBy(-5,5);
        f->setLine(e->x(), e->y(), d->x(), d->y());
    }
}
