#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QTimer>
#include <QFile>
#include <QFileDialog>
#include <time.h>

#include "paintarea.h"
#include "Graph_List.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    friend class PaintArea;
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void PrepareDraw();//画图前的准备工作
    void ActiveButtons();//激活按钮
    void HideInputButtons();//隐藏手动输入按钮
    void ShowInputButtons();//显示手动输入按钮
    void setData(int c,int w);//设置数据
    ////////////////   对外接口函数    ////////////////
    int get_num_city(){return num_city;}
    int get_num_way(){return num_way;}
    Graph_List *get_graph(){return graph;}

private slots:
    void CreateRand();
    void CreateManu();
    void RemoveRedun();
    void AddRedun();
    void JudgeDepen();
    void ImportantCity();
    void Save();
    void Clear();
    void AddALine();
    void AbandonInput();
    void list_a_line(int,int,bool);

private:
    Ui::MainWindow *ui;
    //窗口组件
    PaintArea *paint;
    //数据;
    int num_city;
    int num_way;
    Graph_List *graph;
};

#endif // MAINWINDOW_H
