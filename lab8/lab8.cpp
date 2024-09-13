#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"


#define stepSize 100
#define maxSize 10000

Profiler p("lab8");


typedef struct NodeTree
{
    int key;
    struct NodeTree *parent;
    struct NodeTree *left;
    struct NodeTree *right;
} NodeTree;

NodeTree* create_Node(int key)
{
    NodeTree *new_Node = (NodeTree *)malloc(sizeof(NodeTree));
    new_Node->key = key;
    new_Node->parent = NULL;
    new_Node->left = NULL;
    new_Node->right = NULL;
    return new_Node;
}

NodeTree* insert_Node(NodeTree* root, int key)
{
    if (root == NULL)
    {
        return create_Node(key);
    }
    else
    {
        if (key < root->key)
        {
            root->left = insert_Node(root->left, key);
            if (root->left != NULL)
            {
                root->left->parent = root;
            }
        }
        else
        {
            root->right = insert_Node(root->right, key);
            if (root->right != NULL)
            {
                root->right->parent = root;
            }
        }
    }
    return root;
}


NodeTree* Build_Tree(int a, int b)
{
    if (a > b)
    {
        return NULL;
    }
    int m = (a + b ) / 2;
    NodeTree* root = create_Node(m);
    // subtrees
    root->left = Build_Tree(a, m - 1);
    root->right = Build_Tree(m + 1, b);
    return root;
}

void inorder(NodeTree* root , int n ) 
{
    //p.countOperation("Recursive", n);
    if (root != NULL) {
        inorder(root->left, n);
        printf("%d ", root->key);
        inorder(root->right, n);
    }
}

void pprint ( NodeTree *root, int level = 0 )
{
    if (root == NULL)
    {
        return;
    }
    pprint(root->right, level + 1);
    printf("\n");
    for (int i = 0; i < level; i++)
    {
        printf("      ");
    }
    printf("%d", root->key);
    pprint(root->left, level + 1);
}

void iterativeInorder(NodeTree *root , int n )
{
    p.countOperation ("Iterativ" , n,3) ;
    int d = 1;
    NodeTree *node = root;
    NodeTree *prev = NULL;
    p.countOperation ("Iterativ" , n) ;

    while (node != NULL)
    {
        p.countOperation ("Iterativ" , n) ;
        if (d == 1)
        {
            p.countOperation ("Iterativ" , n) ;
            if (node->left != NULL)
            {
                p.countOperation ("Iterativ" , n) ;
                NodeTree *leftChild = node->left;
                p.countOperation ("Iterativ" , n, 2) ;
                while (leftChild->right != NULL && leftChild->right != node)
                {
                    p.countOperation ("Iterativ" , n) ;
                    leftChild = leftChild->right;
                    p.countOperation ("Iterativ" , n, 2) ;
                }
                p.countOperation ("Iterativ" , n) ;
                if (leftChild->right == NULL)
                {
                    p.countOperation ("Iterativ" , n) ;
                    leftChild->right = node;
                    node = node->left;
                    p.countOperation ("Iterativ" , n) ;
                }
                else
                {
                    leftChild->right = NULL;
                    printf("%d ", node->key);
                    node = node->right;
                    p.countOperation ("Iterativ" , n, 2) ;
                }
            }
            else
            {
                printf("%d ", node->key); 
                node = node->right;
                p.countOperation ("Iterativ" , n) ;
            }
        }
    }
}



void demo()
{
    int arr[] ={1,7,2,0,5,0,0,0,9,3,0,0,4,0,6,0,0};
    int size = sizeof (arr) / sizeof (arr[0]) ;
    NodeTree* root = NULL;
    int i = 0;
    root = Build_Tree(1,10);
    pprint(root) ;
    printf ("\n") ;
    inorder(root,i) ;
     printf ("\n") ;
    root = Build_Tree(1,10);
    iterativeInorder(root,size) ;
     printf ("\n") ;
}

void perf() 
{
    for (int i = stepSize; i <= maxSize; i += stepSize) {
        NodeTree *a = NULL;
        a = Build_Tree(1, i);
        inorder(a, i);
        iterativeInorder(a, i);
    }
    p.createGroup("Iterativ_VS_Recursive", "Iterativ", "Recursive");
    p.showReport();
}

int main()
{
    demo();
    //perf();
    return (0);
}
