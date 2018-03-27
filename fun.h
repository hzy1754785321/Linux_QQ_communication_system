#ifndef __FUN_H_
#define __FUN_H_
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<pthread.h>
#include<mysql/mysql.h>
#include<sys/wait.h>
struct pack
{
	int ver;
	int type;
	int len;
	char  data[0];
};

struct person 
{
	int flag;
	int id;
	char name[20];
	char passwd[20];
};

struct msgbuf
{
	int id;
	char buf[20];
};
struct online
{
	int id;
	char name[20];
	int confd;
	struct online *next;
};

struct DATA
{
	int id;
	char name[20];
};

struct conline
{
	struct DATA data;
	struct conline *next;
};

struct online * head;
void delete_online(int confd);
int search_id(int confd);
int search_confd(int id);
void show_online();
void  add_online(struct person p,int confd);
void *spack(void *,int,int);
void send_link(int confd,int flag);
void send_delete(int confd);
#endif







