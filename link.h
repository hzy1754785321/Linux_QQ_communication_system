#ifndef __LINK_H_
#define __LINK_H_
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct DATA 
{
	int id;
	char name[20];
};
typedef struct student
{
	struct DATA data;
	struct student * prior;
	struct student * next;
}STU,*Pstu;

#endif
