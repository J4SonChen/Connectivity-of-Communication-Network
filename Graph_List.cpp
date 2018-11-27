#include "Graph_List.h"

#include "tarjan.h"

Graph_List::Graph_List(int num)
{
    judgeComplete = false;
    isDepen = false;
    conRedun = true;
    num_Useful = 0;
    graphsize = num;
    init(graphsize);
    Head=new Vertex[graphsize];
    for(int i=0;i<graphsize;i++)
    {
        Head[i].VerName=i;
        Head[i].isImpor=false;
        Head[i].adjacent=NULL;
    }
}


Graph_List::Graph_List(int num1, int num2)
{
    judgeComplete = false;
    isDepen = false;
    conRedun = true;
    int rand_1,rand_2;
    int e,from,to,weight;
    num_Useful = 0;
    graphsize = num1;

    e = num2;
    weight = 1;
    init(graphsize);
    Head=new Vertex[graphsize];
    for(int i=0;i<graphsize;i++)
    {
        Head[i].VerName=i;
        Head[i].isImpor=false;
        Head[i].adjacent=NULL;
    }
    for(int i=0;i<e;i++)
    {
        rand_1 = qrand()%graphsize;
        rand_2 = qrand()%graphsize;
        while(rand_1==rand_2||GetWeight(rand_1,rand_2)!=-1)
        {
            rand_1 = qrand()%graphsize;
            rand_2 = qrand()%graphsize;
        }
        from = min(rand_1,rand_2);
        to = max(rand_1,rand_2);

        Edge *p = new Edge;
        p->VerAdj=to;
        p->cost=weight;
        p->isRepeat=false;
        p->isPrint=false;
        p->link=NULL;

        bool isRedun = same(from,to);
        if(isRedun) p->isRedun=true;
        else {
            p->isRedun=false;
            merge(from,to);
            num_Useful++;
        }

        Edge *q=Head[from].adjacent;
        if(q==NULL)
        {
            Head[from].VerName=from;
            Head[from].adjacent=p;
        }
        else
        {
            while(q->link)
                q=q->link;
            q->link=p;
        }
        //////以下代码为无向图准备
        #ifdef UNDIRECTED_GRAPH
        p = new Edge;
        p->VerAdj=from;
        p->cost=weight;
        p->isRepeat=true;
        p->isPrint=false;
        p->link=NULL;

        p->isRedun = isRedun;

        q=Head[to].adjacent;
        if(q==NULL)
        {
            Head[to].VerName=from;
            Head[to].adjacent=p;
        }
        else
        {
            while(q->link)
                q=q->link;
            q->link=p;
        }
        #endif // UNDIRECTED_GRAPH
    }
}

Graph_List::~Graph_List()
{
    for(int i=0;i<graphsize;i++)
    {
        Edge *p=Head[i].adjacent;
        while(p!=NULL)
        {
            Head[i].adjacent=p->link;
            delete p;
            p=Head[i].adjacent;
        }
    }
    delete[] Head;
    delete[] fa;
}

bool Graph_List::GraphEmpty() const
{
    return graphsize!=0;
}

int Graph_List::NumberOfVertices() const
{
    return graphsize;
}

int Graph_List::NumberOfEdges() const
{
    int num = 0;
    for(int i=0;i<graphsize;i++)
    {
        Edge *q = Head[i].adjacent;
        if(q)
        {
            num++;
            while(q->link)
            {
                q=q->link;
                num++;
            }
        }
    }
    #ifdef UNDIRECTED_GRAPH
    return num/2;
    #endif // UNDIRECTED_GRAPH
    return num;
}

int Graph_List::GetWeight(const int& v1,const int& v2)
{
    if(v1==-1||v2==-1)  return -1;
    Edge *p=Head[v1].adjacent;
    while(p!=NULL)
    {
        if(p->VerAdj==v2)
            return p->cost;
        p=p->link;
    }
    return -1;
}

Edge * Graph_List::GetNeighbors(const int v)
{
    if(v==-1)    return NULL;
    return Head[v].adjacent;
}

int Graph_List::GetFirstNeighbor(const int v)
{
    if(v==-1)   return -1;
    Edge *p=Head[v].adjacent;
    if(p)   return p->VerAdj;
    else    return -1;
}

int Graph_List::GetNextNeighbor(const int v1, const int v2)
{
    if(v1!=-1&&v2!=-1)
    {
        Edge *p=Head[v1].adjacent;
        while(p->VerAdj!=v2&&p) p=p->link;
        if(!p)  return -1;//若不存在v2结点，返回值-1
        p=p->link;
        if(!p)  return -1;//若存在v2结点，但v2无后继结点，返回值-1
        return p->VerAdj;
    }
    return -1;
}

Vertex * Graph_List::GetHeadList()
{
    return Head;
}

