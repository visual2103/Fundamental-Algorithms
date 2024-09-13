#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Profiler.h"
# define max_size 10000
#define step_size 100
#define M 5

Profiler P("lab7") ;

typedef struct nodeTree
{
    int key ;
    struct nodeTree *left ;
    struct nodeTree *right ;
    int size ; // dim subarborelui la a cărui radă sunt
}nodeTree ;

nodeTree* createNode(int key )
{
    nodeTree* Node = (nodeTree*)malloc(sizeof(nodeTree));
    Node->key = key;
    Node->left = NULL;
    Node->right = NULL;
    Node->size = 1;  //leaf
    return Node;
}

// lg n pentru mult. dinam
nodeTree* OS_Select(nodeTree* x, int i, int size)
{
    P.countOperation("SELECT",size);
    if (x == NULL)
    {
        return NULL;
    }

    int r = 1; // init. r cu 1 pentru nodul curent
    P.countOperation("SELECT",size,2);
    if (x->left != NULL)
    {
        r += x->left->size; P.countOperation("SELECT",size);
    }
    P.countOperation("SELECT",size);
    if (i == r)
    {
        return x;
    }
    else { P.countOperation("SELECT",size);
        if (i < r) {
            return OS_Select(x->left, i, size);
        } else { P.countOperation("SELECT",size);
            if (i > r) {
                return OS_Select(x->right, i - r, size);
            } else {
                return NULL;
            }
        }
    }
}

// ABC construction
nodeTree* Build_Tree(int a, int b)
{
    if (a > b)
    {
        return NULL;
    }
    int m = (a + b ) / 2;
    nodeTree* root = createNode(m);

    // subtrees left și right
    root->left = Build_Tree(a, m - 1);
    root->right = Build_Tree(m + 1, b);

    // update size
    if (root->left != NULL)
    {
        root->size += root->left->size ;
    }
    if (root->right != NULL)
    {
        root->size += root->right->size ;
    }
    return root;
}




void pprint(nodeTree *root, int level = 0)
{
    if (root == NULL)
    {
        return;
    }
    pprint(root->right, level + 1);
    for (int i = 0; i < level; i++)
    {
        printf("      ");
    }
    printf("%d(%d)\n", root->key,root->size);
    pprint(root->left, level + 1);
}

nodeTree* FIND_MIN(nodeTree* node)
{
    nodeTree* x = node;
    while(x->left != NULL)
    {
        x = x->left;
    }
    return x;
}

void OS_DELETE(nodeTree** node,int k, int size) 
{
    nodeTree *q = NULL; 
    P.countOperation("DELETE",size,2);
    if ((*node) == NULL) 
    {
        return;
    }
    P.countOperation("DELETE",size);
    if ((*node)->key > k) 
    {
        (*node)->size--; 
        P.countOperation("DELETE",size);
        OS_DELETE(&(*node)->left, k, size);
    } 
    else 
        { 
            P.countOperation("DELETE",size);
            if ((*node)->key < k) 
            {
                (*node)->size--; P.countOperation("DELETE",size);
                OS_DELETE(&(*node)->right, k, size);
            } 
            else 
                { 
                    P.countOperation("DELETE",size);
                    if ((*node)->left == NULL) 
                    {
                        q = (*node)->right; P.countOperation("DELETE",size,2);
                        free(*node);
                        (*node) = q;
                    } 
                    else 
                        { 
                            P.countOperation("DELETE",size);
                            if ((*node)->right == NULL) 
                            {
                                q = (*node)->left; P.countOperation("DELETE",size,2);
                                free(*node);
                                (*node) = q;
                            } 
                            else 
                            {
                                q = FIND_MIN((*node)->right); P.countOperation("DELETE",size,3);
                                (*node)->key = q->key;
                                (*node)->size--;
                                OS_DELETE(&(*node)->right, q->key, size);
                            }
                        }
                }
        }   
}

void demo ()
{
    int n = 11;

    nodeTree* root = Build_Tree(1, n);
    printf("My BST : \n");
    pprint(root);

   for (int i = 0; i < n; i++)
    {
        int random_index = rand() % root->size + 1;
        nodeTree* node = OS_Select(root, random_index,n);
        if (node != NULL)
        {
            printf("\nThe element with the %d-th smallest key is %d.", random_index, node->key);
        }
        else
            {
                printf("\nThere is no element with the %d-th smallest key.\n", random_index);
            }
    }


    srand(time(NULL)); //
    for (int i = 0; i < n; i++)
    {
        int random_index = rand() % root->size + 1;
        nodeTree *node_delete = OS_Select(root, random_index, n);

        if (node_delete != NULL)
        {
            printf("\nBST after deleting %d: ", node_delete->key);
            OS_DELETE(&root, node_delete->key, n);
            printf("\n");
            pprint(root);
            printf("\n");
        }
    }
}

void Evaluation ()
{
    int n ;
    for (n=step_size ; n< max_size ; n+=step_size)
    {
        for(int i=0 ; i<M ;i++)
        {
            nodeTree* root = Build_Tree (1, n);
            for(int k = 0; k < n; k++) 
            {
                int q = 1 + rand() % root->size;
                nodeTree* Node_Select = OS_Select(root,q,n);
                OS_DELETE(&root,Node_Select->key,n);
            }
        }
    }

    P.divideValues("DELETE",M);
    P.divideValues("SELECT",M);

    P.createGroup("TOTAL","DELETE","SELECT");
    
    P.showReport();
}



int main()
{
    demo();
    Evaluation();
    return 0;
}
