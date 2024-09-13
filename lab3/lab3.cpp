#include <stdio.h>
#include "Profiler.h"
# define max_size 10000
#define step_size 100
#define M 20

Profiler P("lab3") ;

void exchange (int *a , int *b)
{
    int aux = *a ;
    *a = *b ;
    *b = aux ;
}

int Partition_last(int A[] , int p , int r, const int N)
{
    Operation opComparation = P.createOperation ("Quick_Sort_Comparations", N);
    Operation opAssign = P.createOperation ("Quick_Sort_Assigns", N);
    int pivot = A[r];
    int i , j ;
    i = p - 1;
    for (j = p ; j < r ; j++)
    {
        if ( A[j] <= pivot)
        {
            opComparation.count();
            i++ ;
            exchange(&A[i] , &A[j]);
            opAssign.count(3);
        }
    }
    exchange (&A[i + 1] , &A[r]) ;
    opAssign.count(3);
    return (i + 1) ;
}

int Partition_middle(int A[] , int p , int r, int n)
{
    Operation opComparation = P.createOperation ("Quick_Sort_Comparations", n);
    Operation opAssign = P.createOperation ("Quick_Sort_Assigns", n);
    int i , j ;
    int middle = (p + r) / 2 ;
    int pivot = A[middle];
    std::swap(A[middle], A[r]);
    i = p - 1;
    for (j = p ; j < r ; j++)
    {
        if ( A[j] < pivot)
        {
            opComparation.count();
            i++ ;
            exchange(&A[i] , &A[j]);
            opAssign.count(3);
        }
    }
    exchange (&A[i + 1] , &A[r]) ;
    opAssign.count(3);
    return (i + 1) ;
}


void Quick_Sort_last (int A[] , int p , int r , const int N)
{
    if ( p < r )
    {
        int q = Partition_last(A, p, r, N);
        Quick_Sort_last(A , p , q - 1 , N);
        Quick_Sort_last(A , q + 1 , r , N );
    }

}

void Quick_Sort_middle (int A[] , int p , int r, int n)
{

    if ( p < r )
    {
        int q = Partition_middle(A, p, r, n);
        Quick_Sort_middle(A , p , q - 1, n);
        Quick_Sort_middle(A , q + 1 , r, n);
    }
}

int comand (int i , int c )
{
    if (c == 0) return (i - 1) / 2; //parent
    else if (c == 1) return (2 * i) + 1; // left child
    else if (c == 2) return (2 * i) + 2; //right child
    else
    {
        printf("error");
        exit(1);
    }

}

void Heapify(int v[], int n, int i, const int N)
//actually we have to heapify a sub-tree so the "root" from the function it s the root of sub-tree
{
    Operation opComparation = P.createOperation ("Heap_Sort_Comparations", N);
    Operation opAssign = P.createOperation ("Heap_Sort_Assigns", N);
    int bigg = i; // "biggest" value between parent and it's children
    int l = comand(i, 1);
    int r = comand(i, 2); // i' s children
    if (l < n)
    {
        opComparation.count();
        if (v[l] > v[bigg])
        {
            bigg = l;
        }
    }
    if (r < n)
    {
        opComparation.count();
        if (v[r] > v[bigg])
        {
            bigg = r;
        }
    }
    if (bigg != i) //parent not "bigger" than it's children
    {

        exchange (&v[i], &v[bigg]);
        opAssign.count(3);
        Heapify (v, n, bigg, N);
    }
}


void Bottom_up(int v[], int n)
{
    int start = (n - 1) / 2 , i;
    for (i = start ; i >= 0 ; i--)
    {
        Heapify(v, n, i, n);
    }
}

void Heap_Sort(int v[], int n, const int N)
{
//Operation opComparation =P.createOperation ("Heap_Sort_Comparations",n);
    Operation opAssign = P.createOperation ("Heap_Sort_Assigns", N);
    Bottom_up(v, n);
    for (int j = n - 1 ; j > 0 ; j--)
    {
        exchange(&v[0] , &v[j]);
        opAssign.count(3);
        n--;
        Heapify(v, n, 0, N);
    }
}



void Show_Array(int A[] , int n)
{
    int i;
    for (i = 0 ; i < n ; i++)
    {
        printf("%d " , A[i]) ;
    }
    printf("\n");
}

