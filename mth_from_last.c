#include <stdio.h>

typedef struct {
    int data;
    struct node *next;
} node;

struct node *head = NULL;

void add_node(struct node *new)
{
    if (head == NULL)
    {
        head = new;
        return;
    }
    new->next = (struct node *)head;
    head = new;
    return;
}
            
int main()
{
    struct node *new;
    int node_count;

    node_count = 1;
    while(node_count < 6)
    {
        new = (node *)malloc(sizeof(node));
        new->data = node_count;
        new->next = NULL;
        add_node(new);
    }
    return 1;
}
