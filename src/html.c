#include "includes.h"

int process(int fd){
	writelog(1, "Request responding");
	char buff[1024];
	int len = recv(fd, buff, 1024, MSG_NOSIGNAL);
	if ((len == 0) && (errno != EAGAIN)){
		shutdown(fd, SHUT_RDWR);
		close(fd);
		writelog(1, "Slave socket closed (recv = 0)");
		return 0;
	} else if (len == -1){
		perror("Recv error");
	}
	buff[len] = '\0';

	char b[4]; sprintf(b, "Responding to %d with len %d and req %s", fd, len, buff);
	writelog(1,b);

	char* method = (char*)malloc(sizeof(buff)+1);
	char* path;
	char* version;
	strcpy(method, buff);  //copy header data into method
	method = strtok(method, " ");
	printf("%s\n", method);
	path = strtok(NULL, " ");
	printf("%s\n", path);
	version = strtok(NULL, "\r\n");
	printf("%s\n", version);

	writelog(2, "Trying GET", path);
	char *ques_mark = index(path, '?');
	if (ques_mark != NULL){
	printf("%s\n", ques_mark);
		*ques_mark = '\0';
	}


	char path_buff[512];
	sprintf(path_buff, "%s%s", DIR, path);
	writelog(2, "Searching for", path_buff);
//	send(fd, buff, len, MSG_NOSIGNAL);

	FILE* req_file = fopen(path_buff, "r");
	if (req_file == NULL){
		writelog(1, "Requested file not found");
		char* nfound = "HTTP/1.0 404 NOT FOUND\r\nContent-length: 0\r\nContent-Type: text/html\r\n\r\n";
		send(fd, nfound, strlen(nfound), MSG_NOSIGNAL);
		return 0;
	} else {
		char file_buff[4096];
		int file_length = fread(file_buff, 1, 4096, req_file);

		char header_buff[128];
		sprintf(header_buff, "HTTP/1.0 200 OK\r\nContent-length: %d\r\nContent-Type: text/html\r\n\r\n", file_length);
		send(fd, header_buff, strlen(header_buff), MSG_NOSIGNAL);
		send(fd, file_buff, file_length, MSG_NOSIGNAL);
//		fgets(buff, sizeof(buff), req_file);

		fclose(req_file);
		writelog(2, "Send", path_buff);
	}



	return 0;
}

