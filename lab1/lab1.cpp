/*
LAB 1 -AF 11.10.2023 -METODE DE SORTARE

Avem 2 tipuri de strategii: metode bazate pe imaprtire logica si cele bazate pe adiacenta 
1.	Impartire logica
Avem un tablou de n elemente pe care l privim “logic” in 2 parti : o zona numita destinatie (zona deja ordonata – la inceput are dimensiunea 0 ) si zona  sursa care e neordonata. 
Obiectivul nostru este ca la fiecare pas sa extragem cate un element din sursa si se adauga in destinatie , ceea ce face ca la fiecare pas dimensiunea destinatiei creste si cea a sursei scade  => Nr finit de pasi adica n pasi 

A.	Insertion Sort 
Usor -> partea de extragere in O(1) a oricarui element din sursa datorita faptului ca avem acces direct la pozitie (index) , insa vom selecta elementul v[i] , primul element din sursa– il punem intr-un  buffer si l punem la locul lui potrivit . Vom determina locul sau potrivit la un index j , a i elemental de pe j-1 e mai mic decat el si j+1 e mai mare decat el (si egal-insa sa pastram cumva stabilitate ape termen lung) .
Modul de a determina locul potrivit  (j) cu :
	a.Cautare liniara : folosim un iterator de la stanga spre dreapta , cat timp buffer ul e mai mic decat pozitia curenta , avansez pana cand e mai mare decat buffer ul 
	b. Cautare binara : -se opreste cand  Buffer ul e egal cu elemental sau cand capetele s au interschimbat .

Am determinat locatia =>
Modul de a pune la pozitia j elementul selectat din sursa : shiftare la dreapta => incepem de la cel mai din dreapta , locul gol se deplaseaza la stanga o data cu deplasarea elementelor la dreapta => adaugarea dureaza O(1) si extragerea in O(i)  

Despre cum functioneaza ?


Presupunem ca elementul de pe prima pozitie v[0] se afla la locul lui , vom continua sa sortam vectorul incepand cu elemental  v[1] . Luam elementul “curent” , v[i] si l memoram intr o variabila x –“cheie” ,  j e pozitia ultimului element “sortat” (aflat la locul lui pana in prezent ) .
v[j+1]=v[j] presupune plasarea “cheii” dupa elemental imediat mai mic decat el , “cel mai mare numar , mai mic decat decat cheia “

ex: v={4,1,5,7,3}
dupa iteratia 1=> v={1,4,5,7,3}
dupa iteratia 2=> v={1,4,5,7,3}
dupa iteratia 3=> v={1,4,5,7,3}
dupa iteratia 4=> v={1,3,4,5,7}

	My sorted array is: 1 , 3 , 4 ,5 ,7

Despre variabile
x- “cheia” : 
i- indicele cartii care urmeaza sa fie inserata in vectorul sortat
j- indice stanga /dreapta

Complexitate: O(n2)
 Primul for efectueaza O(n) iteratii , iar al doilea tot O(n) – in cel mai rau caz . 


Optimizari:
Stim ca cautarea binara se poate face doar pe un vector ordonat , cum elementele din partea stanga sunt ordonate , putem reduce O(n) la O(log n), dar complexitatea finala nu va fi modificata deoarece permuatrea elementelor cu o pozitie presupune un consum de O(n) operatii. 

Util cand avem un nr mic de obiecte 



B.	Selection Sort 
 vrem sa facem extragerea usoara , asadar vom selecta cel mai mic element din sursa , parcurg sursa o data , determin pozitia minimului , dupa ce am terminat de parcurs sursa , interschimb 

Diferente intre SelectionSort si InsertionSort 
SS -presupune mai putini pasi decat IS dar mai multe comparatii.
SS- verifica sursa pt determinarea minimului in timp ce IS parcurge destinatia pt a gasi pozitia corecta la care va fi inserat primul element din sursa.

Cazuri:
	A. AVERAGE  case  : vector de la input  nesortat (random numbers)
	B. BEST case :vectorul de la input e sortat => nr minim de swap uri
	C. WORST case :vectorul de la input e sortat invers (descrescator)

InsertionSort: 
Time complexity :
AVERAGE:O(n^2)
	            WORST:O(n^2)
	            BEST : O(n)    
SelectionSort: 
Time complexity :
AVERAGE:O(n^2)
	            WORST:O(n^2)
	            BEST : O(n)    

BubbleSort: 
Time complexity :
AVERAGE:O(n^2)
	            WORST:O(n^2)
	            BEST : O(n)    


Space complexity : O(1) – for all   

		


!! retinem ca : doar  realizate pe datele de intrare , sau date de acelasi tip ,
 nu contorizam indexi si iteratorii deoarece 
-sunt intregi , iar operatiile pe intregi sunt mai rapide , 
-cei ai buclelor se reprezinta intr o alta zona in registrii care sunt mai rapide decat memoria , operatiile sunt ordin de marime mai rapide motiv pt care nu le contorizam 






//"Easy-put algorithm "- Insertion_Sort(=O(n)-best, O(n^2)-average, O(n^2)-worst
//"Easy-pick algorithm"-selection sort =O(n^2)-best, O(n^2)-average, O(n^2)-worst
//BubbleSort=O(n)-best, O(n^2)-average, O(n^2)-worst
// BinaryInsertionSort=O(n*log n)-best, O(n^2)-average, O(n^2)-worst

//la bubble sort recursive desi, daca ar fi best case, nu am avea avantajul implementarii iterative, dar numarul pasilor recursivi se decrementeaza


          int BinarySearch(int v[],int n, int x , int r , int l)
    {
        // r-right ; l-left m-middle 
        Operation opC =p.createOperation("SelectionSort_comp",n);
        //Operation opA =p.createOperation("SelectionSort_assign",n);
        int m;
        while (l<r)
        {
            m=(l+r)/2;
            if(x==v[m]) 
            {
                return m;
                opC.count();
            }
            else 
                if(x>=v[m])  
                {
                    l=m+1;
                    opC.count();
                }
                else r=m-1;
        }
        return r;
    }
    //O(n*log n)-BEST, O(n^2)-WORST, O(n^2)-AVERAGE
    void Binary_Insertion_Sort(int v[], int n)
    {
        Operation opC =p.createOperation("SelectionSort_comp",n);
        Operation opA =p.createOperation("SelectionSort_assign",n);
        int x , position;
        for (int i=1;i<n;i++)
        {
            x=v[i];
            opA.count();
            int j=i-1;
            position=BinarySearch(v,n,x,0,j);
            opC.count();
            while(j>=position)
            {
                opC.count();
                v[j+1]=v[j]; //shift
                j--;
            }
            if (j!=i-1)  // overwrite sometimes
            {
                  v[j+1]=x;
                  opA.count();
            }

            
        }

    }
	

*/

