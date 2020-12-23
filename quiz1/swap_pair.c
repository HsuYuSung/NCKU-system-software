#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct __node {
    int value;
    struct __node *next;
} node_t;

void add_entry(node_t **head, int new_value)
{
    node_t **indirect = head;

    node_t *new_node = malloc(sizeof(node_t));
    new_node->value = new_value;
    new_node->next = NULL;

    assert(new_node);
    while (*indirect)
        indirect = &(*indirect)->next;
    *indirect = new_node;
}

node_t *find_entry(node_t *head, int value)
{
    node_t *current = head;
    for (; current && current->value != value; current = current->next)
        /* interate */;
    return current;
}

void remove_entry(node_t **head, node_t *entry)
{
    node_t **indirect = head;

    while ((*indirect) != entry)
        indirect = &(*indirect)->next;

    *indirect = entry->next;
    free(entry);
}

void swap_pair(node_t **head)
{
    for (node_t **node = head; *node && (*node)->next; node = &(*node)->next->next) {
        node_t *tmp = *node;
        *node = (*node)->next;
        tmp->next = (*node)->next;
        (*node)->next = tmp;
    }
}

// node_t *swap_pair(node_t *head)
// {
//     for (node_t **node = &head; *node && (*node)->next; node = &(*node)->next->next) {
//         node_t *tmp = *node;
//         *node = (*node)->next;
//         tmp->next = (*node)->next;
//         (*node)->next = tmp;
//     }
//     return head;
// }

// static void reverse(node_t** head_ref) 
// { 
//     node_t *prev = NULL; 
//     node_t *current = *head_ref; 
//     node_t *next = NULL; 
//     while (current != NULL) { 
//         // Store next 
//         next = current->next; 
  
//         // Reverse current node's pointer 
//         current->next = prev; 
  
//         // Move pointers one position ahead. 
//         prev = current; 
//         current = next; 
//     } 
//     *head_ref = prev; 
// } 


/* the no return version of reverse
void reverse(node_t **head)
{
    node_t *cursor = NULL;
    node_t *next;
    while (*head) {
        next = (*head)->next;
        (*head)->next = cursor;
        cursor = *head;
        *head = next;
    }
    *head = cursor;
}
*/

node_t *rev_recursive(node_t *head, node_t **pos)
{
    if(head->next) {
        node_t *tmp = rev_recursive(head->next, pos);
        tmp->next = head;
        head->next = NULL;
        return head;
        
    }
    *pos = head;
    return head;
}

void reverse(node_t **head)
{   
    if (!*head || !(*head)->next)
        return;
    node_t **pos;
    pos = head;
    
    rev_recursive(*head, pos);
    head = pos;
}




void print_list(node_t *head)
{
    for (node_t *current = head; current; current = current->next)
        printf("%d ", current->value);
    printf("\n");
}

//find the previous position of target
void find_pos(node_t **tmp, uint16_t pos)
{   
    for (pos--;pos;pos--) {
        *tmp = (*tmp)->next;
    }
}

void shuffle(node_t **head)
{   
    uint16_t count = 0;
    for (node_t *current = *head; current; current = current->next) {
        count += 1;
    }
    if (count == 1)
        return;


    node_t *tail = NULL;
    node_t *cursor = *head;
    node_t *tmp = *head;
    uint16_t pos = rand() % count;
    if (pos == 0) {
        tail = cursor;
        cursor = cursor->next;
        count -= 1;
    } else {
        find_pos(&tmp, pos);
        node_t *target = tmp->next;
        tmp->next = target->next;
        target->next = cursor;
        tail = target;
        count -= 1;
    }
    *head = tail;

    for (count--; count; count--) {
        tmp = cursor;
        uint16_t pos = rand() % count;

        if (pos == 0) {
            tail = cursor;
            cursor = cursor->next;
            continue;
        }
        find_pos(&tmp, pos);
        node_t *target = tmp->next;
        tmp->next = target->next;
        target->next = cursor;
        tail->next = target;
        tail = tail->next;
    }
}




int main(int argc, char const *argv[])
{
    node_t *head = NULL;

    print_list(head);

    add_entry(&head, 72);
    add_entry(&head, 101);
    add_entry(&head, 108);
    add_entry(&head, 109);
    add_entry(&head, 110);
    add_entry(&head, 111);

    print_list(head);

    node_t *entry = find_entry(head, 101);
    remove_entry(&head, entry);

    entry = find_entry(head, 111);
    remove_entry(&head, entry);

    print_list(head);

    /* swap pair.
     * See https://leetcode.com/problems/swap-nodes-in-pairs/
     */
    swap_pair(&head);
    print_list(head);
    reverse(&head);
    print_list(head);
    shuffle(&head);
    print_list(head);
    
    
    
    
    
    // node_t **tmp = &head;
    // find_pos(&head, 5);
    // printf("%d", head->value);


    return 0;
}