#include "includes.h"



int main(int argc, char **argv) {
	 LOG = open("server.log", O_WRONLY | O_CREAT | O_APPEND, 0666);

	 if (-1 == pthread_spin_init(&MUT_SP_Q, PTHREAD_PROCESS_PRIVATE)){
		 perror("create MUT_SP_Q");
	 }

	 if (-1 == sem_init(&SEMQ, PTHREAD_PROCESS_PRIVATE, 0)){
		 perror("create SEMQ");
	 }

	char *port;
	char *address;
	char *directory;

	char opt;
	while (-1 != (opt = (char) getopt(argc, argv, "p:h:d:"))){
		switch (opt){
		case ('p'):
			port = optarg;
			break;
		case ('h'):
			address = optarg;
			break;
		case ('d'):
			directory = optarg;
			break;
		default:
			break;
		}
	}

	DIR = directory;
	int pid = fork();
	if (pid > 0){
		exit(0);
	}
	setsid();
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	
	writelog(7, "SERVER STARTED", "Address:", address, "Port:", port, "Directory:", directory);

	int master_socket = create_master_socket(address, port, directory);
	start_master_thread(master_socket);

	writelog(1, "SERVER STOPPED");


//	printf("Test queue");
//	printf("push 42, 12, 24, 0, 22\n");
//	struct queue q;
//	queue_init(&q);
//	queue_push(&q, 42);
//	queue_push(&q, 12);
//	queue_push(&q, 24);
//	queue_push(&q, 0);
//	queue_push(&q, 22);
//
//	printf("%d\n", queue_pop(&q));
//	printf("%d\n", queue_pop(&q));
//	printf("%d\n", queue_pop(&q));
//	printf("%d\n", queue_pop(&q));
//	printf("%d\n", queue_pop(&q));
//	printf("%d\n", queue_pop(&q));


	return 0;
}


