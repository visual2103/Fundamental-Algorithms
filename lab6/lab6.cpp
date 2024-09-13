#include <stdio.h>
#include <stdlib.h>
#define size 10

void pprint1(int a[], int n, int root, int level) 
{
    for (int i = 0; i < n; i++) {
        if (a[i] == root) {
            printf("%*s%d\n", level * 4, "", i);
            pprint1(a, n, i, level + 1);
        }
    }
}

void demo1() 
{
    int Arr[] = {6, 2, 7, 5, 2, 7, 7, -1, 5, 2};
    int n = sizeof(Arr) / sizeof(Arr[0]);
    printf("Parent array :\n ");
    pprint1(Arr, n, -1, 0);
}

typedef struct Node2 
{
    int key;
    int nrChildren;
    struct Node2 *Children[size];
} Node2;

void create_Multi_Way(Node2 *r2[size], int arr[]) 
{
    for (int i = 0; i < size; i++) {
        r2[i] = (Node2 *)malloc(sizeof(Node2));
        r2[i]->key = i;
        r2[i]->nrChildren = 0;
    }

    for (int i = 0; i < size; i++) {
        if (arr[i] != -1) {
            r2[arr[i]]->Children[r2[arr[i]]->nrChildren] = r2[i];
            r2[arr[i]]->nrChildren++;
        }
    }
}

void pprint2(Node2 *root, int level) 
{
    if (root == NULL) 
    {
        return;
    }

    printf("%*s%d\n", level * 4, "", root->key);

    for (int i = 0; i < root->nrChildren; i++) 
    {
        pprint2(root->Children[i], level + 1);
    }
}

void demo2() 
{
    int arr[] = {6, 2, 7, 5, 2, 7, 7, -1, 5, 2};

    printf("Multi way tree: \n");

    Node2 *root[size]; // r2
    create_Multi_Way(root, arr);

    pprint2(root[7], 0);
    printf("\n");
}

typedef struct Node3 
{
    int key;
    struct Node3 *firstChild;
    struct Node3 *nextSibling;
} Node3;

Node3 *Binary_Tree(Node2 *node) 
{
    if (node == NULL) 
    {
        return NULL;
    }

    Node3 *res = (Node3 *)malloc(sizeof(Node3));
    if (res == NULL) 
    {
        printf("Alocare de memorie esuata");
        exit(EXIT_FAILURE);
    }

    res->key = node->key;
    res->firstChild = NULL;
    res->nextSibling = NULL;

    if (node->nrChildren > 0) 
    {
        res->firstChild = Binary_Tree(node->Children[0]);

        Node3 *last = res->firstChild;
        for (int i = 1; i < node->nrChildren; i++) 
        {
            Node3 * v=Binary_Tree(node->Children[i]);
            last->nextSibling = v ;
            last = v;
        }
    }

    return res;
}


void pprint3(Node3 *root, int level) 
{
    if (root == NULL) {
        return;
    }

    printf("%*s%d\n", level * 4, "", root->key);
    if (root->firstChild != NULL) {
        pprint3(root->firstChild, level + 1);
    }

    if (root->nextSibling != NULL) {
        pprint3(root->nextSibling, level);
    }
}

void freeBinaryTree(Node3 *root) 
{
    if (root == NULL) 
    {
        return;
    }

    freeBinaryTree(root->firstChild);
    freeBinaryTree(root->nextSibling);
    free(root);
}

void demo3() 
{
     int arr[] = {6, 2, 7, 5, 2, 7, 7, -1, 5, 2};

    Node2 *root[size]; // r2
    create_Multi_Way(root, arr);
 
    Node3 *newRoot = Binary_Tree(root[7]);

    printf("Binary Tree:\n");
    pprint3(newRoot, 0);

    freeBinaryTree(newRoot);
}
int main() 
{
    demo1();
    demo2();
    demo3();
    return 0;
}
