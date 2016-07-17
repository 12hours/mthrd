#include "includes.h"

int create_master_socket(char* address, char* port, char* directory){

	int MasterSocket;
	if (-1 == (MasterSocket = socket(AF_INET, SOCK_STREAM, 0))){
		writelog(1, "Failed to create mastersocket");
		perror("Mastersocket");
	} else {
		writelog(1, "Mastersocket creation success");
	}

//	set_nonblock(MasterSocket);
	make_reusable(MasterSocket);

	struct sockaddr_in Saddr;
	memset(&Saddr, 0, sizeof(Saddr));
//	Saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	Saddr.sin_addr.s_addr = inet_addr(address);
	Saddr.sin_family = AF_INET;
	Saddr.sin_port = htons(atoi(port));

	if (-1 == bind(MasterSocket, (struct sockaddr*)&Saddr, sizeof(Saddr))){
		writelog(1, "Failed to bind mastersocket");
		perror("Bind master");
	} else {
		writelog(1, "Mastersocket bind success");
	}

	if (-1 == listen(MasterSocket, SOMAXCONN)){
		writelog(1, "Failed to listen mastersocket");
		perror("Listen to master");
	} else {
		writelog(1, "Mastersocket listen success");
	}

	return MasterSocket;
}

int make_reusable(int sock){
	int fl = 1;
	if (-1 == setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &fl, sizeof(int))){
		perror("Making reusable");
		return -1;
	}
	return 1;
}

int echo_respond(int slave_socket){
	char buff[128];
	writelog(1, "Start responding to socket");
	int len = recv(slave_socket, buff, 128, MSG_NOSIGNAL);

	if ( (len == 0) && (errno != EAGAIN)) {
		shutdown(slave_socket, SHUT_RDWR);
		close(slave_socket);
		writelog(1, "Stop responding");
		return 1;
	} else if ( len > 0){
		send(slave_socket, buff, len, MSG_NOSIGNAL);
	}
	return 1;
}

int set_nonblock(int fd){
	int flags;
	if ( -1 == ( flags = fcntl(fd, F_GETFL, 0))){
		flags = 0;
	}
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}


int close_socket(int sock){
	char sock_str[16];
	sprintf(sock_str, "%d", sock);

	if (-1 == shutdown(sock, SHUT_RDWR)){
		writelog(2, "Failed to shutdown socket", sock_str);
		perror("Socket shutdown");
		return -1;
	} else {
		writelog(3, "Socket", sock_str, "shutdown success");
	}

	if (-1 == close(sock)){
		writelog(2, "Failed to close socket", sock_str);
		perror("Close socket");
		return -1;
	} else {
		writelog(3, "Socket", sock_str, "close success");
	}

	return 0;
}

int accept_socket(int sock){
	int AcceptedSocket;
	if (-1 == (AcceptedSocket = accept(sock, 0, 0))){
		writelog(1, "Failed to accept socket");
		perror("Accepting socket");
		return -1;
	} else {
		char sock_str[64];
		sprintf(sock_str, "Socket %d performed accept; new socket: %d", sock, AcceptedSocket);
		writelog(1, sock_str);
		return AcceptedSocket;
	}
}

