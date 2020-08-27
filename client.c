#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include "struct.h"
#include "tools.h"


static long online = 0;		//记录当前的用户
static int lock = 0;		//密码错误的次数

int msgid_ctos;				//消息队列的标识符
int msgid_stoc;				//消息队列的标识符

int init(void);				//创建消息队列
void open_acc(void);		//开户操作
void destory(void);			//销户操作
void login(void);			//登录操作
void unlock(void);			//解锁操作
void user_menu(void);		//用户菜单
void menu(void);			//菜单(开户,销户)
void save(void);			//存钱
void take(void);			//取钱
void transf(void);			//转账
void select_money(void);	//查询余额
void change_pw(void);		//改变密码

int main()
{
	init();
	menu();
}

int init(void)				//创建消息队列
{
	//ftok是获取ＩＰＣ键值
	msgid_ctos = msgget(ftok(".",100),IPC_CREAT);
	if(0 > msgid_ctos)
	{
		perror("msgget");
		return -1;
	}
	msgid_stoc = msgget(ftok(".",200),IPC_CREAT);
	if(0 > msgid_stoc)
	{
		perror("msgget");
		return -1;
	}
	return 0;
}

void login(void)			//登录操作
{
	if(lock >= 3)
	{
		printf("您的账户已被锁定，请解锁\n");
		getch();
		return;
	}
	Account acc = {};
	Msg msg = {110};
	printf("请输入帐号：");
	scanf("%ld",&acc.user);
	getchar();
	printf("请输入密码：");
	get_pw(acc.pw,true,L_PW);
	msg.acc = acc;
	//发送消息
	msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);

	Msg msg2 = {};
	// 接收消息
	msgrcv(msgid_stoc,&msg2,sizeof(Msg),220,MSG_NOERROR);
	
	if(acc.user == msg2.acc.user)
	{
		online = acc.user;
		printf("\n用户:%ld,登陆成功\n",online);
		getch();
		user_menu();
	}
	else
	{	
		lock++;	
		printf("\n帐号或密码错误\n");
		getch();
	}
}

void open_acc(void)			//开户操作
{

	Account acc = {};
	Msg msg = {111};
	printf("请输入姓名：");
	get_str(acc.name,L_NAME);
	printf("请输入身份证：");
	get_str(acc.id,L_ID);
	printf("请输入密码：");
	get_str(acc.pw,L_PW);
	msg.acc = acc;
	//发送消息
	msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);

	Msg msg2 = {};
	// 接收消息
	msgrcv(msgid_stoc,&msg2,sizeof(Msg),221,MSG_NOERROR);
	printf("msg2:%s,acc:%s\n",msg2.acc.name,acc.name);
	if(strcmp(acc.name,msg2.acc.name) == 0)
	{
		printf("帐号:%ld,开户成功\n",msg2.acc.user);
	}
	else
	{		
		printf("开户失败\n");
	}
	getch();
}

void save(void)				//存款
{
	Account acc = {};
	Msg msg = {112};

	printf("请输入存款金额:");
	scanf("%lf",&acc.money);
	getchar();
	acc.user = online;	
	msg.acc = acc;
	//发送消息
	msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);

	Msg msg2 = {};
	// 接收消息
	msgrcv(msgid_stoc,&msg2,sizeof(Msg),222,MSG_NOERROR);
	if(msg2.acc.user == online)
	{
		printf("存钱成功\n");
	}
	else
	{
		printf("存钱失败\n");
	}
	getch();
}

