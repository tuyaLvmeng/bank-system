#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "struct.h"
#include "tools.h"

long user = 0;//帐号

int transf_a(Account acc)
{
    char path[L_PATH] = "./data/";
	char ex[L_PATH] = ".dat";
	sprintf(path,"%s%ld%s",path,acc.user,ex);

	Account buf = {};
	int fd = open(path,O_RDWR);
	if(0 != read(fd,&buf,sizeof(Account)))
	{
		printf("余额:%.2lf,",buf.money);
		if(buf.money < acc.money)
		{
			printf("余额不足\n");
			return -1;
		}
		else
		{
			user = acc.user;
			return 0;
		}
	}
	else
	{
		printf("读取数据丢失\n");
		return -1;
	}
}


int transf_b(Account acc)
{
    char path1[L_PATH] = "./data/";
	char ex1[L_PATH] = ".dat";
	sprintf(path1,"%s%ld%s",path1,acc.user,ex1);

	if(0 == access(path1,F_OK))
	{

		char path[L_PATH] = "./data/";
		char ex[L_PATH] = ".dat";
		sprintf(path,"%s%ld%s",path,user,ex);

		Account buf = {};
		int fd = open(path,O_RDONLY);
		if(0 != read(fd,&buf,sizeof(Account)))
		{
			//printf("余额:%.2lf\n",buf.money);
		}
		else
		{
			printf("读取数据丢失\n");
		}
	
		Account buf1 = {};
		int fd1 = open(path1,O_RDWR);
		if(0 != read(fd1,&buf1,sizeof(Account)))
		{
			buf1.money += acc.money;
			buf.money -= acc.money;

			close(fd);
			close(fd1);
	
			fd1 = open(path1,O_RDWR);
			if(write(fd1,&buf1,sizeof(Account)) < 0)
			{
				printf("写入对方数据失败\n");
				return -2;
			}
			else
			{
				printf("转帐成功\n");
			}
		
			fd = open(path,O_RDWR);
			if(write(fd,&buf,sizeof(Account)) < 0)
			{
				printf("写入自身数据失败\n");
				return -2;
			}
			else
			{
				printf("余额：%.2lf\n",buf.money);
				return 2;
			}
			close(fd);
			close(fd1);
		}
		else
		{
			printf("读取对方数据丢失\n");
			return -2;
		}
	}
	else
	{
		printf("对方账号不存在\n");
		return -2;
	}

}

int main()
{
	int msgid_ctos = msgget(ftok(".",100),IPC_CREAT|0644);
	if(0 > msgid_ctos)
	{
		perror("msgget");
		return -1;
	}
	int msgid_stoc = msgget(ftok(".",200),IPC_CREAT|0644);
	if(0 > msgid_stoc)
	{
		perror("msgget");
		return -1;
	}

	long user1 = 0,user2 = 0;

	for(;;)
	{
		Msg msg = {};
		// 接收消息
		msgrcv(msgid_ctos,&msg,sizeof(Msg),114,MSG_NOERROR);
		if(msg.flag == 0)
		{

			printf("转帐帐号:%ld\n",msg.acc.user);
			printf("转帐金额:%.2lf\n",msg.acc.money);
			user1 = msg.acc.user;
	
			int result = transf_a(msg.acc);

			Msg msg2 = {224};
			if(result == 0)
			{
				msg2.acc.user = user1;
				msg2.flag = 0;
			}		
			else
			{
				msg2.flag = -1;
			}
			msgsnd(msgid_stoc,&msg2,sizeof(Msg)-sizeof(msg2.type),0);	

		}
		else
		{
			printf("帐向帐号:%ld\n",msg.acc.user);
			printf("转帐金额:%.2lf\n",msg.acc.money);
			user2 = msg.acc.user;

			int result = transf_b(msg.acc);

			Msg msg2 = {224};
			if(result != -2)
			{
				msg2.acc.user = user2;
				msg2.flag = 2;
			}		
			else
			{
				msg2.flag = -2;
			}
			msgsnd(msgid_stoc,&msg2,sizeof(Msg)-sizeof(msg2.type),0);
		}
	}
	pause();
}
