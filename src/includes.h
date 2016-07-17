
#ifndef INCLUDES_H_
#define INCLUDES_H_

#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdio.h>
#include <memory.h>
#include <pthread.h>
#include <semaphore.h>
#include <strings.h>

#define MAX_EVENTS 128
#define MAX_THREADS 1
int LOG;
char *DIR;

int writelog(int, ...);
int create_master_socket(char*, char*, char*);
int set_nonblock(int);
int close_socket(int);
int respond(int);
int make_reusable(int);
int accept_socket(int);
int echo_respond(int);

void *worker_thread(void*);
int process(int);

struct queue {
	int *items;
	int len;    // num of items in array
	int size;	// size of memory allocated
	int begin;
	int end;
	int max_begin;
};
int queue_init(struct queue*);
int queue_push(struct queue*, int);
int queue_pop(struct queue*);
int queue_length(struct queue*);

pthread_mutex_t deq;
pthread_spinlock_t MUT_SP_Q;
sem_t SEMQ;


#endif

