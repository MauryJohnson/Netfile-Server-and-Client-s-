#ifndef LIBNETFILES_H
#define LIBNETFILES_H
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <netdb.h>
#include <math.h>
//
#define unrestricted  0
#define exclusive     1
#define transaction   2
//
#define INVALID_FILE_MODE 500
//
char* MODE = "-1";
//
int SOCKFD = -1;
//
int HOSTINIT=-1;
//
int PORT=-1;
//
struct sockaddr_in SERVADDR;
//
struct hostent* SERVER;
//
char* HOSTNAME;
//
ssize_t netopen(const char* fileName, size_t intPerm);
//
ssize_t netclose(int fd);
//
ssize_t netwrite(int fd,  const void* buffer,  size_t num);
//
ssize_t netread(int fd, void* buff, size_t num);
//
int netserverinit(char* hostname, int filemode);
//
#endif
