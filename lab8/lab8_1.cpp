#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler P("lab8.1");

#define step_size 100
#define max_size 10000
#define M 10
#define prag 10
#define DIM 40
#define time_test 1000
#define time_step 300
#define prag_limit 90
#define prag_test 10

void change (int *a ,int *b)
{
    int aux = *a ;
    *a = *b ;
    *b = aux ;
}

void InsertionSort_IT(int arr[],int s, int dim, int size)
{
    for(int i=s+1 ; i<=dim ; i++)
    {
        int j=i;
        int t=arr[i];
        P.countOperation("QS_HYBRID", size,2);
        while ( j>=s && arr[j-1]>t )
        {
            arr[j]=arr[j-1];
            P.countOperation("QS_HYBRID", size);
            j--;
            P.countOperation("QS_HYBRID", size);
        }
        arr[j]=t;
        P.countOperation("QS_HYBRID", size);
    }
}

int Partition_last(int A[] , int p , int r,int size)
{
    int pivot=A[r];
    P.countOperation("Quick Sort", size);
    int i =p ;

    for (int j=p ; j < r; j++)
    {
        P.countOperation("Quick Sort", size);
        if ( A[j] <= pivot)
        {
            change(&A[i] , &A[j]);
            i++ ;
            P.countOperation("Quick Sort", size, 3);
        }
    }
//    i++;
    change (&A[i] , &A[r]) ;
    P.countOperation("Quick Sort", size, 3);
    return i ;
}

int Partition_Hybrid(int arr[] , int p , int r,int size)
{
    P.countOperation("QS_HYBRID", size);
    int q =arr[r];
    int i =p ;

    for (int j=p;j<r;j++)
    {
        P.countOperation("QS_HYBRID", size);
        if (arr[j] <= q)
        {
            change(&arr[i] , &arr[j]);
            i++ ;
            P.countOperation("QS_HYBRID", size, 3);
        }
    }
    //i++;
    change (&arr[i] , &arr[r]) ;
    P.countOperation("QS_HYBRID", size, 3);
    return i ;
}

void Quick_Sort_last(int arr[] , int p , int r ,int size)
{   P.countOperation("Quick Sort", size);
    if(r> p)
    {
        int q = Partition_last(arr, p, r, size);
        P.countOperation("Quick Sort", size);
        Quick_Sort_last(arr, p, q - 1, size);
        Quick_Sort_last(arr, q + 1, r, size);
    }
    else
    {
        return ;
    }

}

void Quick_Sort_Hybrid (int arr[] , int p , int r ,int size  ,int treapta)
{
    //parcurge toate treptele 
    while(p < r) 
    {
        if (r - p < treapta) 
        {
            InsertionSort_IT(arr, p , r, size);
            return;
        }
        else
        {
            P.countOperation("QS_HYBRID", size);
            int q = Partition_Hybrid(arr, p, r, size);
            if(q-p<r-q)
            {
                Quick_Sort_Hybrid(arr, p, q - 1, size, treapta);
                p = q + 1;
            }
            else
            {
                Quick_Sort_Hybrid(arr, q + 1, r, size, treapta);
                r = q - 1;
            }
        }
    }
}


void printArray(int array[], int size)
{
    for(int i = 0; i < size; i++)
    {
        printf("%d ",array[i]);
    }
    printf("\n");
}

void demo()
{
    int arr[50];
    FillRandomArray(arr,50,0,100);
    int dim = sizeof(arr) / sizeof(arr[0]);

    Quick_Sort_Hybrid(arr, 0 , dim, dim,prag);
    printArray(arr, dim);

    int arr1[50];
    FillRandomArray(arr1,50,0,100);
    int dim1 = sizeof(arr1) / sizeof(arr1[0]);
    Quick_Sort_last(arr1, 0, dim1, dim1);
    printf("\n");
    printArray(arr1, dim1);

}

//op
void perf1()
{
    int i;
    for(i=step_size; i<max_size;i+=step_size)
    {
        int arr1[i];
        int arr2[i];
        FillRandomArray(arr1, i);
        memcpy(arr2, arr1, sizeof(int)*i ) ;
        //Quick_Sort_last(arr1, 0, i, i);
        Quick_Sort_Hybrid(arr2, 0, i, i,prag );
    }
    P.createGroup("TOTAL","Quick Sort","QS_HYBRID");
    P.showReport();
}

//time
void perf2()
{
    for (int i = time_step; i <= max_size; i += time_step)
    {
        int arr[i];
        P.startTimer("TIME_QS", i);
        for (int test = 0; test < time_test; ++test)
        {
            FillRandomArray(arr, i);
            Quick_Sort_last(arr, 0, i, i);
        }
        P.stopTimer("TIME_QS", i);

        P.startTimer("TIME_QS_HYBRID", i);
        for (int test = 0; test < time_test; ++test)
        {
            FillRandomArray(arr, i);
            Quick_Sort_Hybrid(arr , 0, i, i,prag);
        }
        P.stopTimer("TIME_QS_HYBRID", i);
    }
    P.createGroup("TOTAL_TIME", "TIME_QS", "TIME_QS_HYBRID");
    P.showReport();
}

void perf3()
{
    int arr[max_size] ;
    for (int i =0 ;i < prag_limit ; i++)
    {
        for (int j = 0 ; j< prag_test ; j++)
        {
            FillRandomArray(arr ,max_size );
            Quick_Sort_Hybrid(arr , 0 , max_size , i ,i) ;
        }
    }
    P.divideValues("QS_HYBRID" , prag_test);
    P.showReport();
}

int main()
{
    demo();
    // perf1();
    // perf2();
    //perf3();
    return 0;
}
