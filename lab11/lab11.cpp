#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
#define step_size1 100
#define step_size2 10

Profiler p("lab11");

typedef struct node
{
    int k;
    int visited;
    int time[2];
    node* next;
}NodeT;

typedef struct _node
{
    int N;
    NodeT** edges; //array of pointers
}Graph;

NodeT* new_node(int key )
{
    NodeT* x = (NodeT*) malloc(sizeof(NodeT));
    x->k = key;
    x->visited = 0;
    x->next = NULL;
    return x;
}

void new_edge(Graph* g1, int a, int b) // add a new edge between nodes with keys a and b in the graph
{
    NodeT* x = new_node(b);
    x->next = g1->edges[a]->next; // next of the new node x is set to point to the first node in the adjacency list of the node with key a in the graph
    g1->edges[a]->next = x;
}

void show_Adjacency_List(Graph* g1)
{
    int k;
    for(k=0; k<g1->N; k++)
    {
        printf("node %d : ",k);
        NodeT* a=g1->edges[k]->next;
        while(a!=NULL)
        {
            printf("%d ",a->k);
            a = a->next;
        }
        printf("\n");
    }
}

Graph* create(int N)
{
    Graph* g1=(Graph*)malloc(sizeof(Graph));
    g1->edges=(NodeT**)malloc(sizeof(NodeT*)*N);
    g1->N=N;
    int k=0;
    while(k<N)
    {
        g1->edges[k]= new_node(k);
        k++;
    }
    return g1;
}

//traverse the graph starting from node s using DFS and update an array arr with the parents of nodes in the DFS Tree
void DFS(Graph* g1, int s, int arr[], int profiler, int *time)
{
    NodeT* s1=g1->edges[s];
    g1->edges[s1->k]->visited=1;
    printf("%d(%d) ",s1->k, *time);

    //timp descoperire
    g1->edges[s]->time[0] = *time;

    (*time)++;
    s1=s1->next;
    p.countOperation("DFS",profiler,4);
    while(s1!=NULL)
    {   p.countOperation("DFS",profiler,2);
        if(g1->edges[s1->k]->visited==0)
        {
            arr[s1->k]=g1->edges[s]->k;
            DFS(g1,s1->k, arr, profiler,time);
        }
        s1=s1->next;
    }

    g1->edges[s]->time[1] = *time;
}

typedef struct Random_Struct{
    int x;
    int y;
}Random_Struct; //pairs of indices in  operations

//it ensures that from node x to node y I have at most one edge
bool FindInStruct(Random_Struct randomStruct[], int size, Random_Struct key)
{
    if(key.x == key.y)
    {
        return true;
    }
    for(int i = 0; i < size; i++){
        if(key.x == randomStruct[i].x && key.y == randomStruct[i].y){
            return true;
        }
    }
    return false;
}
void perf()
{
    int i;
    for(i=1000;i<4500;i+=step_size1)
    {
        Graph* g1= create(100);
        Random_Struct randomStruct[i+1];
        int size=0;
        int k=0;
        while(k<i)
        {
            int indice2=rand()%(g1->N-1);
            int indice=rand()%(g1->N-1);
            Random_Struct element={indice,indice2};
            while (FindInStruct(randomStruct,size,element))
            {
                indice2=rand()%(g1->N-1);
                indice=rand()%(g1->N-1);
                element={indice,indice2};
            }
            randomStruct[size++]={indice,indice2};
            new_edge(g1,indice,indice2);
            k++;
        }
        int time = 0;
        int arr[4501];
        DFS(g1,0,arr, i,&time);
    }
    p.showReport();
}

void perf1()
{
    int i;
    for(i=100;i<200;i+=step_size2)
    {
        Graph* g1= create(i);
        Random_Struct randomStruct[4501];
        int size=0;
        int k=0;
        while(k<4500)
        {
            int index2=rand()%(g1->N-1);
            int index1=rand()%(g1->N-1);
            Random_Struct element={index1,index2};
            while (FindInStruct(randomStruct,size,element))
            {
                index2=rand()%(g1->N-1);
                index1=rand()%(g1->N-1);
                element={index1,index2};
            }
            randomStruct[size++]={index1,index2};
            new_edge(g1,index1,index2);
            k++;
        }
        int time = 0;
        int arr[4501];
        DFS(g1,0,arr, i, &time);
    }
    p.showReport();
}

typedef struct {
    NodeT* item[4500];
    int q;
}stack;

void init_stack(stack* S)
{
    S->q=-1;
}

