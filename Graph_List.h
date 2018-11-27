
/** ！展开我！
*   本代码包含无向图和有向图两种选项
*   本代码默认使用有向图
*   如需使用无向图(可视为双向有向图)
*   请于包含警戒后添加
*   #define UNDIRECTED_GRAPH
*/


#ifndef GRAPH_LIST_H_INCLUDED
#define GRAPH_LIST_H_INCLUDED

#define UNDIRECTED_GRAPH


#include <time.h>
#include <cstdlib>
#include <QDebug>

#define MaxWeight 1000



struct Edge//边结点
{
    friend class Graph_List;
    int VerAdj;//邻接顶点序号
    int cost;///权值,本题中并未使用，
            /// 留以扩展程序功能，
            /// 如计算网络搭建费用等
    bool isRepeat;//是否为已存在的一条边的反向边
    bool isRedun;//是否为冗余边
    bool isPrint;//是否已经输出至文本框
    Edge *link;
};

struct Vertex//顶点表中结点
{
    friend class Graph_List;
    int VerName;//顶点名称
    int x,y;
    bool isImpor;//是否需要配备用机
    Edge *adjacent;
};

class Graph_List
{
    friend class tarjan;
private:
    Vertex *Head;//顶点表头指针
    int graphsize;
    int num_Useful;
    bool judgeComplete;
    bool isDepen;
    bool conRedun;
    ///kruscal数据///
    int *fa;


public:
    Graph_List(int num);
    Graph_List(int num1, int num2);
    virtual ~Graph_List();

    bool GraphEmpty() const;
    bool GraphFull() const;
    int NumberOfVertices() const;
    int NumberOfEdges() const;

    int GetWeight(const int& v1,const int& v2);//查找指定边权值
    Edge *GetNeighbors(const int v);//返回顶点v的邻接顶点表
    int GetFirstNeighbor(const int v);//查找某顶点第一个邻接顶点
    int GetNextNeighbor(const int v1,const int v2);
    Vertex *GetHeadList();

    void SetVertex(int,int);
    void InsertVertex(const int v);//向图中插入一个点
    void InsertEdge(const int v1,const int v2,const int weight);
    void DeleteVertex(const int v1);
    void DeleteEdge(const int v1,const int v2);//删除一条边

    bool GetisNet();
    void SetJudgeComplete(bool);
    void Judge();
    bool GetisDepen();
    void SetconRedun(bool);

    void Display()
    {
        int j=0;
        for(int i=0;i<graphsize;i++)
        {
            qDebug()<<i<<"->";
            Edge *p = Head[i].adjacent;
            while(p)
            {
                qDebug()<<'('<<p->VerAdj<<") ";
                j++;
                p = p->link;
            }
            qDebug()<<endl;
        }
        qDebug()<<j;
    }

    ///Kruscal函数///
    void init(int x){
        fa = new int[x];
        for(int i=0;i<=x;i++) fa[i]=i;
    }
    int getfa(int x){
        return fa[x]=(fa[x]==x?x:getfa(fa[x]));
    }
    void merge(int x,int y){
        x = getfa(x);
        y = getfa(y);
        fa[x]=y;
    }
    bool same(int x,int y){
        return getfa(x)==getfa(y);
    }




};

#endif // GRAPH_LIST_H_INCLUDED
