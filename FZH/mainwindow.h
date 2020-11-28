#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QPushButton"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QTimer>
#include "MineMap.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    CMineMap mineMap;  //定义类
    int offsetx,offsety;
    QTimer *rumtime; //计时器实现
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *);
protected:
    void mousePressEvent(QMouseEvent *event);
   // void mouseMoveEvent(QMouseEvent *event);

private slots:  //槽函数

    void on_actionRestart_triggered();
    void on_actionExit_triggered();

    void on_actionPrimary_triggered();

    void on_actionMedium_triggered();

    void on_actionSenior_triggered();

    void on_sectime();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
