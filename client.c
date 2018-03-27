#include"./fun.h"
struct conline *chead=NULL;

void *mypack(void *data,int len,int type)
{
	struct pack * temp=(struct pack *)malloc(sizeof(struct pack)+len);
	temp->ver=1;
	temp->type=type;
	temp->len=len;
	memcpy(temp->data,data,len);
	return temp;

}



void person_login(int sockfd)
{
	struct person temp;
	printf("please input id:");
	scanf("%d",&temp.id);
	printf("please input passwd:");
	scanf("%s",temp.passwd);
	struct pack *p=mypack(&temp,sizeof(temp),1);
	int ret=write(sockfd,p,sizeof(struct pack)+sizeof(struct person));
	if(ret<=0)
	{
		printf("write failed %d\n",__LINE__);
		close(sockfd);
		exit(0);
	}
}
void recv_login(int sockfd)
{
	struct person temp;
	int ret=read(sockfd,&temp,sizeof(struct person));
	if(ret<=0)
	{
		close(sockfd);
		printf("person login read failed %d\n",__LINE__);
		exit(0);
	}
	if(temp.flag==1)
		printf("welcome %s login\n",temp.name);
	else
	{
		printf("your id or passwd is error\n");
		exit(0);
	}
}
void person_register(int sockfd)
{
	struct person temp;
	printf("please input name:");
	scanf("%s",temp.name);
	printf("please input passwd:");
	scanf("%s",temp.passwd);
	struct pack *p=mypack(&temp,sizeof(temp),2);
	int ret=write(sockfd,p,sizeof(struct pack)+sizeof(struct person));

	if(ret<=0)
	{
		printf("write failed %d\n",__LINE__);
		close(sockfd);
		exit(0);
	}
}
void recv_register(int sockfd)
{
	struct person temp;
	int ret=read(sockfd,&temp,sizeof(struct person));
	if(ret<=0)
	{
		close(sockfd);
		printf("person register read failed %d\n",__LINE__);
		exit(0);
	}
	if(temp.flag==1)
		printf("success register your id is:%d\n",temp.id);
	else
	{
		printf("register failed\n");
		exit(0);
	}
}

void menu(int sockfd)
{
	printf("1 login\n");
	printf("2 register\n");
	printf("0 exit\n");
	printf("please input you chioce:");
	int key;
	scanf("%d",&key);
	switch(key)
	{
		case 1:
			person_login(sockfd);
			break;
		case 2:
			person_register(sockfd);
			break;
		case 0:
			close(sockfd);
			exit(0);
			break;
		default:
			printf("error input\n");
			menu(sockfd);
	}
}

int  send_one(int sockfd)
{

	struct msgbuf temp;
	printf("please input the message(id:buf)");
	scanf("%d:%s",&temp.id,temp.buf);

	struct pack * p=mypack(&temp,sizeof(struct msgbuf),3);	
	int ret=write(sockfd,p,sizeof(struct pack)+sizeof(struct msgbuf));
	if(ret<=0)
	{
		printf("write mssage failed\n");
		close(sockfd);
		return -1;;
	}
	return 0;

}
int  send_more(int sockfd)
{
	int num;
	printf("请输入你要发送的人数:");
	scanf("%d",&num);
	printf("请输入你要发送的信息:");
	struct msgbuf temp;
	scanf("%s",temp.buf);
	int ret;
	for(int i=0;i<num;i++)
	{
		printf("请输入你要发送的第%d个人的id:",i+1);
		scanf("%d",&temp.id);
		struct pack * p=mypack(&temp,sizeof(struct msgbuf),3);	
		ret=write(sockfd,p,sizeof(struct pack)+sizeof(struct msgbuf));
		if(ret<=0)
		{
			printf("write mssage failed\n");
			close(sockfd);
			return -1;;
		}
	}
	return 0;

}


int  send_all(int sockfd)
{
	printf("请输入你要发送的信息:");
	struct msgbuf temp;
	scanf("%s",temp.buf);
	temp.id=-1;
	int ret;
	struct pack * p=mypack(&temp,sizeof(struct msgbuf),4);	
	ret=write(sockfd,p,sizeof(struct pack)+sizeof(struct msgbuf));
	if(ret<=0)
	{
		printf("write mssage failed\n");
		close(sockfd);
		return -1;
	}
	return 0;

}


