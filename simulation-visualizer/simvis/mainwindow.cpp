#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsEllipseItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGraphicsScene* gscene = new QGraphicsScene();
    ui->graphicsView->setScene(gscene);

    gm.init(ui->graphicsView->scene());
    gm.addVertices("default_sim_out.xml");
    gm.addEdges("default_graph_out.gexf");

    ui->graphicsView->fitInView(gscene->sceneRect(), Qt::KeepAspectRatio);

    ticktimer = new QTimer(this);
    connect(ticktimer, SIGNAL(timeout()), this, SLOT(simtick()));
    ticktimer->start(100);
    ticktimer->setInterval(100);
}

MainWindow::~MainWindow()
{
    delete ui->graphicsView->scene();
    delete ui;
}

void MainWindow::simtick()
{
    static unsigned int i = 0;
    gm.drawTick(i++);
}
