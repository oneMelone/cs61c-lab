#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
	node *slow, *fast;
   	slow = fast = head;
	while (fast != NULL && fast -> next != NULL) {
		fast = fast -> next -> next;
		slow = slow -> next;
		if (fast == slow) {
			return 1;
		}
	}
    return 0;
}