int  send_message(int sockfd)
{
	printf("1 单发\n");
	printf("2 多发\n");
	printf("3 群发\n");
	int key;
	scanf("%d",&key);
	switch(key)
	{
		case 1:if(send_one(sockfd))
				   return -1;
			   break;
		case 2:if(send_more(sockfd))
				   return -1;
			   break;
		case 3:if(send_all(sockfd))
				   return -1;
			   break;
		default:
			   printf("error input\n");
			   break;
	}
	return 0;
}

void recv_message(int sockfd)
{
	struct msgbuf temp;
	int ret=read(sockfd,&temp,sizeof(struct msgbuf));
	if(ret<=0)
	{
		printf("recv message failed\n");
		close(sockfd);
		return ;
	}
	else
		printf("recv %s from %d\n",temp.buf,temp.id);

}
void recv_online_link(int sockfd)
{
	struct DATA data;
	int ret=read(sockfd,&data,sizeof(struct DATA));
	if(ret<=0)
	{
		printf("recv link failed\n");
		close(sockfd);
		return ;
	}
	struct conline *temp=(struct conline *)malloc(sizeof(struct conline));
	temp->data=data;
	temp->next=chead;
	chead=temp;
}

void recv_delete(int sockfd)
{
	struct DATA data;
	int ret=read(sockfd,&data,sizeof(struct DATA));
	if(ret<=0)
	{
		printf("recv link failed\n");
		close(sockfd);
		return ;
	}
	if(chead==NULL)
		return ;
	struct conline *p,*q;
	if(chead->data.id==data.id)
	{
		p=chead->next;
		free(chead);
		chead=p;
		return ;
	}
	p=chead;
	q=p->next;
	while(q!=NULL)
	{
		if(q->data.id==data.id)
		{
			p->next=q->next;
			free(q);
			return ;
		}
		p=p->next;
		q=q->next;
	}
	return ;
	
}
void show_online_link()
{
	printf("id\tname\n");
	struct conline *p=chead;
	while(p!=NULL)
	{
		printf("%d\t%s\n",p->data.id,p->data.name);
		p=p->next;
	}
}


int main(int argc,char ** argv)
{
	/*
	   int socket(int domain, int type, int protocol);
	   作用:创建套接字
	   参数1:指出协议族--IPV4协议族(AF_INET)
	   参数2:指定协议类型,SOCK_STREAM指定字节流协议(如TCP)，SOCK_DGRAM指定数据报协议(如UDP),
	   参数3:指定具体协议,通常传递0
	   返回值:成功返回非负的套接字(类似与文件描述符), 失败返回-1

	 */
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	printf("sockfd=%d pid=%d\n",sockfd,getpid());

	/*
	   int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
	   作用:请求三次握手，建立端对端链接
	   参数1:套接字描述符
	   参数2:通用地址族结构--你要链接谁，对方的ip,端口等信息
	   参数3:地址族长度
	   返回值:成功 0 ，失败 -1
	 */

	//定义一个IPV4地址族结构
	struct sockaddr_in saddr;
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(atoi(argv[1]));//将16位的整数由主机字节序转化为网络字节序
	//将字符串IP转换成4字节整数存放在saddr.sin_addr.s_addr中
	int ret=inet_pton(AF_INET,"192.168.1.82",&saddr.sin_addr.s_addr);

	ret=connect(sockfd,(struct sockaddr *)&saddr,sizeof(saddr));

	if(ret==-1)
	{
		perror("connect failed\n");
		return -1;
	}
	menu(sockfd);

	struct msgbuf temp;
	pid_t pid;
	pid=fork();
	if(pid==0)
	{
		struct pack p;
		while(1)
		{
			ret=read(sockfd,&p,sizeof(struct pack));
			if(ret<=0)
			{
				printf("recv message failed\n");
				close(sockfd);
				return 0;
			}
			switch(p.type)
			{
				case 1:recv_message(sockfd);
					   break;
				case 2:recv_online_link(sockfd);
					   show_online_link();
					   break;
				case 3:recv_login(sockfd);
					   break;
				case 4:recv_register(sockfd);
					   break;
				case 5:recv_delete(sockfd);
					   show_online_link();
					   break;

			}

		}
	}
	else if(pid>0)
	{
		while(1)
		{
			if(send_message(sockfd))
				break;
		}
		int status;
		wait(&status);
	}
}











