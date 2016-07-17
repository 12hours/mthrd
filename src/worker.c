#include "includes.h"

void *worker_thread(void* arg){
	struct queue* sockets = (struct queue*) arg;
	writelog(1, "SubThread initialized");
	int fd;
	while (1){
		sem_wait(&SEMQ);
//		pthread_mutex_lock(&MUT_SP_Q);
		pthread_spin_lock(&MUT_SP_Q);
		fd = queue_pop(sockets);
		pthread_spin_unlock(&MUT_SP_Q);

		if (fd == -1){
			writelog(1, "Queue pop returned -1");
			continue;
		} else {
			process(fd);
		}
	}
	return NULL;
}