bool is_empty(stack* stack1)
{
    if(stack1->q == -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_full(stack* stack1)
{
    if(stack1->q==4500)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void push(stack* stack1, NodeT* element)
{
    if(is_full(stack1))
    {
        perror("the stack is full");
        exit(-1);
    }
    else
    {
        stack1->item[++stack1->q]=element;
    }
}
NodeT* pop(stack* stack1)
{
    if(is_empty(stack1))
    {
        perror("is already empty");
        exit(-1);
    }
    else
    {
        return stack1->item[stack1->q--];
    }
}
NodeT* peek(stack* stack1)
{
    if(is_empty(stack1))
    {
        perror("is already empty");
        exit(-1);
    }
    else
    {
        return stack1->item[stack1->q];
    }
}

//DFS-based topological sort of the graph starting from node s and adds nodes to a stack
void DFS_Topological_Sort(Graph* g1, int s, stack* stack1)
{
    NodeT* s1=g1->edges[s];
    g1->edges[s1->k]->visited=1;
    s1=s1->next;
    while(s1!=NULL)
    {
        if(g1->edges[s1->k]->visited==0)
        {
            DFS_Topological_Sort(g1,s1->k,stack1);
        }
        s1=s1->next;
    }
    push(stack1,g1->edges[s]);
}
void Topological_Sort(Graph* g1)
{
    stack stack1;
    init_stack(&stack1);
    int k=0;
    while(k<g1->N)
    {
        if(g1->edges[k]->visited==0)
        {
            DFS_Topological_Sort(g1,k,&stack1);
        }
        k++;
    }
    while (!is_empty(&stack1))
    {
        NodeT* a= pop(&stack1);
        printf("%d ",a->k);
    }
}

void scc(Graph* g1, int u, int disc[], int low[],stack* stack1,int stackMember[], int *time)
{
    (*time)++;
    disc[u]=(*time);
    low[u]=(*time);

    NodeT* s1=g1->edges[u];
    push(stack1,s1);
    stackMember[u]=1;
    s1=s1->next;
    while(s1!=NULL)
    {
        if(disc[s1->k]==-1)
        {
            scc(g1,s1->k,disc,low,stack1,stackMember,time);
            if(low[u]>low[s1->k])
            {
                low[u]=low[s1->k];
            }
        }
        else
        {
            if(stackMember[s1->k])
            {
                if(disc[s1->k]<low[u])
                {
                    low[u]=disc[s1->k];
                }
            }
        }
        s1=s1->next;
    }

    NodeT* q;

    if(low[u]==disc[u])
    {
        while(peek(stack1)->k != u)
        {
            q = pop(stack1);
            printf("%d ",q->k);
            stackMember[q->k]=0;
        }
        q= pop(stack1);
        printf("%d\n",q->k);
        stackMember[q->k]=0;
    }
}

void init_comp(int n, int disc[], int low[], int stackMember[])
{
    for(int i=0;i<n;i++)
    {
        disc[i]=-1;
        low[i]=-1;
        stackMember[i]= -1;
    }
}

void Tarjan_SCC(Graph* g1)
{
    int disc[g1->N];
    int low[g1->N];
    int stackMember[g1->N];

    init_comp(g1->N,disc,low,stackMember);

    stack stack1;
    init_stack(&stack1);

    int t=0;

    int k = 0;
    while(k<g1->N)
    {
        if(disc[k]==-1)
        {
            scc(g1,k,disc,low,&stack1,stackMember,&t);
        }
        k++;
    }

}

void demo1()
{
     Graph* g1 = create(8);
    new_edge(g1, 0, 1);
    new_edge(g1, 0, 2);
    new_edge(g1, 0, 3);
    new_edge(g1, 1, 3);
    new_edge(g1, 2, 4);
    new_edge(g1, 3, 5);
    new_edge(g1, 3, 6);
    new_edge(g1, 4, 7);
    new_edge(g1, 4, 5);
    new_edge(g1, 5, 2);

    Tarjan_SCC(g1);
}

void pprint1(int a[], int n, int root, int level)
{
    for (int i = 0; i < n; i++) {
        if (a[i] == root) {
            printf("%*s%d\n", level * 4, "", i);
            pprint1(a, n, i, level + 1);
        }
    }
}

void init_array(int arr[], int n)
{
    int k=0;
    while(k<n)
    {
        arr[k]=-2;
        k++;
    }
}

void demo()
{
    Graph* g1 = create(8);
    new_edge(g1, 0, 1);
    new_edge(g1, 0, 2);
    new_edge(g1, 0, 3);
    new_edge(g1, 1, 3);
    new_edge(g1, 2, 4);
    new_edge(g1, 3, 5);
    new_edge(g1, 3, 6);
    new_edge(g1, 4, 7);
    new_edge(g1, 4, 5);
    new_edge(g1, 5, 2);

    show_Adjacency_List(g1);





    // printf("Topological Sort : ") ;
    // Topological_Sort(g1) ;
    // printf("\n");
    
    int time = 0;
    int arr[8];
    init_array(arr, g1->N); // VECTOR CE PARINTI PENTRU ARBORELE REZULTAT DFS
    DFS(g1,0,arr,0, &time); // DFS PLEACA DE LA 0
    printf("\nDFS TREE:\n");
    arr[0]=-1; // MARCARE NOD DE START CA SA STIE PRETTY PRINT UNDE E RADACINA

    pprint1(arr,g1->N,-1,0);

    for(int i = 0; i < g1->N; i++){
        printf(" %d (%d-%d)\n", g1->edges[i]->k, g1->edges[i]->time[0], g1->edges[i]->time[1]);
    }

}

int main()
{
    demo();
    demo1();
   //perf();
   //perf1();
    return 0;
}
