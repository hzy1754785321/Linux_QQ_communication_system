
#include"./fun.h"
void * spack(void *p,int len,int type)
{
	struct pack * temp=(struct pack *)malloc(sizeof(struct pack)+len);
	temp->ver=2;
	temp->len=len;
	temp->type=type;
	memcpy(temp->data,p,len);
	return temp;
}

void send_link(int confd,int flag)
{
	struct DATA data;
	struct online *p=head;
	while(p!=NULL)
	{
		if(p->confd==confd)
		{
			data.id=p->id;
			strcpy(data.name,p->name);
			break;
		}
		p=p->next;
	}
	p=head;
	struct pack * temp=spack(&data,sizeof(data),2);
	while(p!=NULL)//将新增加的节点发送给所有已经在线的人
	{
		if(p->confd!=confd)
			write(p->confd,temp,sizeof(struct DATA)+sizeof(struct pack));
		p=p->next;
	}
	free(temp);
	p=head;
	while(p!=NULL)//将所有在线的人发给新增加的人
	{
		if(p->confd!=confd)
		{
			data.id=p->id;
			strcpy(data.name,p->name);
			temp=spack(&data,sizeof(data),2);
			write(confd,temp,sizeof(struct DATA)+sizeof(struct pack));
			free(temp);
		}
		p=p->next;
	}
	return ;
}

void send_delete(int confd)
{
	printf("on send delete\n");
	struct online *p=head;
	struct DATA data;
	while(p!=NULL)
	{
		if(p->confd==confd)
		{
			data.id=p->id;
			strcpy(data.name,p->name);
			break;
		}
		p=p->next;
	}
	struct pack * q=spack(&data,sizeof(struct DATA),5);
	p=head;
	while(p!=NULL)
	{
		if(p->confd!=confd)
			write(p->confd,q,sizeof(struct pack)+sizeof(struct DATA));
		p=p->next;
	}
	free(q);
	return ;
}


void  add_online(struct person p,int confd)
{
	struct online *temp=(struct online *)malloc(sizeof(struct online));
	temp->id=p.id;
	strcpy(temp->name,p.name);
	temp->confd=confd;
	temp->next=head;
	head=temp;
}

void show_online()
{
	struct online *p=head;
	printf("id\tconfd\tname\n");
	while(p!=NULL)
	{
		printf("%d\t%d\t%s\n",p->id,p->confd,p->name);
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







