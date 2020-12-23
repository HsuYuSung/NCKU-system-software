#include <stdio.h>
#include <stdlib.h>



struct ListNode {

	int val;
	struct ListNode* next;
};

void printlist(struct ListNode *head)
{
	struct ListNode *ptr = head;
	while(ptr) {
		printf("%d -> ", ptr->val);
		ptr = ptr->next;
	}
	printf("NULL\n");
}

void push(struct Node **head, int val)
{
	struct ListNode *newNode = (struct Node*) malloc(sizeof(struct ListNode));
	newNode->val = val;
	newNode->next = *head;
	*head = newNode;
}

struct ListNode* SortedMerge(struct ListNode *a, struct ListNode *b)
{
	if (!a)
		return b;
	if (!b)
		return a;
	
	struct ListNode *result = NULL;
	
	if (a->val <= b->val) {
		result = a;
		result->next = SortedMerge(a->next, b);
	} else {
		result = b;
		result->next = SortedMerge(a, b->next);
	}
	return result;
}

void FrontBackSplit(struct ListNode *sourct, struct ListNode **frontRef, struct ListNode **backRef)
{
	
}