void take(void)				//取款
{
	Account acc = {};
	Msg msg = {113};
	printf("请输入取款金额:");
	scanf("%lf",&acc.money);
	getchar();
	acc.user = online;	
	msg.acc = acc;
	msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);

	Msg msg2 = {};
	// 接收消息
	msgrcv(msgid_stoc,&msg2,sizeof(Msg),223,MSG_NOERROR);
	if(msg2.acc.user == online)
	{
		printf("取钱成功\n");
	}
	else if(msg2.acc.user == online + 2)
	{
		printf("余额不足，请充值\n");
	}
	else
	{
		printf("取钱失败\n");
	}
	getch();
}
void transf(void)			//转账
{
	Account acc = {};
	Msg msg = {114};
	printf("请输入转帐金额:");
	scanf("%lf",&acc.money);
	getchar();
	acc.user = online;	
	msg.acc = acc;
	msg.flag = 0;
	msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);

	Msg msg2 = {};
	// 接收消息
	msgrcv(msgid_stoc,&msg2,sizeof(Msg),224,MSG_NOERROR);
	if(msg2.flag == 0)
	{
		printf("请输入要转帐的帐号：");
		scanf("%ld",&acc.user);
		getchar();
		msg.acc = acc;
		msg.flag = 1;
		msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);
		
		// 接收消息
		msgrcv(msgid_stoc,&msg2,sizeof(Msg),224,MSG_NOERROR);
		
		if(msg2.flag == 2)  printf("转帐成功\n");
		else printf("转帐失败\n");
	}
	else
	{
		printf("余额不足\n");
	}
	getch();
}
void select_money(void)			//余额查询
{
	Account acc = {};
	Msg msg = {115};
	acc.user = online;	
	msg.acc = acc;
	msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);

	Msg msg2 = {};
	// 接收消息
	msgrcv(msgid_stoc,&msg2,sizeof(Msg),225,MSG_NOERROR);
	if(msg2.acc.user == online)
	{
		printf("余额：%.2lf\n",msg2.acc.money);
	}
	else
	{
		printf("查询失败\n");
	}
	getch();
}
void change_pw(void)			//修改密码
{
	Account acc = {};
	Msg msg = {116};
	printf("请输入新密码:");
	get_pw(acc.pw,true,L_PW);
	acc.user = online;	
	msg.acc = acc;
	msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);

	Msg msg2 = {};
	// 接收消息
	msgrcv(msgid_stoc,&msg2,sizeof(Msg),226,MSG_NOERROR);
	if(msg2.acc.user == online)
	{
		printf("\n改密成功\n");
	}
	else
	{
		printf("\n改密失败\n");
	}
	getch();
}

void destory(void)				//销户
{
	Account acc = {};
	Msg msg = {117};
	printf("请输入销户帐号:");
	scanf("%ld",&acc.user);
	getchar();
	printf("请输入姓名：");
	get_str(acc.name,L_NAME);
	printf("请输入身份证：");
	get_str(acc.id,L_ID);
	msg.acc = acc;
	msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);

	Msg msg2 = {};
	// 接收消息
	msgrcv(msgid_stoc,&msg2,sizeof(Msg),227,MSG_NOERROR);
	if(msg2.acc.user == 0)
	{
		printf("销户成功\n");
	}
	else if(msg2.acc.user == 1)
	{
		printf("相关信息错误\n");
	}
	else if(msg2.acc.user == 2)
	{
		printf("帐号不存在\n");
	}
	getch();
}

void unlock(void)			//解锁
{
	if(lock < 3)
	{
		printf("您没有被锁定，sorry\n");
		getchar();
		return;
	}
	Account acc = {};
	Msg msg = {118};
	printf("请输入帐号：");
	scanf("%ld",&acc.user);
	getchar();
	printf("请输入姓名：");
	get_str(acc.name,20);
	printf("请输入身份证：");
	get_str(acc.id,L_ID);
	msg.acc = acc;
	msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);

	Msg msg2 = {};
	// 接收消息
	msgrcv(msgid_stoc,&msg2,sizeof(Msg),228,MSG_NOERROR);
	
	printf("in:%ld,back:%ld\n",acc.user,msg2.acc.user);
	
	if(acc.user == msg2.acc.user)
	{
		printf("%ld,解锁成功\n",acc.user);
		lock = 0;
		online = acc.user;
		change_pw();
	}
	else
	{	
		printf("相关信息错误\n");
		getch();
	}
}

void user_menu(void)
{
	while(true)
	{
		system("clear");
		printf("===0===0=阿强银行=0===0===\n");
		printf("#         1.存钱         #\n");
		printf("#         2.取钱         #\n");
		printf("#         3.转帐         #\n");
		printf("#         4.查询         #\n");
		printf("#         5.改密         #\n");
		printf("#         0.返回         #\n");
		printf("--------------------------\n");
		switch(get_cmd('0','5'))
		{
			case '1':save(); break;
			case '2':take(); break;
			case '3':transf(); break;
			case '4':select_money(); break;
			case '5':change_pw();break;
			case '0':return;
		}
	}
}

void menu(void)
{
	while(true)
	{
		system("clear");
		printf("===0===0=阿强银行=0===0===\n");
		printf("#         1.开户         #\n");
		printf("#         2.销户         #\n");
		printf("#         3.登录         #\n");
		printf("#         4.解锁         #\n");
		printf("#         0.退出         #\n");
		printf("--------------------------\n");
		switch(get_cmd('0','4'))
		{
			case '1':open_acc(); break;
			case '2':destory(); break;
			case '3':login(); break;
			case '4':unlock(); break;
			case '0':return;
		}
	}
}

