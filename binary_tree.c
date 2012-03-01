#include <stdio.h>


typedef struct {
    int data;
    struct node *left;
    struct node *right;
} node;


void tree_insert(node **tree, node *item)
{
    node *curr = *tree;

    if (*tree == NULL)
    {
        *tree = item;
        return;
    }

    if (item->data > curr->data)
    {
        tree_insert((node **)&(curr->right), (node *)item);
    }
    else
    {
        tree_insert((node **)&(curr->left), (node *)item);
    }
}

void tree_parse(node *leaf)
{
    if (leaf == NULL)
    {
        return;
    }
    
    tree_parse((node *)leaf->left);
    printf("Node is %d\n", leaf->data);
    tree_parse((node *)leaf->right);
}

int tree_count(node *leaf)
{
    int count = 0; 

    if (leaf == NULL)
    {
        return 0;
    }

    count = 1 + tree_count((node *)leaf->left) + tree_count((node *)leaf->right);
    return(count);
}

int max_depth(node *leaf)
{
    int ldepth; int rdepth;

    if (leaf == NULL)
    {
        return 0;
    }
    ldepth = max_depth((node *)leaf->left);
    rdepth = max_depth((node *)leaf->right);

    if (ldepth > rdepth)
        return ldepth + 1;
    else
        return rdepth + 1;
}

int main()
{
    int i, count =0;
    node *head;
    node *item;

    head = NULL;

    head = (node *)malloc(sizeof(node));
    head->data = 1;
    head->left = NULL;
    head->right = NULL;

#if 1
    for (i = 2; i < 9; i = i+2)
    {
        printf("adding item %d\n", i);
        item = (node *)malloc(sizeof(node));
        item->data = i;
        item->left = NULL;
        item->right = NULL;
        tree_insert(&head, item);
    }
    for (i = 9; i > 2; i = i-2)
    {
        printf("adding item %d\n", i);
        item = (node *)malloc(sizeof(node));
        item->data = i;
        item->left = NULL;
        item->right = NULL;
        tree_insert(&head, item);
    }

#endif
    count = tree_count(head);
    printf("Num of nodes in tree is %d\n", count);
    printf("max depth = %d\n", max_depth(head));

    tree_parse(head);

    return 1;
}
