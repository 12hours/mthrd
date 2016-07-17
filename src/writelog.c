#include "includes.h"


int writelog(int count, ...){
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    strftime(s, sizeof(s), "%c", tm);

    if (-1 == write(LOG, s, strlen(s))){
    	perror("Can't write to log file");
    	return -1;
    }
    if (-1 == write(LOG, " ", 1)){
    	perror("Can't write to log file");
    	return -1;
    }

    va_list vlist;
    va_start(vlist, count);

    while (count){
    	char* msg = va_arg(vlist, char*);
    	if (-1 == write(LOG, msg, strlen(msg))){
    		perror("Can't write to log file");
    		return -1;
    	}
    	if (-1 == write(LOG, " ", 1)){
    		perror("Can't write to log file");
    		return -1;
    	}
    	count--;
    }
    if (-1 == write(LOG, "\n", 1)){
    	perror("Can't write to log file");
    	return -1;
    }
	return 1;
}
