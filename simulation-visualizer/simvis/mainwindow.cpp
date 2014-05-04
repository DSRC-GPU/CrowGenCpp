#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsEllipseItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QGraphicsEllipseItem* e = new QGraphicsEllipseItem(0,0,10,10);
    ui->graphicsView->scene()->addItem(e);
}
