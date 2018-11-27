#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QPainter>
#include <QDebug>

#include "Graph_List.h"

class PaintArea : public QWidget
{
    friend class mainwindow;
    Q_OBJECT

public:
    PaintArea(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);

    void init();
    void DrawALine(int start, int end,bool isRedun);
    void DrawPoint();
    void draw_a_line_complete(int,int,bool);
    void draw_rand(Graph_List *);
    void draw_manu(Graph_List *gra);
    void RemoveRedun();
    void AddRedun();
    void SetshowImor(bool);
    bool GetshowImor(){return showImpor;}


private slots:



private:
    QTimer *timer;
    bool NoData;
    bool isRand;
    bool showRedun;
    bool drawComplete;
    bool showImpor;

    Graph_List *graph;

    //

signals:
    void list_a_line(int,int,bool);
};

#endif // PAINTAREA_H
