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

int unlock(Account acc)
{
    char path[L_PATH] = "./data/";
	char ex[L_PATH] = ".dat";
	sprintf(path,"%s%ld%s",path,acc.user,ex);

	if(0 == access(path,F_OK))
	{
		Account buf = {};
		int fd = open(path,O_RDWR);
		if(0 != read(fd,&buf,sizeof(Account)))
		{
			if(strcmp(acc.name,buf.name) == 0 && strcmp(acc.id,buf.id) == 0)
			{
				close(fd);
				printf("解锁成功\n");
				return 0;
			}
			else
			{
				printf("姓名或身份证错误\n");
				close(fd);
				return -1;
			}
		}
		else
		{
			close(fd);
			printf("读取信息失败\n");
			return -1;
		}
	}
	else
	{
		printf("此帐号不存在\n");
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
		msgrcv(msgid_ctos,&msg,sizeof(Msg),118,MSG_NOERROR);

		printf("user:%ld\n",msg.acc.user);
		printf("name:%s\n",msg.acc.name);
		printf("id:%s\n",msg.acc.id);
		user = msg.acc.user;
		int result = unlock(msg.acc);

		Msg msg2 = {228};
		if(result == 0)
		{
			msg2.acc.user = user;

			printf("msg2:%ld\n",msg2.acc.user);
		}		
		else
		{
			msg2.acc.user = user + 1;
		
			printf("msg2:%ld\n",msg2.acc.user);
		}
		msgsnd(msgid_stoc,&msg2,sizeof(Msg)-sizeof	(msg2.type),0);	


	}
	pause();
}
