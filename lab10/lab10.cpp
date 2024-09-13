#include <stdlib.h>
#include <string.h>
#include "bfs.h"

#define MAX_V 10000

typedef struct Queue {
    int front , rear , size  ;
    Node* item[MAX_V] ;
}Queue ; 


int get_neighbors(const Grid *grid, Point p, Point neighb[])
{
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4

    //luam punctele din jur
    int k = 0;

    Point buffer = p;

    buffer.col--;
    if(grid->mat[buffer.row][buffer.col] == 0){
        neighb[k] = buffer;
        k++;
    }

    buffer = p;
    buffer.col++;
    if(grid->mat[buffer.row][buffer.col] == 0){
        neighb[k] = buffer;
        k++;
    }

    buffer = p;
    buffer.row--;
    if(grid->mat[buffer.row][buffer.col] == 0){
        neighb[k] = buffer;
        k++;
    }

    buffer = p;
    buffer.row++;
    if(grid->mat[buffer.row][buffer.col] == 0){
        neighb[k] = buffer;
        k++;
    }
    
    return k;
} 

void grid_to_graph(const Grid *grid, Graph *graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for(i=0; i<graph->nrNodes; ++i){
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                        graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}


void init(Queue* q) 
{
    q->front = q->rear = -1;
    q->size = 0 ;
}


void Enqueue(Queue *q, Node *it)
{
    if (q->rear == MAX_V - 1)
    {
        printf("Queue is full\n");
        exit(1);
    }

    if (q->front == -1 && q->rear == -1)
    {
        q->front = 0;
    }

    q->item[++q->rear] = it;
    q->size++;
}

Node *Dequeue(Queue *q)
{
    Node *f = q->item[q->front++];
    q->size--;

    if (q->front > q->rear)
    {
        q->front = q->rear = -1;
    }

    return f;
}


bool isEmpty(Queue* q) 
{
    return q->size == 0;
}


void bfs(Graph *graph, Node *s, Operation *op)
{
    for (int i = 0; i < graph->nrNodes; i++)
    {
        if(op != NULL) op->count(3);
        graph->v[i]->color = COLOR_WHITE;
        graph->v[i]->dist = INT_MAX;
        graph->v[i]->parent = NULL;
    }

    // start node init
    s->color = COLOR_GRAY;
    s->dist = 0;
    s->parent = NULL;
    if(op != NULL) op->count(3);

    // queue
    Queue q;
    init(&q);
    Enqueue(&q, s); if(op != NULL) op->count();

    // bfs traversal
    if(op != NULL) op->count(3);
    while (q.front != -1)
    {
        Node *u = Dequeue(&q); if(op != NULL) op->count();
        for (int i = 0; i < u->adjSize; i++)
        {
            Node *v = u->adj[i]; if(op != NULL) op->count();
            if (v->color == COLOR_WHITE)
            {
                v->color = COLOR_GRAY;
                v->dist = u->dist + 1;
                v->parent = u;
                Enqueue(&q, v);
                if(op != NULL) op->count(4);
            }
        }
        u->color = COLOR_BLACK; if(op != NULL) op->count();
    }
}

void pprint(int a[], int n, int root, int level, Point repr[]) 
{
    for (int i = 0; i < n; i++) 
    {
        if (a[i] == root) 
        {
            printf("%*s(%d,%d)\n", level * 4, "", repr[i].row,repr[i].col);
            pprint(a, n, i, level + 1, repr);
        }
    }
}

void print_bfs_tree(Graph *graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for(int i=0; i<graph->nrNodes; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            transf[i] = n;
            ++n;
        }else{
            transf[i] = -1;
        }
    }
    if(n == 0){
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;                         
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for(int i=0; i<graph->nrNodes && !err; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            if(transf[i] < 0 || transf[i] >= n){
                err = 1;
            }else{
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL){
                    p[transf[i]] = -1;
                }else{
                    err = 1;
                    for(int j=0; j<graph->nrNodes; ++j){
                        if(graph->v[i]->parent == graph->v[j]){
                            if(transf[j] >= 0 && transf[j] < n){
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if(!err){
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs

        pprint(p, n, -1, 0, repr);

    }

    if(p != NULL){
        free(p);
        p = NULL;
    }
    if(repr != NULL){
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[]) 
{
    if (start == NULL || end == NULL) 
    {
        return -1; 
    }

    for (int i = 0; i < graph->nrNodes; i++) 
    {
        graph->v[i]->color = COLOR_WHITE;
        graph->v[i]->dist = INT_MAX;
        graph->v[i]->parent = NULL;
    }
    
    Queue q;
    init(&q);

    start->color = COLOR_GRAY;
    start->dist = 0;
    start->parent = NULL;

    Enqueue(&q, start);

    // BFS traversal
    while (!isEmpty(&q)) 
    {
        Node *u = Dequeue(&q);
        // Check neighbors
        for (int i = 0; i < u->adjSize; i++) 
        {
            Node *v = u->adj[i];
            if (v->color == COLOR_WHITE) 
            {
                v->color = COLOR_GRAY;
                v->dist = u->dist + 1;
                v->parent = u;
                Enqueue(&q, v);
            }
        }
        // mark u as processed
        u->color = COLOR_BLACK;

        // if end node is reached, reconstruct and return the length of the path
        if (u == end) 
        {
            int path_length = u->dist + 1;
            if (path != NULL) 
            {
                // reconstruct the path
                Node *temp = u;
                for (int j = path_length - 1; j >= 0; j--) 
                {
                    path[j] = temp;
                    temp = temp->parent;
                }
            }
            return path_length;
        }
    }

    // no path found
    return -1;
}

typedef struct random_struct{
    int x;
    int y;
}random_struct; //pt duplicate

bool findInStruct(random_struct randomStruct[], int size, random_struct key)
{
    for(int i = 0; i < size; i++){
        if((key.x == randomStruct[i].x && key.y == randomStruct[i].y) || (key.y == randomStruct[i].x && key.x == randomStruct[i].y)){
            return true;
        }
    }
    return false; //
}

void performance()
{
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for(n=1000; n<=4500; n+=100){
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected

        
        for(int ii = 0; ii < graph.nrNodes - 1; ii++) {
            graph.v[ii]->adjSize = 1;
            graph.v[ii]->adj = (Node **) malloc(sizeof(Node *) * graph.v[ii]->adjSize);
            graph.v[ii]->adj[0] = graph.v[ii + 1];
        } //graf conex 

        random_struct randomStruct[10000];
        int size = 0;

        int k = n - graph.nrNodes;
        while(k != 0) {
            int indice2 = rand() % (graph.nrNodes - 1);
            int indice = rand() % (graph.nrNodes - 1);
            random_struct element = {indice, indice2};
            while(findInStruct(randomStruct, size, element)) {
                indice2 = rand() % (graph.nrNodes - 1);
                indice = rand() % (graph.nrNodes - 1);
                element = {indice, indice2};
            }
            randomStruct[size++] = {indice, indice2};
            Node *buffer = graph.v[indice2];
//            int indice = 0;
            graph.v[indice]->adjSize++;
            graph.v[indice]->adj = (Node **) realloc(graph.v[indice]->adj, graph.v[indice]->adjSize * sizeof(Node *));
            graph.v[indice]->adj[graph.v[indice]->adjSize - 1] = buffer;
            k--;
        }

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for(n=100; n<=200; n+=10){
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected
        
        for(int ii = 0; ii < graph.nrNodes - 1; ii++) {
            graph.v[ii]->adjSize = 1;
            graph.v[ii]->adj = (Node **) malloc(sizeof(Node *) * graph.v[ii]->adjSize);
            graph.v[ii]->adj[0] = graph.v[ii + 1];
        }

        random_struct randomStruct[10000];
        int size = 0;

        int k = 4500 - graph.nrNodes;
        while(k != 0) {
            int indice2 = rand() % (graph.nrNodes - 1);
            int indice = rand() % (graph.nrNodes - 1);
            random_struct element = {indice, indice2};
            while(findInStruct(randomStruct, size, element)) {
                indice2 = rand() % (graph.nrNodes - 1);
                indice = rand() % (graph.nrNodes - 1);
                element = {indice, indice2};
            }
            randomStruct[size++] = {indice, indice2};
            Node *buffer = graph.v[indice2];
//            int indice = 0;
            graph.v[indice]->adjSize++;
            graph.v[indice]->adj = (Node **) realloc(graph.v[indice]->adj, graph.v[indice]->adjSize * sizeof(Node *));
            graph.v[indice]->adj[graph.v[indice]->adjSize - 1] = buffer;
            k--;
        }


        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}