#include <stdio.h>
    #include <stdlib.h>
    #include "Profiler.h"
    #define MAX_SIZE 10000
    #define STEP_SIZE 100
    #define M 5 

    Profiler p("lab1_1");

    void exchange (int *x, int *y)
    {
        int aux = *x;
        *x = *y;
        *y = aux;
    }




    void SelectionSort (int v[],int n)
    {
        Operation opC =p.createOperation("SelectionSort_comp",n);
        Operation opA =p.createOperation("SelectionSort_assign",n);
        int m;
        for(int i=0 ; i <=n-1; i++)
        {
            m=i;
            for(int j=i+1;j<n ;j++)
            {
                opC.count();
                if (v[j]<v[m]) 
                {
                    m=j;
                    opA.count();
                }
            }
            exchange(&v[m],&v[i]); 
            opA.count(3);
        }
    }

void BubbleSort(int arr[], int n)
{
    Operation opC1 =p.createOperation("BubbleSort_comp",n);
    Operation opA1 =p.createOperation("BubbleSort_assign",n);
    int i, j;
    bool swapped;
    for (i = 0; i < n - 1; i++) {
        opA1.count();
        swapped = false;
        for (j = 0; j < n - i - 1; j++) {
            opC1.count();
            if (arr[j] > arr[j + 1]) 
            {
                opA1.count();
                opA1.count();
                opA1.count();
                opA1.count();
                int aux=arr[j];
                arr[j]=arr[j + 1];
                arr[j+1]=aux;
                swapped = true;
            }
        }
 
        // If no two elements were swapped
        // by inner loop, then break
        opC1.count();
        if (swapped == false)
            break;
    }
}

