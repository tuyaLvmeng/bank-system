#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <string.h>
#include <getch.h>
#include <stdbool.h>

void clear_stdin(void);								//清空输入缓存区

char* get_str(char* str,size_t len);				//对输入的字符串进行处理

char get_cmd(char start,char end);					//获取命令

char* get_pw(char* passwd,bool is_show,size_t size);//输入密码的处理

#endif//TOOLS_H
