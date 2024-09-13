#include "Profiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


Profiler P("lab9");
using namespace std;

#define step_size 100
#define max_size 10
#define nr_tests 5

typedef struct Edge{
    int e1;
    int e2;
    int cost;
}Edge;

int COMPARE_EDGES(const void* a, const void *b)
{
    const Edge* pa=(Edge*)a;
    const Edge* pb=(Edge*)b;
    return pa->cost - pb->cost;
}

void MAKE_SET(int parents[], int ranks[], int n)
{
    for(int i=0;i<n;i++)
    {
        parents[i]=i; P.countOperation("MAKE_SET",n);
        ranks[i]=0; P.countOperation("MAKE_SET",n);
    }
}

int FIND_PARENT(int parents[], int x, int p)
{   P.countOperation("FIND_PARENT",p);
    if(parents[x]==x)
    {
        return x;
    }
    return parents[x] = FIND_PARENT(parents,parents[x],p);
}

void UNION_SET(int x,int y,int parents[],int rank[],int p)
{
    int u1 = FIND_PARENT(parents,x,p); 
    P.countOperation("UNION_SET",p);
    
    int u2 = FIND_PARENT(parents,y,p); 
    P.countOperation("UNION_SET",p);

    P.countOperation("UNION_SET",p);
    if(rank[u1] <= rank[u2])
    {
        parents[u1]=u2; 
        P.countOperation("UNION_SET",p);
        P.countOperation("UNION_SET",p);
        if(rank[u1]==rank[u2])
        {
            P.countOperation("UNION_SET",p);
            rank[u2]++;
        }
    }
    else
    {
        P.countOperation("UNION_SET",p);
        parents[u2] = u1;
    }
}

vector<Edge> kruskal(Edge edges[], int m,int n)
{
    printf("Kruskal : \n") ;
    vector<Edge> result;
    int parents[n];
    int ranks[n];
    MAKE_SET(parents,ranks,n);
    qsort(edges,m,sizeof(Edge),COMPARE_EDGES);
    for(int i=0;i<m;i++)
    {
        int q1= FIND_PARENT(parents,edges[i].e1,n);
        int q2= FIND_PARENT(parents,edges[i].e2,n);
        if(q1!=q2)
        {
            Edge new_edge_to_push = {edges[i].e1,edges[i].e2,edges[i].cost};
            printf ("edge : (%d,%d) with weigth %d\n" ,edges[i].e1,edges[i].e2,edges[i].cost) ;
            result.push_back(new_edge_to_push);
            UNION_SET(q1,q2,parents,ranks,n);
        }
    }
    return result;
}

void print_result(vector<Edge> result)
{
    for(int i=0;i<result.size();i++)
    {
        printf("%d %d %d\n",result[i].e1,result[i].e2,result[i].cost);
    }
}

void demo()
{
    int nr = 10; 
    int parents[10];
    int ranks[10];

    MAKE_SET(parents,ranks,nr);

    for(int i = 0; i < nr; i++)
    {
        printf("node : %d parent : %d rank : %d\n",
               i,parents[i],ranks[i]);
    }

    printf("-------------------------\n");

    printf("UNION SET 2,3 :\n");
    UNION_SET(2,3,parents,ranks,3);

    for(int i = 0; i < nr; i++)
    {
        printf("node : %d parent : %d rank : %d\n",
               i,parents[i],ranks[i]);
    }

    printf("UNION SET 5,6 :\n");
    UNION_SET(5,6,parents,ranks,3);

    for(int i = 0; i < nr; i++)
    {
        printf("node : %d parent : %d rank : %d\n",
               i,parents[i],ranks[i]);
    }

    printf("UNION SET 3,6 :\n");
    UNION_SET(3,6,parents,ranks,3);

    for(int i = 0; i < nr; i++)
    {
        printf("node : %d parent : %d rank : %d\n",
               i,parents[i],ranks[i]);
    }


    printf("\nPARINTELE NODULUI 5:");

    printf("%d", FIND_PARENT(parents,5,5));

    printf("\n") ;

}

void demoKruskal(){
    Edge edges[]={
            {4,3,9},
            {4,0,4},
            {3,0,1},
            {3,2,5},
            {0,1,2},
            {3,1,3},
            {2,1,3},
            {2,5,8},
            {1,5,7}
    };
    vector<Edge> result = kruskal(edges,9,6);
    print_result(result);

}

bool exists(int a,int b,Edge edge[],int m)
{
    int k=0;
    while(k<m)
    {
        if(edge[k].e1==a && edge[k].e2==b)
        {
            return true;
        }
        if(edge[k].e1==b && edge[k].e2==a)
        {
            return true;
        }
        k++;
    }
    return false;
}

void build_MyGraph(Edge edges[],int n)
{
    srand(time(NULL));
    for(int i=0;i<n-1;i++)
    {
        edges[i].e1=i;
        edges[i].e2=i+1;
        edges[i].cost=rand()%(n-2);
    }
    for(int i=n-1;i<4*n;i++)
    {
        edges[i].e1=rand()%(n-2);
        edges[i].e2=rand()%(n-2);
        edges[i].cost=rand()%(n-2);
        while (exists(edges[i].e1,edges[i].e2,edges,i) || edges[i].e1 == edges[i].e2)
        {
            edges[i].e1=rand()%(n-2);
            edges[i].e2=rand()%(n-2);
        }
//        printf("%d %d %d\n",edges[i].e1,edges[i].e2,edges[i].cost);
    }
}

void perf()
{
    int i;
    for(i=step_size;i<max_size;i+=step_size)
    {
       for(int tests=0;tests<=nr_tests;++tests)
       {
            Edge edge[i * 4];
            build_MyGraph(edge,i);
            vector<Edge> result = kruskal(edge,i*4,i);
       }
    }
    
    P.showReport();

}

int main() 
{
    demo();
    demoKruskal();
    //perf();

    return 0;
}
