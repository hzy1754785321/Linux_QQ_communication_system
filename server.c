#include"./fun.h"
char * user="root";
char * passwd="1997";
char * database="qq_data";
char * server="localhost";


void hander_login(int confd)
{
	struct person temp;
	int ret=read(confd,&temp,sizeof(struct person));
	if(ret<=0)
	{
		printf("read login temp failed %d\n",__LINE__);
		return;
	}
	MYSQL *conn;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,passwd,database,0,NULL,0))
	{
		fprintf(stderr,"%s\n",mysql_error(conn));
		return ;
	}
	char sql[100];
	sprintf(sql,"select * from user where id=%d and passwd=\'%s\'",temp.id,temp.passwd);
	if(mysql_query(conn,sql))
	{
		fprintf(stderr,"%s\n",mysql_error(conn));
		return ;
	}
	MYSQL_RES *res;
	MYSQL_ROW row;

	res=mysql_use_result(conn);

	row=mysql_fetch_row(res);
	if(row==NULL)
	{
		printf("id or passwd error\n");
		temp.flag=-1;
	}
	else
	{
		strcpy(temp.name,row[1]);
		temp.flag=1;
		add_online(temp.id,confd);
		system("clear");
		show_online();
	}
	write(confd,&temp,sizeof(struct person));
	mysql_free_result(res);
	mysql_close(conn);
}
void hander_register(int confd)
{
	struct person temp;
	int ret=read(confd,&temp,sizeof(struct person));
	if(ret<=0)
	{
		printf("read register temp failed %d\n",__LINE__);
		return;
	}
	MYSQL *conn;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,passwd,database,0,NULL,0))
	{
		fprintf(stderr,"%s\n",mysql_error(conn));
		return ;
	}
	char sql[100];
	//sprintf(sql,"select * from person where id=%d and passwd=\'%s\'",temp.id,temp.passwd);
	sprintf(sql,"insert into user(name,passwd) values(\'%s\',\'%s\')",temp.name,temp.passwd);
	if(mysql_query(conn,sql))
	{
		fprintf(stderr,"%s\n",mysql_error(conn));
		return ;
	}
	
	strcpy(sql,"select max(id) as value from user");
	if(mysql_query(conn,sql))
	{
		fprintf(stderr,"%s\n",mysql_error(conn));
		return ;
	}
	


	MYSQL_RES *res;
	MYSQL_ROW row;

	res=mysql_use_result(conn);

	row=mysql_fetch_row(res);
	if(row==NULL)
	{
		printf("id or passwd error\n");
		temp.flag=-1;
	}
	else
	{
		temp.id=atoi(row[0]);
		temp.flag=1;
		add_online(temp.id,confd);
		system("clear");
		show_online();
	}
	write(confd,&temp,sizeof(struct person));
	mysql_free_result(res);
	mysql_close(conn);

}


void recv_message(int confd)
{
	struct msgbuf temp;
	int ret=read(confd,&temp,sizeof(struct msgbuf));
	if(ret<=0)
	{
		printf("recv mssage failed\n");
		delete_online(confd);
		system("clear");
		show_online();
		close(confd);
		pthread_exit(NULL);
	}
	printf("%d\t%s\n",temp.id,temp.buf);
	int sendconfd=search_confd(temp.id);
	if(sendconfd==-1)
	{
		printf("send message failed\n");
		return ;
	}
	temp.id=search_id(confd);
	write(sendconfd,&temp,sizeof(struct msgbuf));

}
void hander_send_all(int confd)//处理群发
{
	struct msgbuf temp;
	int ret=read(confd,&temp,sizeof(struct msgbuf));
	if(ret<=0)
	{
		printf("recv mssage failed\n");
		delete_online(confd);
		system("clear");
		show_online();
		close(confd);
		pthread_exit(NULL);
	}
	struct online *p=head;
	temp.id=search_id(confd);
	while(p!=NULL)
	{
	//	if(p->confd !=confd)
			write(p->confd,&temp,sizeof(struct msgbuf));
			printf("test1\n");
			write(p->confd,p,sizeof(struct online));
			printf("test2\n");
		p=p->next;
	}
	
}

void send_status(int confd)
{
		struct msgbuf temp;
		read(confd,&temp,sizeof(struct msgbuf));
		struct online *p=head;
		temp.flag=1;
		while(p!=NULL)
		{
				if(p->confd!=confd)
						write(p->confd,&temp,sizeof(struct msgbuf));
				p=p->next;
		}
}




void * fun(void * arg)
{
	int confd=*((int *)arg);
	struct pack temp;
	while(read(confd,&temp,sizeof(struct pack))==sizeof(struct pack))
	{
		switch(temp.type)
		{
			case 1: hander_login(confd);
					break;
			case 2:hander_register(confd);
				   break;
			case 3:recv_message(confd);
				   break;
			case 4:hander_send_all(confd);
				   break;
			default:
						printf("error type %d\n",__LINE__);
						delete_online(confd);
						system("clear");
						show_online();
						close(confd);
						return NULL;

		}

	}
	printf("client closed %d\n",__LINE__);
	delete_online(confd);
	system("clear");
	show_online();
	close(confd);
	return NULL;



}

int main(int argc,char ** argv)
{
	head=NULL;

	int listenfd= socket(AF_INET,SOCK_STREAM,0);
	/*
	   int bind(int sockfd, const struct sockaddr *addr,
	   socklen_t addrlen);
	   作用:将本程序与自身的端口和ip进行捆绑，便于对方链接

	   成功返回0 失败返回-1
	 */
	struct sockaddr_in myaddr;
	myaddr.sin_family=AF_INET;
	myaddr.sin_port=htons(9002);
	inet_pton(AF_INET,"192.168.1.52",&myaddr.sin_addr.s_addr);


	int ret=bind(listenfd,(struct sockaddr *)&myaddr,sizeof(myaddr));
	if(ret<0)
	{
		perror("bind failed\n");
		return -1;
	}

	/*
	   int listen(int sockfd, int backlog); 
	   作用:监听套接字-listen看是否有人请求三次握手
	   参数2:最大的等待队列的大小
	   返回值:成功返回0 失败 -1
	 */
	ret=listen(listenfd,20);

	if(ret<0)
	{
		perror("listen failed\n");
		return -1;
	}

	/*
	   int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	   作用:接收三次握手,也就是处理客户端的connect，建立与客户端之间的链接
	   参数1:用户监听的套接字
	   参数2：客户端的ip与端口,存放客户端的IPV4地址族,
	   参数3:地址族的长度的指针
	   返回值:链接成功,返回一个与客户端通信的套接字描述符,失败返回-1
	 */
	struct sockaddr_in caddr;
	int len =sizeof(caddr);
	char buf[20];

	int confd;
	while(1)
	{
		confd=accept(listenfd,(struct sockaddr *)&caddr,&len);

		//printf("confd=%d\n",confd);
		//printf("client port=%d\n",ntohs(caddr.sin_port));
		//const char *inet_ntop(int af, const void *src,char *dst, socklen_t size);
		//printf("client address=%s\n",inet_ntop(AF_INET,&caddr.sin_addr.s_addr,buf,20));
		pthread_t thread;
		pthread_create(&thread,NULL,fun,&confd);
	}
	close(listenfd);

}
