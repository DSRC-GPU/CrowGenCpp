#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphmanager.h"
#include <vector>
#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void simtick();

private:
    Ui::MainWindow *ui;
    QTimer* ticktimer;
    GraphManager gm;
};

#endif // MAINWINDOW_H
