#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("城市通讯模拟程序");
    paint = new PaintArea(this);
    ui->verticalLayout_5->addWidget(paint);

    HideInputButtons();

    ui->pushButtonRand->setDefault(true);
    ui->pushButtonHideR->setDisabled(true);
    ui->pushButtonShowR->setDisabled(true);
    ui->pushButtonJudge->setDisabled(true);
    ui->pushButtonImpor->setDisabled(true);
    ui->pushButtonSave->setDisabled(true);

    graph = NULL;

    ////////工具栏信号槽//////////////////
    QObject::connect(ui->pushButtonRand, SIGNAL(clicked(bool)), this, SLOT(CreateRand()));
    QObject::connect(ui->pushButtonManu, SIGNAL(clicked(bool)), this, SLOT(CreateManu()));
    QObject::connect(ui->pushButtonHideR, SIGNAL(clicked(bool)), this, SLOT(RemoveRedun()));
    QObject::connect(ui->pushButtonShowR, SIGNAL(clicked(bool)), this, SLOT(AddRedun()));
    QObject::connect(ui->pushButtonJudge, SIGNAL(clicked(bool)), this, SLOT(JudgeDepen()));
    QObject::connect(ui->pushButtonImpor, SIGNAL(clicked(bool)), this, SLOT(ImportantCity()));
    QObject::connect(ui->pushButtonSave, SIGNAL(clicked(bool)), this, SLOT(Save()));
    QObject::connect(ui->pushButtonClear, SIGNAL(clicked(bool)), this, SLOT(Clear()));
    QObject::connect(ui->pushButtonAdd, SIGNAL(clicked(bool)), this, SLOT(AddALine()));
    QObject::connect(ui->pushButtonAband, SIGNAL(clicked(bool)), this, SLOT(AbandonInput()));
    QObject::connect(paint, SIGNAL(list_a_line(int,int,bool)), this, SLOT(list_a_line(int,int,bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
//////////////////  public函数   /////////////////////

void MainWindow::PrepareDraw()
{
    if(graph!=NULL)   delete graph;

    ui->pushButtonRand->setDisabled(true);
    ui->pushButtonManu->setDisabled(true);
    ui->pushButtonRand->setDefault(true);
    ui->pushButtonHideR->setDisabled(true);
    ui->pushButtonShowR->setDisabled(true);
    ui->pushButtonJudge->setDisabled(true);
    ui->pushButtonImpor->setDisabled(true);
    ui->pushButtonImpor->setText("标记备用机");
    ui->pushButtonSave->setDisabled(true);
    ui->pushButtonClear->setDisabled(true);

    ui->labelNowCity->setText(QString::number(num_city));
    ui->labelNowWay->setText(QString::number(num_way));
    ui->textBrowserRedun->clear();
    ui->textBrowserWay->clear();

    paint->init();
    qDebug()<<"prepare complete";
}


void MainWindow::ActiveButtons()
{
    ui->pushButtonRand->setText("随机构建");
    ui->pushButtonRand->setDisabled(false);
    ui->pushButtonRand->setFocus();
    ui->pushButtonManu->setText("手动构建");
    ui->pushButtonManu->setDisabled(false);
    ui->pushButtonHideR->setDisabled(false);
    ui->pushButtonShowR->setDisabled(false);
    ui->pushButtonJudge->setDisabled(false);
    ui->pushButtonImpor->setDisabled(false);
    ui->pushButtonSave->setDisabled(false);
    ui->pushButtonClear->setDisabled(false);
}

void MainWindow::HideInputButtons()
{
    ui->label_Start->hide();
    ui->lineEditStart->hide();
    ui->label_End->hide();
    ui->lineEditEnd->hide();
    ui->pushButtonAdd->hide();
    ui->pushButtonAband->hide();
}

void MainWindow::ShowInputButtons()
{
    ui->label_Start->show();
    ui->lineEditStart->show();
    ui->label_End->show();
    ui->lineEditEnd->show();
    ui->pushButtonAdd->show();
    ui->pushButtonAband->show();
}

void MainWindow::setData(int c,int w)
{
    num_city = c;
    num_way = w;
}




//////////////////  槽函数  ////////////////////
void MainWindow::CreateRand()
{

    qsrand(time(NULL));//设置随机种子

    int input_1 = ui->lineEditCity->text().toInt();
    int input_2 = ui->lineEditWay->text().toInt();

    if(input_1 <= 1 || input_2 <= 0 || input_2 > input_1*(input_1-1)/2) {
        QMessageBox::warning(this,"警告","您的输入不正确，请重新输入！",QMessageBox::Ok);
        //清空内容并定位光标
        ui->lineEditCity->clear();
        ui->lineEditWay->clear();
        ui->lineEditCity->setFocus();
    }
    else {
        setData(input_1,input_2);
        ui->pushButtonRand->setText("构建中");
        PrepareDraw();


        graph = new Graph_List(num_city,num_way);
        graph->Display();
        paint->draw_rand(graph);
        ActiveButtons();
        qDebug()<<"ok2";
    }

}

void MainWindow::CreateManu()
{
    qsrand(time(NULL));//设置随机种子

    int input_1 = ui->lineEditCity->text().toInt();
    int input_2 = ui->lineEditWay->text().toInt();

    if(input_1 <= 1|| input_2 <= 0 || input_2 > input_1*(input_1-1)/2) {
        QMessageBox::warning(this,"警告","您的输入不正确，请重新输入！",QMessageBox::Ok);
        //清空内容并定位光标
        ui->lineEditCity->clear();
        ui->lineEditWay->clear();
        ui->lineEditCity->setFocus();
    }
    else {
        setData(input_1,input_2);
        ui->pushButtonManu->setText("构建中");
        PrepareDraw();
        graph = new Graph_List(num_city);
        graph->Display();
        ShowInputButtons();
        ui->lineEditStart->setFocus();
        ui->pushButtonAdd->setDefault(true);
        paint->draw_manu(graph);
    }
}

void MainWindow::RemoveRedun()
{
    ui->pushButtonJudge->setDisabled(true);
    graph->SetJudgeComplete(false);
    graph->SetconRedun(false);
    graph->Judge();
    paint->RemoveRedun();
}

void MainWindow::AddRedun()
{
    ui->pushButtonJudge->setDisabled(false);
    graph->SetJudgeComplete(false);
    graph->SetconRedun(true);
    graph->Judge();
    paint->AddRedun();
}

void MainWindow::JudgeDepen()
{
    graph->Judge();
    if(graph->GetisNet()){
        if(graph->GetisDepen()) QMessageBox::information(this,"可靠性","这是一个可靠网络",QMessageBox::Ok);
        else                    QMessageBox::information(this,"可靠性","这是一个不可靠网络",QMessageBox::Ok);
    }
    else {
        QMessageBox::information(this,"可靠性","该网络不完善，有城市未联网",QMessageBox::Ok);
    }
}

void MainWindow::ImportantCity()
{
    if(!paint->GetshowImor())
    {
        ui->pushButtonImpor->setText("取消标记");
        paint->SetshowImor(true);
    }
    else {
        ui->pushButtonImpor->setText("标记备用机");
        paint->SetshowImor(false);
    }
    graph->Judge();
    paint->update();
}

void MainWindow::Save()
{
    QString filename = QFileDialog::getSaveFileName(this,"保存",QDir::homePath());

    QFile *file = new QFile;
    file->setFileName((filename));
    if(file->open(QIODevice::WriteOnly|QIODevice::Text)) {
        QTextStream out(file);

        graph->Judge();
        if(graph->GetisNet()){
            if(graph->GetisDepen()) out<<tr("这是一个可靠网络，含：")<<endl;
            else                    out<<tr("这是一个不可靠网络，含：")<<endl;
        }
        else {
            out<<tr("该网络不完善，有城市未联网，含：")<<endl;
        }
        out<<tr("城市：")<<num_city<<tr("个")<<endl;
        out<<tr("线路：")<<num_way<<tr("个")<<endl;
        out<<tr("实用线路如下：")<<endl;
        out<<ui->textBrowserWay->toPlainText()<<endl;
        out<<tr("冗余线路如下：")<<endl;
        out<<ui->textBrowserRedun->toPlainText()<<endl;


        file->close();
        delete file;
    }
    else {
        return;
    }
}

void MainWindow::Clear()
{
    if(graph)   {qDebug()<<"Delete grapgh.";delete graph;}
    graph = NULL;
    ui->lineEditCity->clear();
    ui->lineEditWay->clear();
    ui->lineEditCity->setFocus();

    ui->pushButtonRand->setDefault(true);
    ui->pushButtonHideR->setDisabled(true);
    ui->pushButtonShowR->setDisabled(true);
    ui->pushButtonJudge->setDisabled(true);
    ui->pushButtonImpor->setDisabled(true);
    ui->pushButtonImpor->setText("标记备用机");
    ui->pushButtonSave->setDisabled(true);

    ui->labelNowCity->clear();
    ui->labelNowWay->clear();
    ui->textBrowserRedun->clear();
    ui->textBrowserWay->clear();

    paint->init();
}

void MainWindow::AddALine()
{
    int input_1 = ui->lineEditStart->text().toInt();
    int input_2 = ui->lineEditEnd->text().toInt();
    if(input_1 < 1 ||input_1 > num_city || input_2 < 1 || input_2 > num_city || input_1 == input_2){
        QMessageBox::warning(this,"警告","您的输入不正确，请重新输入！",QMessageBox::Ok);
    }
    else if(graph->GetWeight(input_1-1,input_2-1)!=-1){
        QMessageBox::warning(this,"警告","该边已存在，请重新输入！",QMessageBox::Ok);
    }
    else {
        graph->InsertEdge(input_1-1,input_2-1,1);
        paint->update();
        if(graph->NumberOfEdges()==num_way)
        {
            ActiveButtons();
            HideInputButtons();
        }
    }
    ui->lineEditStart->clear();
    ui->lineEditEnd->clear();
    ui->lineEditStart->setFocus();
    ui->pushButtonAdd->setDefault(true);
}

void MainWindow::AbandonInput()
{
    ActiveButtons();
    Clear();
    HideInputButtons();

}

void MainWindow::list_a_line(int init,int end,bool isRedun)
{
    if(isRedun){
        QString tmp;
        tmp.sprintf("(%4d ,%4d )",init,end);
        ui->textBrowserRedun->append(tmp);
    }
    else {
        QString tmp;
        tmp.sprintf("(%4d ,%4d )",init,end);
        ui->textBrowserWay->append(tmp);
    }
}
