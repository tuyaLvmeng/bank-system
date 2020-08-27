#ifndef STRUCT_H
#define STRUCT_H

#define L_NAME 20		//姓名长度
#define L_ID   20		//身份证号长度
#define L_PW   20		//密码长度
#define L_PATH 20		//文件名长度

typedef struct Account
{
	long user;			//帐号
	char name[L_NAME];	//姓名
	char id[L_ID];		//身份证号
	char pw[L_PW];		//密码
	double money;		//金额
}Account;

typedef struct Msg		//消息结构体
{
	long type;			//消息类型
	int flag;
	Account acc;		//用户结构体
}Msg;

#endif//STRUCT_H
