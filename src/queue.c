#include "includes.h"

int queue_init(struct queue *q){
	q->items = malloc(sizeof(int));
	q->len = 0;
	q->size = 1;
	q->begin = 0;
	q->end = -1;
	q->max_begin = 10;
	return 1;
}

int queue_push(struct queue *q, int item){
	if (q->end == q->size){
		q->items = realloc(q->items, q->size * 2 * sizeof(int));
		q->size *=2;
	}
	q->end += 1;
	q->items[q->end] = item;
	q->len += 1;
	return 1;
}

int queue_pop(struct queue *q){
	if (q->len == 0){
		return -1;
	}
	q->len -= 1;
	int ret = q->items[q->begin];
	q->begin++;

	if (q->begin == q->max_begin){
		int i;
		for (i = q->begin; i <= q->end; i++){
			q->items[i-q->max_begin] = q->items[i];
		}
		q->begin -= q->max_begin;
		q->end -= q->max_begin;
	}

	return ret;
}

int queue_length(struct queue *q){
	return q->len;
}
