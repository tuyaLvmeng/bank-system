#include"tools.h"

//清空输入缓存区
void clear_stdin(void)
{
	stdin->_IO_read_ptr = stdin->_IO_read_end;
}

//对输入的字符串进行处理
char* get_str(char* str,size_t len)
{
	if(NULL==str)					//为空返回
	{
		perror("str");
		return NULL;
	}
	
	char *in = fgets(str,len,stdin);//获取输入指定的长度的字符串

	size_t cnt = strlen(str);

	if('\n'==str[cnt-1])			//将指定长度的最后一位进行截断
	{
		str[cnt-1]='\0';
	}

	clear_stdin();					//清空多余的字符

	return str;						//返回处理好的字符串
}

//获取命令
char get_cmd(char start,char end)
{
	clear_stdin();

	printf("请输入指令:");

	while(true)
	{
		char val = getch();
		if(val>=start && val<=end)
		{
			printf("%c\n",val);
			return val;
		}
	}
}

//输入密码的处理
//passwd输入 is_show是否显示 size 长度限制
char* get_pw(char* passwd,bool is_show,size_t size)
{
	if(NULL==passwd) return NULL;			//为空返回

	int count = 0;							//记录字符的个数

	do
	{
		char val = getch();

		if(127==val)						//ascii 127 为退格
		{
			if(count>0)
			{
				if(is_show) printf("\b \b");//显示的话要把上一个字符覆盖
				count--;
			}
			continue;
		}
		else if(10==val)					//10为回车符 结束标志
		{
			break;
		}
		passwd[count++]=val;
		if(is_show) printf("*");
	}while(count<size-1);

	passwd[count]='\0';

	return passwd;
}
