
#include <stdio.h>
#include "Profiler.h" 
#define M 5 //numb of tests
#define max_size 10000
#define step_size 100

Profiler p("lab2");



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

/*int parent(int i)
{
    return (i-1)/2;
}

int left(int i)
{
    return (2*i)+1;
}

int right(int i)
{
    return (2*i)+2;
}
*/


void exchange(int *a ,int *b )
{
    int aux=*a;
    *a = *b;
    *b = aux;
}

// to mentain Max-Heap propriety 
/*for a single node takes O(log n) time complexity n-numb. of nodes 
and for the entire heap =>O(n log n)* -explaination : call Heapify costs O(lg n)
                                                    : call Built-Heap(not here)-costs O(n)
                                    */
void Heapify(int v[],int dim,int i )
//actually we have to heapify a sub-tree so the "root" from the function it s the root of sub-tree
{
    Operation opComparation =p.createOperation ("Bottom_Up_Comparations",dim);
    Operation opAssign =p.createOperation ("Bottom_Up_Assigns",dim);
    int bigg=i; //  "biggest" value between parent and it's children 
    int l=comand(i,1);
    int r=comand(i,2); // i' s children
    if (l<dim)
    {
        opComparation.count();
        if  (v[l]>v[bigg])
        {
            bigg=l;
        }
    }
    if (r<dim) 
    {
        opComparation.count();
        if (v[r]>v[bigg])
        {
            bigg=r;
        }
    }
    if (bigg!=i) //parent not "bigger" than it's children 
    {
        
        exchange (&v[i],&v[bigg]);
        opAssign.count(3);
        Heapify (v,dim,bigg);
    }
}
/*
void Build_My_Heap(int v[],int dim)
{   
    int start=(dim/2)-1; // ! work with command 
    int i;
    for(i=start ; i>=0 ; i--) 
    {
        Heapify (v , dim , i) ;
    }
}
*/

void Bottom_up(int v[],int dim)
{
    /*here we need the last non-leaf node =>it means the parent of last-node
    so we will find this at the index (dim-1)-1)/2 =(dim/2)-1 start index here 
*/
    int start=(dim-1)/2 , i;
    for (i=start ; i>=0 ;i--)
    {
        Heapify(v,dim,i);
    }
}

void Push_My_Heap(int v[] , int dim , int i)
{
    Operation opComparation =p.createOperation ("Top_Down_Comparations",dim);
    Operation opAssign =p.createOperation ("Top_Down_Assigns",dim);
    // overflow verif !!! max_size 
    int x = comand(i,0); 
    while (i>0)
    {
         opComparation.count();
        if ( v[x] < v[i] )
        {
            exchange(&v[i],&v[x]);
            opAssign.count(3);
            i=x;
            x = comand(i,0);
        }
        else break; //second cond is false 
    }
    //return heap_size;
}
// v contine elementele ce le pun in heap
// w vectorul in care construiesc heap ul
void Top_Down(int v[], int w[] , int dim )
{
    //int heap_size=0;
    for(int i=1; i<dim ; i++)
    {
        Push_My_Heap(v , dim , i) ;
    }
}

void Heap_Sort(int v[],int dim )
{
    //uncountable 
    Bottom_up(v,dim);
    int j;
    for (j=dim-1 ; j>0 ; j--)
    {
        exchange(&v[0] ,&v[j]);

        dim--;
        Heapify(v,dim,0);
    }
}

/*top down (td) aproach is less efficient than bottom up (bu) because :
top dowm -priority queues
bottom up -sorting 
1.numb of comparisons: bu requires fewer than td (binary tree minimize the number of swaps and comparisons)
2.memory acces : bu builds the heaps from the leaves up to the root => all the data are close in memory 
                 td starts from the root and works down to the leaves -> more cache misses and slower 
                                                                         performance (large heaps)  */
void Show_My_Heap (int v[],int n)
{
    for (int i=0 ;i<n ;i++)
    {
        printf("%d ",v[i]);
    }
    printf("\n");
}


void pprint(int a[],int n,int k,int level=0)
{
    int i;
    for (i=0 ; i<level ; i++)
    {
        printf("    ");
    }
    printf("%d\n ", a[k]) ;
    int l=comand(k,1);
    int r=comand(k,2);
    if (l<n) 
        pprint(a,n,l,level+1);
    if (r<n) 
        pprint(a,n,r,level+1);
}

void demo ()
{
    int v2[]={4,2,5,1,8,6,9,3};
    int v3[]={4,2,5,1,8,6,9,3};
    int v4[]={4,2,5,1,8,6,9,3};
    int dim=sizeof(v2)/sizeof(v2[0]);
    printf("My heap : ") ; 
    Show_My_Heap(v2,dim);
    pprint(v2,dim,0);
    Heap_Sort(v2,dim);
    printf("New heap with Heap Sort :");
    Show_My_Heap(v2,dim);
    pprint(v2,dim,0);
    Bottom_up(v3,dim);
    printf("New heap with Bottom Up :");
    Show_My_Heap(v3,dim);
    pprint(v3,dim,0);
    int w[] = {0};
    Top_Down(v4,w,dim);
    printf("New heap with Top Down :");
    Show_My_Heap(v4,dim);
    pprint(v4,dim,0);
}

//average case 
void perf(int order)
{
    int X[max_size];
    int Y[max_size];
    int w[max_size];
    int dim ;
    for( dim=step_size ; dim<=max_size ; dim+=step_size )
    {
        for ( int test= 0 ; test<= M ; test++ )
        {
            FillRandomArray(X,dim,10,50000,false,order);
            for(int i=0 ; i<dim ; i++)
            {
                Y[i]=X[i];
            }
            Bottom_up(X,dim);
            Top_Down(Y,w,dim);
        }
    }
    //average
    p.divideValues("Bottom_Up_Comparations",M);
    p.divideValues("Bottom_Up_Assigns",M);
    p.divideValues("Top_Down_Comparations",M);
    p.divideValues("Top_Down_Assigns",M);

    p.addSeries("Total_Bottom_Up" , "Bottom_Up_Comparations" , "Bottom_Up_Assigns" );
    p.addSeries("Total_Top_Down" , "Top_Down_Comparations" , "Top_Down_Comparations" );

    p.createGroup("Average_Comparations","Bottom_Up_Comparations" , "Top_Down_Comparations" );
    p.createGroup("Average_Assigns","Bottom_Up_Assigns" , "Top_Down_Assigns" );
    p.createGroup("Average_Total" ,"Total_Bottom_Up" , "Total_Top_Down" ) ;
    
}

void perf_all()
{
    perf(UNSORTED);
    p.reset("Worst");
    perf(ASCENDING);
    p.showReport();
}

int main()
{
    demo();
    //perf_all();
    return 0;
}