void Graph_List::InsertVertex(const int v)
{
    if(v<graphsize) qDebug()<<"This node is existed!";
    Head[++graphsize].VerName = v;
    Head[graphsize].adjacent = NULL;
}

void Graph_List::SetVertex(int width,int height)
{

    for(int i=0;i<graphsize;i++)
    {
        Head[i].x = rand()%(width-50)+15;
        Head[i].y = rand()%(height-50)+15;
    }
}

void Graph_List::InsertEdge(const int v1,const int v2,int weight)
{
    if(v1==-1||v2==-1) return;
    if(weight==0||weight>=MaxWeight) return;
    if(v1>=graphsize||v2>graphsize){
        qDebug()<<"Error,the vertex of edge don't exist!";
        return;
    }
    if(GetWeight(v1,v2)!=-1){
        qDebug()<<"Error,this is an edge existed!";
        return;
    }
    else
    {
        Edge *p=new Edge;
        p->VerAdj=v2;
        p->cost=weight;
        p->isRepeat=false;
        p->isPrint=false;
        p->link=NULL;

        bool isRedun = same(v1,v2);
        if(isRedun) p->isRedun=true;
        else {
            p->isRedun=false;
            merge(v1,v2);
            num_Useful++;
        }

        Edge *q=Head[v1].adjacent;
        if(q==NULL)
        {
            Head[v1].VerName=v1;
            Head[v1].adjacent=p;
        }
        else
        {
            while(q->link!=NULL)
                q=q->link;
            q->link=p;
        }
        //以下代码为无向图准备
        #ifdef UNDIRECTED_GRAPH
        p=new Edge;
        p->VerAdj=v1;
        p->cost=weight;
        p->isRepeat=true;
        p->isPrint=false;
        p->link=NULL;

        if(isRedun) p->isRedun=true;
        else        p->isRedun=false;

        q=Head[v2].adjacent;
        if(q==NULL)
        {
            Head[v2].VerName=v2;
            Head[v2].adjacent=p;
        }
        else
        {
            while(q->link!=NULL)
                q=q->link;
            q->link=p;
        }
        #endif // UNDIRECTED_GRAPH
    }
}

void Graph_List::DeleteVertex(const int v)
{
    if(v==-1)    qDebug()<<"The vertex is not in the graph"<<endl;
    else
    {
        graphsize--;
        for(int i=0;i<graphsize;i++)
        {
            Edge *p = Head[i].adjacent;
            if(i==v)
            {
                while(p!=NULL)
                {
                    Head[i].adjacent = p->link;
                    delete p;
                    p = Head[i].adjacent;
                }
            }
            else
            {
                while(p->link->VerAdj!=v&&p->link)
                {
                    if(p->VerAdj>v) p->VerAdj -= 1;
                    p = p->link;
                }
                if(p->link->VerAdj==v)
                {
                    Edge *q = p->link;
                    p->link = q->link;
                    delete q;
                }
                while(p->link)
                {
                    if(p->VerAdj>v)
                        p->VerAdj -= 1;
                    p = p->link;
                }
            }
        }
    }
}

void Graph_List::DeleteEdge(const int v1,const int v2)
{
    if(v1==-1||v2==-1) return;
    if(v1>=graphsize||v2>=graphsize)  {qDebug()<<"The edge is not in the graph"<<endl;return;}
    Edge *p=Head[v1].adjacent;
    if(p->VerAdj==v2)
    {
        Edge *q = p;
        Head[v1].adjacent = q->link;
        delete q;
    }
    else
    {
        while(p->link)
        {
            if(p->link->VerAdj==v2)
            {
                Edge *q = p->link;
                p->link = q->link;
                delete q;break;
            }
            p = p->link;
        }
    }
    //以下代码为无向图准备
    #ifdef UNDIRECTED_GRAPH
    p = Head[v2].adjacent;
    if(p->VerAdj==v1)
    {
        Edge *q = p;
        Head[v2].adjacent = q->link;
        delete q;
    }
    else
    {
        while(p->link)
        {
            if(p->link->VerAdj==v1)
            {
                Edge *q = p->link;
                p->link = q->link;
                delete q;break;
            }
            p = p->link;
        }
    }
    #endif // UNDIRECTED_GRAPH
}

bool Graph_List::GetisNet()
{
    return num_Useful==graphsize-1;
}

void Graph_List::SetJudgeComplete(bool flag)
{
    judgeComplete = flag;
}

void Graph_List::Judge()
{
    if(!judgeComplete)
    {
        tarjan t;
        t.input(this);
        t.count(this);
        judgeComplete = true;
    }
}

bool Graph_List::GetisDepen()
{
    return isDepen;
}

void Graph_List::SetconRedun(bool flag)
{
    conRedun = flag;
}
