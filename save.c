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

//存钱
int save(Account acc)
{
    char path[L_PATH] = "./data/";
	char ex[L_PATH] = ".dat";
	sprintf(path,"%s%ld%s",path,acc.user,ex);

	Account buf = {};
	int fd = open(path,O_RDWR);
	if(0 != read(fd,&buf,sizeof(Account)))
	{
		printf("原:%.2lf,",buf.money);
		buf.money += acc.money;
		printf("现:%.2lf\n",buf.money);
		close(fd);
		fd = open(path,O_RDWR);
		if(write(fd,&buf,sizeof(Account)) < 0)
		{
			printf("写入失败\n");
			return -1;
		}
		else
		{
			printf("写入成功\n");
			printf("余额：%.2lf元\n",buf.money);
			return 0;
		}
	}
	else
	{
		printf("读取数据丢失\n");
		return -1;
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

	long user = 0;

	for(;;)
	{
		Msg msg = {};
		// 接收消息
		msgrcv(msgid_ctos,&msg,sizeof(Msg),112,MSG_NOERROR);
		printf("user:%ld\n",msg.acc.user);
		printf("money:%lf\n",msg.acc.money);
		user = msg.acc.user;
	
		int result = save(msg.acc);

		Msg msg2 = {222};
		if(result == 0)
		{
			msg2.acc.user = user;
		}		
		else
		{
			msg2.acc.user = user + 1;
		}
		msgsnd(msgid_stoc,&msg2,sizeof(Msg)-sizeof	(msg2.type),0);	
	}
	pause();
}
