
#include"./fun.h"

void  add_online(int id,int confd)
{
	struct online *temp=(struct online *)malloc(sizeof(struct online));
	temp->id=id;
	temp->confd=confd;
	temp->next=head;
	head=temp;
}

void show_online()
{
	struct online *p=head;
	printf("id\tconfd\n");
	while(p!=NULL)
	{
		printf("%d\t%d\n",p->id,p->confd);
		p=p->next;
	}
}

int search_confd(int id)
{
	struct online *p=head;
	while(p!=NULL)
	{
		if(p->id==id)
			return p->confd;
		p=p->next;
	}
	return -1;
}
int search_id(int confd)
{
	struct online *p=head;
	while(p!=NULL)
	{
		if(p->confd==confd)
			return p->id;
		p=p->next;
	}
	return -1;

}

void delete_online(int confd)
{
	struct online *p,*q;
	if(head==NULL)
		return ;
	if(head->confd==confd)
	{
		p=head->next;
		free(head);
		head=p;
		return ;
	}
	
	p=head,q=head->next;
	while(q!=NULL)
	{
		if(q->confd==confd)
		{
			p->next=q->next;
			free(q);
			return ;
		}
		p=p->next;
		q=q->next;
	}
}







