#include "inc/linked_list.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void LinkedList_Add(LinkedList *head, void *item) {
    if (!head->item)
        head->item = item;
    else {
        LinkedList *tmp = head;
        while (tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = (LinkedList *)malloc(sizeof(LinkedList));
        memset(tmp->next, 0, sizeof(LinkedList));
        tmp->next->item = item;
    }
}

void LinkedList_Remove(LinkedList **head, void *item) {
	LinkedList *tmp = *head, *prev = NULL;
	while (tmp) {
		if (tmp->item == item) {
			if (prev == NULL) {
				if (tmp->next)
					memcpy(head, &tmp->next, sizeof(LinkedList));
				else
					memset(head, 0, sizeof(LinkedList));
			} else if (tmp->next)
				prev->next = tmp->next;
            else
                memset(&prev->next, 0, sizeof(LinkedList));
			break;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}
