#include "includes.h"


int start_master_thread(int MasterSocket){
	struct queue slaves;
	queue_init(&slaves);

	pthread_t ids[MAX_THREADS];
	int t;
	for (t = 0; t< MAX_THREADS; t++){
		if (-1 ==pthread_create(&(ids[t]), NULL, worker_thread, (void*)&slaves)){
			perror("Thread creating");
		}
	}

	int EPoll = epoll_create1(0);
	struct epoll_event Event;
	Event.data.fd = MasterSocket;
	Event.events = EPOLLIN | EPOLLET;

	if (-1 == epoll_ctl(EPoll, EPOLL_CTL_ADD, MasterSocket, &Event)){
		perror("Adding MasterSocket to Epoll");
	}

	set_nonblock(MasterSocket);
	while (1){
		struct epoll_event Events[MAX_EVENTS];
		int N = epoll_wait(EPoll, Events, MAX_EVENTS, -1);

		int i;
		for (i = 0; i < N; i++){
			if (Events[i].data.fd == MasterSocket){
				int SlaveSocket = accept_socket(MasterSocket);
				set_nonblock(SlaveSocket);
				make_reusable(SlaveSocket);

				struct epoll_event Event;
				Event.data.fd = SlaveSocket;
				Event.events = EPOLLIN | EPOLLET;
				if (-1 == epoll_ctl(EPoll, EPOLL_CTL_ADD, SlaveSocket, &Event)){
					perror("Adding SlaveSocket to Epoll");
				}
			} else {

				// MUT_SP_Q lock
				pthread_spin_lock(&MUT_SP_Q);

				// echo_respond(Events[i].data.fd);
				queue_push(&slaves, Events[i].data.fd);
				//log
				char buff[128];
				sprintf(buff, "Pushing socket to stack; socket: %d", Events[i].data.fd);
				writelog(1, buff);

				// SEMQ
				sem_post(&SEMQ);

				// MUT_SP_Q unlock
				pthread_spin_unlock(&MUT_SP_Q);
			}
		}

	}


	return 1;
}