void InsertionSort_REC (int arr[], int n)
{
    Operation opComparation = P.createOperation("InsertionSort_comp_REC",n);
    Operation opAssign = P.createOperation("InsertionSort_assign_REC",n);
//  base case
    if (n < 1)
    {
//  printf("error") ;
        return ;
    }
    InsertionSort_REC (arr , n - 1) ;
    int l = arr[n - 1];
    opAssign.count();
    int j = n - 2;
    while (j >= 0 && arr[j] > l)
    {
        opComparation.count();
        arr[j + 1] = arr[j];
        opAssign.count();
        j-- ;
    }
    arr[j + 1] = l ;
    opAssign.count();
}

void InsertionSort_IT(int arr[], int n)
{
    Operation opComparation = P.createOperation("InsertionSort_comp_IT",n);
    Operation opAssign = P.createOperation("InsertionSort_assign_IT",n);
    for(int i=1;i<n;i++)
    {
        int j=i-1;
        opAssign.count();
        int t=arr[i];
        opComparation.count();
        while ( j>=0&&arr[j]>t )
        {  
            opComparation.count();
            arr[j+1]=arr[j];
            opAssign.count();
            j--;
        }
        arr[j+1]=t;
        opAssign.count();
    }

}

int QuickSelect(int A[], int p, int r, int k, const int N) {
    if (p == r) {
        return A[p];
    }

    int q = Partition_last(A, p, r, N);
    int leftLength = q - p + 1;

    if (k == leftLength) {
        return A[q];
    } else if (k < leftLength) {
        return QuickSelect(A, p, q - 1, k, N);
    } else {
        return QuickSelect(A, q + 1, r, k - leftLength, N);
    }
}

void demo()
{
    int A1[] ={2,8,7,1,3,5,6,4} ;
    int A2[] ={2,8,7,1,3,5,6,4} ;
    int A3[] ={2,8,7,1,3,5,6,4} ;
    int A4[] ={2,8,7,1,3,5,6,4} ;
    int A5[] ={2,8,7,1,3,5,6,4} ;
    int A6[] ={1,5,3,2,8,9,20,13} ; // 1 2 3 5 8 9 13 20 
    int n =sizeof(A1)/sizeof(A1[0]);
    printf("My array is : ") ;
    Show_Array(A1 ,n) ;

    Quick_Sort_last (A1 , 0 , n-1 ,n ) ; 
    printf("My array sorted with QS_last is : ") ;
    Show_Array (A1,n);

    Quick_Sort_middle(A2 ,0 ,n-1 ,n);
    printf("My array sorted with QS_middle  is : ") ;
    Show_Array (A2,n);

    Heap_Sort(A3,n,100);
    printf("My array sorted with HS is : ") ;
    Show_Array (A3,n);

    InsertionSort_IT(A4,n);
    printf("My array sorted with IS_IT is : ") ;
    Show_Array (A4,n);

    InsertionSort_REC(A5,n);
    printf("My array sorted with IS_REC is : ") ;
    Show_Array (A5,n);

    int k=4; //pozitia 
    int r=QuickSelect(A6,0,n-1,k,n); // nr de pe pozitia k in vectorul "ordonat"
    printf("The element %d is located at position %d ." , r , k) ;
    printf("\n");
    
}


//pivot =last element
void perf_average_worst(int order)
{
    int X[max_size];
    int n ;
    for ( n = step_size ; n <= max_size ; n += step_size )
    {
        for ( int test = 0 ; test <= M ; test++ )
        {
            FillRandomArray(X, n, 10, 50000, false, order);
            Quick_Sort_last(X , 0 , n - 1 , n) ;
        }
    }
//average
    P.divideValues("Quick_Sort_Comparations", M);
    P.divideValues("Quick_Sort_Assigns", M);

    P.addSeries("Total_Quick_Sort" , "Quick_Sort_Comparations" , "Quick_Sort_Assigns" );
//P.createGroup("Average_Comparations","Heap_Sort_Comparations" , "Quick_Sort_Comparations" );
    P.showReport();
}

//best case
void perf_best ()
{
    int X[max_size];
    int n ;
    for ( n = step_size ; n <= max_size ; n += step_size )
    {
        for ( int test = 0 ; test <= M ; test++ )
        {
            FillRandomArray(X, n, 10, 50000, false, ASCENDING);
            Quick_Sort_middle(X , 0 , n - 1 , n) ;
        }
    }

//best
    P.divideValues("Quick_Sort_Comparations", M);
    P.divideValues("Quick_Sort_Assigns", M);

    P.addSeries("Total_Quick_Sort" , "Quick_Sort_Comparations" , "Quick_Sort_Assigns" );

    P.showReport();
}

