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

//登录
int login(Account acc)
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
			if((acc.user == buf.user) && strcmp(acc.pw,buf.pw) == 0)
			{
				close(fd);
				printf("登陆成功\n");
				return 0;
			}
			else
			{
				printf("帐号或密码错误\n");
				close(fd);
				return -1;
			}
		}
		else
		{
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
		msgrcv(msgid_ctos,&msg,sizeof(Msg),110,MSG_NOERROR);
	
		printf("user:%ld\n",msg.acc.user);
		printf("pw:%s\n",msg.acc.pw);
		user = msg.acc.user;
		int result = login(msg.acc);

		Msg msg2 = {220};
		if(result == 0)
		{
			msg2.acc.user = user;
		}		
		else
		{
			msg2.acc.user = user + 1;
		}
		msgsnd(msgid_stoc,&msg2,sizeof(Msg)-sizeof(msg2.type),0);	


	}
	pause();
}
