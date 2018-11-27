#include "mainwindow.h"
#include "paintarea.h"


PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{

    QPalette p = palette();
    p.setColor(QPalette::Window,Qt::white);
    setPalette(p);
    setAutoFillBackground(true);
    isRand = false;
    NoData = true;
    drawComplete = false;
    showImpor = false;
    graph = NULL;
}

void PaintArea::paintEvent(QPaintEvent *)
{
    qDebug()<<"draw begin"<<this->width()<<this->height();
    if(NoData) return;

    Vertex *Head = graph->GetHeadList();
    for(int i=0;i<graph->NumberOfVertices();i++)
    {
        Edge *p = Head[i].adjacent;
        while(p)
        {
            if(!p->isRepeat)
            {
                qDebug()<<i<<","<<p->VerAdj;
                DrawALine(i,p->VerAdj,p->isRedun);
                if(!drawComplete&&!p->isPrint) {
                    draw_a_line_complete(i+1,p->VerAdj+1,p->isRedun);
                    p->isPrint = true;
                }
            }
            p = p->link;
        }
    }

    DrawPoint();
    qDebug()<<"draw complete";
}

///////////////////  public函数  ////////////////////////

void PaintArea::DrawALine(int start, int end, bool isRedun)
{
    Vertex *Head = graph->GetHeadList();
    QPainter *paint = new QPainter;
    paint->begin(this);
    paint->setRenderHint(QPainter::Antialiasing);
    QPen pen;
    if(isRedun) pen=QPen(Qt::blue,2,Qt::DashLine);
    else        pen=QPen(Qt::red,2,Qt::SolidLine);
    paint->setPen(pen);
    if(!showRedun&&isRedun) return;
    paint->drawLine(Head[start].x+10,Head[start].y+10,Head[end].x+10,Head[end].y+10);
    paint->end();
}

void PaintArea::DrawPoint()
{
    Vertex *Head = graph->GetHeadList();
    QPainter *paint = new QPainter;
    paint->begin(this);
    paint->setRenderHint(QPainter::Antialiasing);
    paint->setPen(Qt::black);
    for(int i=0;i<graph->NumberOfVertices();i++)
    {
        QRect rect(Head[i].x,Head[i].y,20,20);
        if(showImpor&&Head[i].isImpor) paint->setBrush(Qt::red);
        else                paint->setBrush(Qt::lightGray);
        paint->drawRect(rect);
        paint->drawText(rect,Qt::AlignHCenter,QString::number(i+1));
    }
    paint->end();
}

void PaintArea::init()
{
    graph = NULL;
    NoData = true;
    isRand = false;
    drawComplete = false;
    showImpor = false;
    repaint();
}

void PaintArea::draw_rand(Graph_List *gra)
{
    graph = gra;
    graph->SetVertex(this->width(),this->height());

    NoData =false;
    isRand = true;
    showRedun = true;
    repaint();
    drawComplete = true;
}

void PaintArea::draw_manu(Graph_List *gra)
{
    graph = gra;
    graph->SetVertex(this->width(),this->height());
    NoData = false;
    isRand = false;
    showRedun = true;
    update();

}

void PaintArea::RemoveRedun()
{
    showRedun = false;
    update();
}

void PaintArea::AddRedun()
{
    showRedun = true;
    update();
}

void PaintArea::SetshowImor(bool flag)
{
    showImpor = flag;
}

///////////////////  槽函数     ////////////////////////



///////////////////  发射信号函数  /////////////////////

void PaintArea::draw_a_line_complete(int start, int end, bool isRedun)
{
    emit list_a_line(start,end,isRedun);
}