void InsertionSort(int arr[],int n){
    Operation opC3 =p.createOperation("InsertionSort_comp",n);
    Operation opA3 =p.createOperation("InsertionSort_assign",n);
    for(int i=1;i<n;i++)
    {
        int j=i-1;
        opA3.count();
        int t=arr[i];
        opC3.count();
        while(j>=0&&arr[j]>t)
        {  opC3.count();
            arr[j+1]=arr[j];
            opA3.count();
            j--;
        }
        arr[j+1]=t;
        opA3.count();
    }

}

    void ShowArray(int v[], int n)
    {
        //printf("Array is: ");
        for(int i=0;i<n;i++)
        {
            printf("%d ",v[i]);
        }
        printf("\n");

    }
    // demo for each algorithm implemented 
    void demo()
    {
        int v1[]={5,2,4,6,1,3};
        int v2[]={5,2,4,6,1,3};
        int v3[]={5,2,4,6,1,3};
        int n=sizeof(v1)/sizeof(v1[0]);
        printf("My array is : ");
        ShowArray(v1,n);
        SelectionSort(v1,n);
        printf("My sorted array with ss is :");
        ShowArray(v1,n);
        BubbleSort(v2,n);
        printf("My sorted array with bs is :");
        ShowArray(v2,n);
        InsertionSort(v3,n);
        printf("My sorted array with is is :");
        ShowArray(v3,n);
    }

void perf() 
{
    int v[MAX_SIZE], u[MAX_SIZE],w[MAX_SIZE];
    int n;
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
        FillRandomArray(v,n);//,10,50000,false,option);
        memcpy(u,v,n*sizeof(u[0]));
        memcpy(w,v,n*sizeof(u[0]));
        SelectionSort(v,n);
        BubbleSort(w,n);
        InsertionSort(u,n);
    }

    p.createGroup(" comp","BubbleSort_comp","SelectionSort_comp","InsertionSort_comp");
    p.createGroup(" assign","BubbleSort_assign","SelectionSort_assign","InsertionSort_assign");
   // p.addSeries("BubbleSort_assign","SelectionSort_assign","InsertionSort_assign");
    p.addSeries("total","BubbleSort_assign","BubbleSort_comp");
    p.addSeries("total","InsertionSort_assign","BubbleSort_comp");
    p.addSeries("total","SelectionSort_assign","BubbleSort_comp");
    p.showReport();


}

void best()
{
    int v[MAX_SIZE], u[MAX_SIZE],w[MAX_SIZE];
    int n;
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {

        FillRandomArray(v,n,10,1000,false,ASCENDING);
        memcpy(u,v,n*sizeof(u[0]));
        memcpy(w,v,n*sizeof(u[0]));
        SelectionSort(v,n);
        BubbleSort(w,n);
        InsertionSort(u,n);
    }


    p.createGroup("best comp","BubbleSort_comp","SelectionSort_comp","InsertionSort_comp");
    p.createGroup("best assign","BubbleSort_assign","SelectionSort_assign","InsertionSort_assign");
    //p.addSeries("BubbleSort_assign","SelectionSort_assign","InsertionSort_assign");
    p.showReport();

}

void worst()
{
    int v[MAX_SIZE], u[MAX_SIZE],w[MAX_SIZE];
    int n;
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {

        FillRandomArray(v,n,10,1000,false,DESCENDING);
        memcpy(u,v,n*sizeof(u[0]));
        memcpy(w,v,n*sizeof(u[0]));
        SelectionSort(v,n);
        BubbleSort(w,n);
        InsertionSort(u,n);
    }


    p.createGroup("worst comp","BubbleSort_comp","SelectionSort_comp","InsertionSort_comp");
    p.createGroup("worst assign","BubbleSort_assign","SelectionSort_assign","InsertionSort_assign");
    p.showReport();

}

int main()
{
    //demo(); //good 

    //perf();
    //best();
    worst();

    return 0;
}
