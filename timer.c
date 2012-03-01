#include <stdio.h>
#include <stdlib.h>

typedef void (*callback)(void);

/*
 * data structure for timer_queue
 */
typedef struct {
    int alarm_tick_count;
    callback function;
    struct timer_node *next;
} timer_node;

/* head points to head of timer_queue */
struct timer_node *head = NULL;

void alarm_set(void (*callback)(void), int alarm_tick_count)
{
    struct timer_node *new;
    struct timer_node *curr_node, *prev_node;

    new = malloc(sizeof(timer_node));
    if (!new)
    {
        return;
    }
    new->alarm_tick_count = alarm_tick_count;
    new->function = callback;
    new->next = NULL;

    /*
     * Sorted insert into timer_queue.
     * head->node 1->node 2->NULL where
     * node 1.alarm_tick_count <= node 2.alarm_tick_count
     */
    if (head == NULL)
    {
        head = new;
        return;
    }
    curr_node = prev_node = head;
    while (curr_node != NULL)
    {
        if (curr_node->alarm_tick_count >= alarm_tick_count)
        {
            break;
        }
        prev_node = curr_node;
        curr_node = curr_node->next;
    }
    if (curr_node != NULL)
    {
        new->next = curr_node;
    }
    prev_node->next = new;

    return;
}

void alarm_check(int system_tick_count)
{
    struct timer_node *curr_node = head; 
    struct timer_node *remove_node = NULL;
    callback function;

    while(curr_node != NULL)
    {
        /*
         * current system tick is shorter than any 
         * alarm ticks
         */
        if (curr_node->alarm_tick_count > system_tick_count)
            break;
        /*
         * alarm needs to be fired at current system tick
         */
        if (curr_node->alarm_tick_count == system_tick_count)
        {
            function = curr_node->function;
            /* call callback function */
            (function)();
            head = curr_node->next;
            free(curr_node);
            curr_node = head;
        }
        else
        {
            /* highly unlikely case */
            curr_node = curr_node->next;
        }
    }
}

int main()
{
            
    return 1;
}