//QuickSort vs HeapSort
void perf_QS_vs_HS (int order)
{
    int X[max_size];
    int Y[max_size];
    int n ;
    for ( n = step_size ; n <= max_size ; n += step_size )
    {
        for ( int test = 0 ; test <= M ; test++ )
        {
            FillRandomArray(X, n, 10, 50000, false, order);
            Quick_Sort_last(X , 0 , n - 1 , n) ;
            memcpy(Y, X , n * sizeof(int)) ;
            Heap_Sort(Y , n, n);
        }
    }

//best
    P.divideValues("Quick_Sort_Comparations", M);
    P.divideValues("Quick_Sort_Assigns", M);
    P.divideValues("Heap_Sort_Comparations", M);
    P.divideValues("Heap_Sort_Assigns", M);

    P.addSeries("Total_Quick_Sort" , "Quick_Sort_Comparations" , "Quick_Sort_Assigns" );
    P.addSeries("Total_Heap_Sort" , "Heap_Sort_Comparations" , "Heap_Sort_Assigns" );

    P.createGroup("Total_Assigns" , "Quick_Sort_Assigns" , "Heap_Sort_Assigns" ) ;
    P.createGroup("Total_Comparations", "Quick_Sort_Comparations" , "Heap_Sort_Comparations" );
    P.createGroup("TOTAL" , "Total_Quick_Sort" , "Total_Heap_Sort");
    P.showReport();
}


void perf_Time_IT_vs_REC(int order)
{
    int A[max_size];
    int X[max_size];
    int Y[max_size];
    int n ;
    for ( n = step_size ; n <= max_size ; n += step_size )
    {
        FillRandomArray(A, n, 10, 50000, false, order);
        P.startTimer("iterativ_time", n);
        for ( int test = 0 ; test <= M ; test++ )
        {
            memcpy(X, A , n * sizeof(int));
            InsertionSort_IT(X, n) ;   
        }
            P.stopTimer("iterativ_time", n);

            P.startTimer("recursiv_time", n);
            for (int test = 0 ; test <= M ; test++)
            {
                memcpy(Y, A , n * sizeof(int));
                InsertionSort_REC(Y, n) ;
            }
            P.stopTimer("recursiv_time", n);
    }
    for( n = step_size ; n <= max_size ; n += step_size )
    {
        FillRandomArray(A, n, 10, 50000, false, order);
        for ( int test = 0 ; test <= M ; test++ )
        {
            memcpy(X, A , n * sizeof(int));
            InsertionSort_IT(X, n) ;  
            memcpy(Y, A , n * sizeof(int));
            InsertionSort_REC(Y, n) ;    
        }
    }
   // P.divideValues("Insertion_Sort_recursiv_time", M);
   // P.divideValues("Insertion_Sort_iterativ_time", M);

    P.divideValues("InsertionSort_comp_IT", M);
    P.divideValues("InsertionSort_assign_IT", M);

    P.divideValues("InsertionSort_comp_REC", M);
    P.divideValues("InsertionSort_assign_REC", M);

    P.addSeries("total_IT", "InsertionSort_comp_IT" ,"InsertionSort_assign_IT" ) ;
    P.addSeries("total_REC", "InsertionSort_comp_REC" ,"InsertionSort_assign_REC" ) ;

    P.createGroup("total","total_IT" ,"total_REC" );
    P.createGroup("total_assign","InsertionSort_assign_IT", "InsertionSort_assign_REC");
    P.createGroup("total_comp","InsertionSort_comp_IT", "InsertionSort_comp_REC");
    
    P.createGroup("TOTAL" , "iterativ_time" , "recursiv_time" ) ;
       
    P.showReport();
}

void perf_QS()
{

    perf_average_worst(UNSORTED);
    P.reset("worst qs") ;
    perf_average_worst(DESCENDING);
    P.reset("best qs") ;
    perf_best();
}
void perf_time()
{
    P.reset("UNSORTED");
    perf_Time_IT_vs_REC(UNSORTED) ;
    P.showReport();
}

void perf_QS_vs_HS()
{
    P.reset("average");
    perf_QS_vs_HS(UNSORTED);
    P.reset("worst");
    perf_QS_vs_HS(DESCENDING);
    P.reset("best");
    perf_QS_vs_HS(ASCENDING);
    P.reset("time");
}

int main()
{
    demo();
    //perf_QS();
   //perf_QS_vs_HS();
    //perf_time();

    return 0;
}

