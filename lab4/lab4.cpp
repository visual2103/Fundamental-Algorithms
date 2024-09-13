#include <stdio.h>
#include <stdlib.h>

#include "Profiler.h"

#define maxSize 10000
#define stepSize 100
#define M 5

Profiler p ("lab 4") ;


typedef struct Node 
{
    int info;
    struct Node* next ;   
} Node ;

typedef struct List
{
    struct Node* head ;
    struct Node* tail ;
}List;


int comand (int i , int c )
{
    if (c==0) return  (i-1)/2; //parent 
    else 
        if (c==1) return (2*i)+1; // left child
        else 
            if (c==2) return (2*i)+2; //right child
            else 
                {
                    printf("error");
                    exit(1);
                }
    
}

void exchange(Node **a ,Node **b )
{
    Node *aux = *a;
    *a = *b;
    *b = aux;
}

                                    
void minHeapify(Node *v[],int dim,int i , Operation op) //O(k)
//actually we have to heapify a sub-tree so the "root" from the function it s the root of sub-tree
{
    int minn=i; //  
    int l=comand(i,1);
    int r=comand(i,2); 
    if (l<dim)
    {
        op.count();
        if  ((v[l])->info < (v[minn])->info)
        {
            minn=l;
        }
    }
    if (r<dim) 
    {
        op.count();
        if ((v[r])->info < (v[minn])->info)
        {
            minn=r;
        }
    }
    if (minn!=i) 
    {
        
        exchange (&v[i] , &v[minn]);
        op.count(3);
        minHeapify (v,dim,minn , op);
    }
}

void Build_MyHeap(Node *v[],int dim , Operation op)
{
    int start=(dim-1)/2 ;
    for (int i=start ; i>=0 ;i--)
    {
        minHeapify(v,dim,i ,op);
    }
}

Node* createNode(int info)
{
    Node *node =(Node*)malloc(sizeof(Node)) ;
    node->info=info;
    node->next=NULL;
    return node ;
}
void Insert_First( Node **first , int info)
{
    Node *node=createNode(info) ;
    if ( *first == NULL )
        *first=node ;
    else 
        {
            node->next = *first;
            *first = node ;
        }
}

void print_MyList(Node *first)
{
    while (first !=NULL)
    {
        printf("%d " ,first->info);
        first=first->next;
    }
    printf("\n"); 
}

void create_Lists( List L[] ,int k ,int n )
{
    int A[n];
    for (int i=0 ;i < k-1 ;i++)
    {
        FillRandomArray(A , n/k ,10 ,500,false,DESCENDING);
        L[i].head=NULL;
        L[i].tail=NULL;
        for (int j=0 ; j< n/k ;j++)
        {
            Insert_First(&L[i].head,A[j] );
            
        }
    }
    // generate elem. for k list 
    FillRandomArray(A , (n/k)+(n%k) ,10 ,500,false,DESCENDING);
    L[k-1].head=NULL;
    for (int j=0 ; j< (n/k)+(n%k) ;j++)
        {
            Insert_First( &L[k-1].head , A[j]);
        }
}

List Merge_Sorted_Lists(List L[] , int k , const int n , Operation op)
{
    List Res ;
    Res.head=NULL;
    Res.tail=NULL;

    int heapSize = k ; // k-numb. of lists

    // Create an array A of size k to store the heads of the lists
    Node *A[k];
    for (int i = 0; i < k ; i++) 
    {
        op.count();
        A[i] = L[i].head;
    }

    // Construiește un heap min pentru vectorul A
    Build_MyHeap(A, heapSize,op);
    op.count();
    while (heapSize > 0) 
    {
        op.count(2);
        if (Res.tail != NULL) 
        {
            op.count();
           Res.tail ->next = A[0];
        } 
        else 
        {
            op.count();
            Res.head = A[0];
        }

       Res.tail  = A[0];
        A[0] = A[0]->next;
        op.count(2);

        op.count();
        if (A[0] == NULL) 
        {
            op.count(2);
            A[0] = A[heapSize - 1];
            heapSize--;
        }
        op.count();
        if (heapSize > 1) 
        {
            minHeapify(A, heapSize, 0,op);
        }
    }
    
    Res.tail->next=NULL;
    op.count();

    return Res;
}    

void free_my_memory(List L[] , int k)
{
    for(int i=0 ; i<k ; i++)
    {
        Node *x=L[i].head;
        while (x != NULL)
        {
            Node *t=x;
            x=x->next;
            free(t);
        }
    }
    L->head=NULL;
    L->tail=NULL;
}


void perf()
{
    int n=0 ;
    int K[3]={5 , 10 , 100};
    List lists[maxSize] ;
    List Res;
    for (int i=0 ;i < maxSize ;i++)
    {
        lists[i].head=NULL ;
        lists[i].tail=NULL ;
    }

    for (int i=0; i<3 ;i++)
    {
        for( n=stepSize ; n<=maxSize ; n+=stepSize )
            for (int test= 0 ; test<M ; test++)
            {
                create_Lists(lists , K[i] , n) ;
                if (i==0)
                {
                    Operation op =p.createOperation ("Operations k1",n); 
                    Res=Merge_Sorted_Lists(lists , K[i] , n , op);
                }
                if (i==1)
                {
                    Operation op =p.createOperation ("Operations k2",n); 
                    Res=Merge_Sorted_Lists(lists , K[i] , n , op);
                }
                if (i==2)
                {
                    Operation op =p.createOperation ("Operations k3",n); 
                    Res=Merge_Sorted_Lists(lists , K[i] , n , op);
                }
                
            }
    }
    p.divideValues("Operations k1",M);
    p.divideValues("Operations k2",M);
    p.divideValues("Operations k3",M);
    p.createGroup("Total" ,"Operations k1","Operations k2" ,"Operations k3");
    p.showReport();

    p.reset("n fix");
    n=maxSize ;
     for( int k=10 ; k<=500 ; k+=10)
            for (int test= 0 ; test<M ; test++)
            {
                create_Lists(lists ,k , n) ;
                Operation op =p.createOperation ("Operations n",k); 
                Res=Merge_Sorted_Lists(lists , k , n , op);
            }
     p.divideValues("Operations n",M);
     p.showReport();

}

void demo()
{
    Operation op=p.createOperation("Op" , 0);
    int n=10;
    int k=3;
    List Lists[maxSize];
    List Res;
    create_Lists(Lists, k ,n ) ;
    for(int i=0 ; i<k ; i++)
    {
        print_MyList(Lists[i].head);
    }
    Res=Merge_Sorted_Lists(Lists , k , n ,op);
    print_MyList(Res.head);
}

int main()
{
    demo();
    //perf();
    return 0;
}

