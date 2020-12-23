#include <stdio.h>
#include <stdlib.h>


/* clang-format off */
#define cons(x,y) (struct ListNode[]){{y, x}}
/* clang-format on*/

struct ListNode {
    int val;
    struct ListNode *next;
};

typedef struct ListNode ListNode;

void printList(ListNode *head) {
    while(head) {
        printf("%d->", head->val);
        head = head->next;
    }
    printf("NULL\n");
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

ListNode* sortList(ListNode* head) {
    // merge sort
    if(!head || !head->next) return head;
    
    ListNode* fast = head->next;
    ListNode* slow = head;
    
    // split list
    while(fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;

    // sort each list
    ListNode* l1 = sortList(head);
    ListNode* l2 = sortList(fast);
    
    // merge sorted l1 and sorted l2
    return merge(l1, l2);
}

int main()
{
    ListNode *arr = cons(cons(cons(cons(NULL, 7), 4), 5), 9);
    ListNode *cur = arr;
    for (;cur;) {
        printf("%d ", cur->val);
        cur = cur->next;
    }
    printf("\n");


    ListNode *ans = sortList(arr);
    printList(ans);
    return 0;
}