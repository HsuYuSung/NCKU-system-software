#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

typedef struct ListNode ListNode;

// insert the item
void insert(ListNode **head, int value) {
    ListNode *tmp = malloc(sizeof(ListNode));
    if (!tmp)
        return;

    tmp->val = value;
    tmp->next = *head;
    *head = tmp;
    return;
}

// count the list length
int countList(ListNode *head) {
	int count = 0;
	while(head) {
		count++;
		head = head->next;
	}
	return count;
}

// print the whole list
void printList(ListNode *head) {
    while(head) {
        printf("%d->", head->val);
        head = head->next;
    }
    printf("NULL\n");
}

// Insertion Sort
void sorted_insert(struct ListNode **head, struct ListNode *node)
{
    if (!*head || (*head)->val >= node->val)
    {
        node->next = *head;
        *head = node;
        return;
    }
    struct ListNode *current = *head;
    while (current->next && current->next->val < node->val)
        current = current->next;
    node->next = current->next;
    current->next = node;
}

void Insertsort(struct ListNode **head)
{
    struct ListNode *sorted = NULL;
    for (struct ListNode *current = *head; current;)
    {
        struct ListNode *next = current->next;
        sorted_insert(&sorted, current);
        current = next;
    }
    *head = sorted;
}

ListNode* merge(ListNode* l1, ListNode* l2) {
    // merge with recursive
    if(!l2) return l1;
    if(!l1) return l2;
    
    if(l1->val<l2->val) {
        l1->next = merge(l1->next, l2);
        return l1;
    } else {
        l2->next = merge(l1, l2->next);
        return l2;
    }
}


#define THR 64

ListNode* sortList(ListNode* head, int count) {
    // merge sort
    if(!head || !head->next) return head;
    
    ListNode* fast = head->next;
    ListNode* slow = head;

    int half_list = count / 2;
    if (half_list < THR)
    {
        Insertsort(&head);
        return head;
    }

    // split list
    while(fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;
	
    // sort each list
    ListNode *l1 = sortList(head, half_list);
    ListNode *l2 = sortList(fast, half_list);

    // merge sorted l1 and sorted l2
    return merge(l1, l2);
}

void free_list(ListNode **head)
{
	while (*head) {
		ListNode *next = (*head)->next;
		free(*head);
		*head = next;
	}
}


int main()
{   
    srand(1);

    ListNode *arr = NULL;

    for (int i = 0; i < 10000; i++) {
       int r = rand();
       insert(&arr, r);
    }

    int list_num  = countList(arr);
    ListNode *ans = sortList(arr, list_num);
    printList(ans);
	free_list(&arr);
//	free_list(&ans);	
    
	return 0;
}
