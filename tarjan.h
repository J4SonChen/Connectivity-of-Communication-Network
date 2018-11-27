#ifndef TARJAN_H
#define TARJAN_H

#include<iostream>
using namespace std;
#include<cstdio>
#include<cstring>
#include<vector>

#include "Graph_List.h"
#define N 10000



class tarjan
{
    friend class Graph_List;
public:
    tarjan() {
        memset(dfn,-1,sizeof(dfn));
        memset(father,0,sizeof(father));
        memset(low,-1,sizeof(low));
        memset(is_cut,false,sizeof(is_cut));
    }
    void input(Graph_List *graph)
    {
        n = graph->graphsize;
        m = graph->NumberOfEdges();
        for(int i=0;i<graph->graphsize;i++)
        {
            Edge *p = graph->Head[i].adjacent;
            while(p)
            {
                if(p->isRedun&&!graph->conRedun) ;
                else {
                    G[i+1].push_back(p->VerAdj+1);/*邻接表储存无向边*/
                    G[p->VerAdj+1].push_back(i+1);
                }

                p = p->link;
            }
        }
    }
    void Tarjan(int i,int Father)
    {
        father[i]=Father;/*记录每一个点的父亲*/
        dfn[i]=low[i]=tim++;
        for(unsigned int j=0;j<G[i].size();++j)
        {
            int k=G[i][j];
            if(dfn[k]==-1)
            {
                Tarjan(k,i);
                low[i]=min(low[i],low[k]);
            }
            else if(Father!=k)/*假如k是i的父亲的话，那么这就是无向边中的重边，有重边那么一定不是桥*/
                low[i]=min(low[i],low[k]);
        }
    }
    void count(Graph_List *graph)
    {
        int rootson=0;
        Tarjan(1,0);

        for(int i=2;i<=n;++i)
        {
            int v=father[i];
            if(v==1)
            rootson++;//统计根节点子树的个数
            else{
                if(low[i]>=dfn[v]){/*割点的条件*/
                    is_cut[v]=true;
                }
            }
        }
        if(rootson>1)   is_cut[1]=true;//根节点的子树个数>=2,就是割点

        for(int i=1;i<=n;++i){
            if(is_cut[i]){
                qDebug()<<i<<endl;
            }
            graph->Head[i-1].isImpor = is_cut[i];
        }
        int bridge = 0;
        for(int i=1;i<=n;++i)
        {
            int v=father[i];
            if(v>0&&low[i]>dfn[v]){/*桥的条件*/
                qDebug()<<v<<","<<i<<endl;
                bridge++;
            }
        }
        if(bridge>0)    graph->isDepen = false;
        else            graph->isDepen = true;
    }
private:
    vector<int> G[N];
    int n,m;
    int low[N],dfn[N];
    bool is_cut[N];
    int father[N];
    int tim=0;
};

#endif // TARJAN_H
