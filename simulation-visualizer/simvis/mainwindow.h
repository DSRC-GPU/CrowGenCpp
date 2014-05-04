#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../../crowdgeneration/CrowGenCpp/src/Vertex.hpp"
#include <vector>
#include <QMainWindow>

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
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<std::vector<Vertex> > simulation;
};

#endif // MAINWINDOW_H
