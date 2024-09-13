
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define Table_Size 11
#define  N 10007
#define m 3000
#define M 9
#define tests 5
#define Size 11
#include "Profiler.h"

int n_searches ;

Profiler p ("lab5") ;
typedef struct Entry
{
    int key;
    char name[30];
}Entry;


void show_myHash(Entry *A[] , int dim)
{
    for(int i=0 ; i < dim ;i++)
    {
        if (A[i]==NULL) 
        {
            printf(" null ") ;
        }
        else 
            printf("%d %s " , A[i]->key ,  A[i]->name) ;
    }
    printf("\n");
}

int hash_function(int x , int i , int dim )
{
    return ( x % dim + 5*i + 3*i*i ) % dim ;
}

int insert_H (Entry *H[] , int x , char name[] , int dim )
{
    int i=0;
    int j=0;
    do 
    {
        j=hash_function(x,i,dim);
        if (H[j]==NULL )
            {
                H[j]=(Entry*)malloc(sizeof(Entry));
                H[j]->key=x; 
                strcpy(H[j]->name , name);
                return j; //=pos where elem is ins.
            }
        else 
            if (H[j]->key==-2 )
            {
                H[j]->key=x; 
                strcpy(H[j]->name , name);
                return j;
            }
            else i++ ;
    }while (i!=dim);
    return -1 ; 
}

int search_H ( Entry *H[] , int x , int dim )
{
    int i=0 ;
    int j;
    do
    {
        j=hash_function(x ,i, dim);
        n_searches++ ; 
        if (H[j] != NULL && H[j]->key == x)
        {
            return j;
        }
        i++;
    } while (H[j]!=NULL && i!=dim);
    return -1;
}

void delete_H(Entry *H[] , int x , int dim)
{
    int pos=search_H(H , x , dim);
    if (pos != -1)
    {
        H[pos]->key=-2;
        strcpy(H[pos]->name ,"DEL" ) ;
    }
}

void show_myArray(int A[], int dim)
{
    for(int i=0 ;i< dim ;i++)
    {
        printf("%d " , A[i]);
    }
    printf("\n");
}


void demo() 
{
    Entry *A[Size]={NULL}; 
    char c[30];

    strcpy(c,"7");
    insert_H(A,7,c,Size);

    strcpy(c,"5");
    insert_H(A,5,c,Size);

    strcpy(c,"18");
    insert_H(A,18,c,Size);
    show_myHash(A,Size);

    int x=search_H(A, 18 ,Size);
    printf("\n%d" , x);
    printf("\n");

    delete_H(A, 18 , Size);
    show_myHash(A, Size);

    strcpy(c,"18");
    insert_H(A,18,c,Size);
    show_myHash(A,Size);

    strcpy(c,"29");
    insert_H(A,29,c,Size);
    show_myHash(A,Size);

    delete_H(A, 18 , Size);
    show_myHash(A, Size);

    int y=search_H(A, 18 ,Size);
    printf("\n%d" , y);
    printf("\n");
}

void perf ()
{
    int max_effort_found =0 ;
    double avg_effort_found =0 ;
    int max_effort =0 ;
    double avg_effort =0 ;
    double fill_factor[]={0.8 , 0.85 , 0.9 ,0.95 ,0.99};
    int size_factor=sizeof(fill_factor)/sizeof(fill_factor[0]);

    FILE *f =fopen("lab5.csv","w");

    for(int i=0 ; i < size_factor ; i++)
    {
        max_effort_found =0 ;
        avg_effort_found =0 ;
        max_effort =0 ;
        avg_effort =0 ;
        for(int j=0 ; j<tests ; j++)   
        {
            Entry* H[N] = {NULL};
            int intro_elem=N*fill_factor[i];
            int k = 0; // numara cate elem am introdus 
            int v[30000]={0}; //introduc elemente unice ( vector de frecventa )
            int ve[N] = {0}; // mem elem introduse 

            while (k < intro_elem)
            {
                int x = rand()%30000 ; // nr pe care vreau sa l introduc in tabel)
                if (v[x]==0 && insert_H(H , x ,"" , N)!=-1)
                {
                    v[x]++;
                    ve[k] = x;
                    k++;
                }
            }

            k=0;
            int search_elem[m];
            FillRandomArray(search_elem,m, 30000,50000,true ,UNSORTED); 
            while (k< m/2)
            {
                int x=rand()%intro_elem; // luam random pt a fi uniform 
                if (v[ve[x]]>0) // daca e folosit in H 
                {
                    search_elem[k]=ve[x];
                    v[ve[x]] = 0;
                    k++;
                }
            }
        
            for(int p=0 ;p<m;p++)
            {
                n_searches=0 ;
                if (search_H(H,search_elem[p],N) != -1)
                {
                    avg_effort_found +=n_searches; //daca l am gasit 
                    if (n_searches > max_effort_found) 
                    {
                        max_effort_found=n_searches;
                    }
                }
                else
                    {
                        avg_effort+=n_searches; //daca l am gasit 
                        if (n_searches > max_effort) 
                            {
                                max_effort=n_searches;
                            }
                    }
            }
        }
        avg_effort/=7500; //1500*5
        avg_effort_found/=7500;
        fprintf(f, "%f , %d , %f , %d , %f \n",fill_factor[i] ,max_effort_found ,avg_effort_found, max_effort ,avg_effort );

    }

    fclose(f);
}

