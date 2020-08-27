#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include "struct.h"
#include "tools.h"

#define ID_NUM 9

// 2个消息队列
static int msgid_ctos;
static int msgid_stoc;
// 存储子进程ID
static pid_t sub_pid[ID_NUM];

void sigint(int sig)
{
    // 对msqid标识的消息队列执行cmd操作
	msgctl(msgid_ctos,IPC_RMID,NULL);
	msgctl(msgid_stoc,IPC_RMID,NULL);
	
	int i;
	for(i=0; i<ID_NUM; i++)
	{
		kill(sub_pid[i],SIGKILL);
	}
}

int main()
{ 
    // 注册信号 
	signal(SIGINT,sigint);

	msgctl(msgid_ctos,IPC_RMID,NULL);
	msgctl(msgid_stoc,IPC_RMID,NULL);

    // IPC_CREAT 创建消息队列,IPC_EXCL 如果存在则创建失败
    // IPC键值，由ftok函数自动生成
	msgid_ctos = msgget(ftok(".",100),IPC_CREAT|IPC_EXCL|0644);
	msgid_stoc = msgget(ftok(".",200),IPC_CREAT|IPC_EXCL|0644);
	
    // 创建一个新的进程
	sub_pid[0] = vfork();
	if(0 == sub_pid[0])
	{
		execl("login","login",NULL);
		printf("登陆子进程创建成功...\n");
	}

	sub_pid[1] = vfork();
	if(0 == sub_pid[1])
	{
		execl("open","open",NULL);
		printf("开户子进程创建成功...\n");
	}
	sub_pid[2] = vfork();
	if(0 == sub_pid[2])
	{
		execl("save","save",NULL);
	}
	
	sub_pid[3] = vfork();
	if(0 == sub_pid[3])
	{
		execl("take","take",NULL);
	}
	
	sub_pid[4] = vfork();
	if(0 == sub_pid[4])
	{
		execl("transf","transf",NULL);
	}

	sub_pid[5] = vfork();
	if(0 == sub_pid[5])
	{
		execl("select","select",NULL);
	}

	sub_pid[6] = vfork();
	if(0 == sub_pid[6])
	{
		execl("change","change",NULL);
	}	

	sub_pid[7] = vfork();
	if(0 == sub_pid[7])
	{
		execl("destory","destory",NULL);
	}

	sub_pid[8] = vfork();
	if(0 == sub_pid[8])
	{
		execl("unlock","unlock",NULL);
	}

	//等待子进程的退出
	while(-1 != wait(NULL));
}
