/* Implementacija sucelja ATP reda nizom. */

#include <stdlib.h>
#include "queue.h"

#define CAPACITY 10
struct node {
	int value;
	struct node *left;
	struct node *middle;
	struct node *right;
};

struct queue_tag {
	queue_element_t items[CAPACITY];
	int size;
	int first;
	int last;
};

/* O(1) */
queue_t queue_new() {
	queue_t queue = malloc(sizeof(struct queue_tag));
	queue->size = 0;
	queue->first = 0;
	queue->last = 0;
	return queue;
}

/* O(1) */
void queue_delete(queue_t queue) {
	free(queue);
}

/* O(1) */
void queue_enqueue(queue_t queue, queue_element_t elem) {
	queue->items[queue->last++] = elem;
	if (queue->last == CAPACITY)
		queue->last = 0;
	queue->size++;
}

/* O(1) */
queue_element_t queue_dequeue(queue_t queue) {
	queue_element_t elem = queue->items[queue->first++];
	if (queue->first == CAPACITY)
		queue->first = 0;
	queue->size--;
	return elem;
}

/* O(1) */
queue_element_t queue_front(queue_t queue) {
	return queue->items[queue->first];
}

/* O(1) */
int queue_is_empty(queue_t queue) {
	return queue->size == 0;
}

/* O(1) */
int queue_size(queue_t queue) {
	return queue->size;
}