//selectie ne uniforma , se iau primele 1500 de elem si numaram nr de acces
void perf2(){
        FILE *f =fopen("lab5_2.csv","w");

        int max_effort_found =0 ;
        double avg_effort_found =0 ;
        int max_effort =0 ;
        double avg_effort =0 ;
        for(int j=0 ; j<tests ; j++)   
        {
            Entry* H[N] = {NULL};
            int intro_elem=N*0.99;
            int k = 0; // numara cate elem am introdus 
            int v[30000]={0}; //introduc elemente unice ( vector de frecventa )
            int ve[N] = {0}; // mem elem introduse 

            while (k < intro_elem)
            {
                int x = rand()%30000 ; // nr pe care vreau sa l introduc in tabel)
                if (v[x]==0 && insert_H(H , x ,"" , N)!=-1)
                {
                    v[x]++;
                    ve[k] = x;
                    k++;
                }
            }

            k=0;
            int search_elem[m];
            FillRandomArray(search_elem,m, 30000,50000,true ,UNSORTED); 
            while (k< m/2)
            {
                
                search_elem[k]=ve[k];
                k++;
            }
        
            for(int p=0 ;p<m;p++)
            {
                n_searches=0 ;
                if (search_H(H,search_elem[p],N) != -1)
                {
                    avg_effort_found +=n_searches; //daca l am gasit 
                    if (n_searches > max_effort_found) 
                    {
                        max_effort_found=n_searches;
                    }
                }
                else
                    {
                        avg_effort+=n_searches; //daca l am gasit 
                        if (n_searches > max_effort) 
                            {
                                max_effort=n_searches;
                            }
                    }
            }
        }
        avg_effort/=7500; //1500*5
        avg_effort_found/=7500;
        fprintf(f, "%f , %d , %f , %d , %f \n",0.95 ,max_effort_found ,avg_effort_found, max_effort ,avg_effort );

        fclose(f);
}

void delete_op()
{
    FILE *f =fopen("lab5_delete.csv","w");

        int max_effort_found =0 ;
        double avg_effort_found =0 ;
        int max_effort =0 ;
        double avg_effort =0 ;
        for(int j=0 ; j<tests ; j++)   
        {
            Entry* H[N] = {NULL};
            int intro_elem=N*0.99;
            int final_intro_elem=N*0.8;
            int k = 0; // numara cate elem am introdus 
            int v[30000]={0}; //introduc elemente unice ( vector de frecventa )
            int ve[N] = {0}; // mem elem introduse 

            while (k < intro_elem)
            {
                int x = rand()%30000 ; // nr pe care vreau sa l introduc in tabel)
                if (v[x]==0 && insert_H(H , x ,"" , N)!=-1)
                {
                    v[x]++;
                    ve[k] = x;
                    k++;
                }
            }

            while(intro_elem > final_intro_elem)
            {
                int x=rand()%intro_elem; // luam random pt a fi uniform 
                if (v[ve[x]]>0) // daca e folosit in H 
                {
                    delete_H(H,ve[x],N);
                    v[ve[x]] = 0;
                    intro_elem--;
                }
            }

            k=0;
            int search_elem[m];
            FillRandomArray(search_elem,m, 30000,50000,true ,UNSORTED); 
            while (k< m/2)
            {
                
                int x=rand()%intro_elem; // luam random pt a fi uniform 
                if (v[ve[x]]>0) // daca e folosit in H 
                {
                    search_elem[k]=ve[x];
                    v[ve[x]] = 0;
                    k++;
                }
            }
        
            for(int p=0 ;p<m;p++)
            {
                n_searches=0 ;
                if (search_H(H,search_elem[p],N) != -1)
                {
                    avg_effort_found +=n_searches; //daca l am gasit 
                    if (n_searches > max_effort_found) 
                    {
                        max_effort_found=n_searches;
                    }
                }
                else
                    {
                        avg_effort+=n_searches; //daca l am gasit 
                        if (n_searches > max_effort) 
                            {
                                max_effort=n_searches;
                            }
                    }
            }
        }
        avg_effort/=7500; //1500*5
        avg_effort_found/=7500;
        fprintf(f, "%f , %d , %f , %d , %f \n",0.8 ,max_effort_found ,avg_effort_found, max_effort ,avg_effort );

        fclose(f);
}
int main()
{
    demo();
    //perf();
    //perf2();
    //delete_op();
    return 0;
}
