#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <iomanip>

using namespace std;

typedef int  SElemType;
enum Status { OK, ERROR, NOSPACE };

//////////////////////////////////////////////////////////

#define MAXSIZE 100
typedef struct
{
	SElemType *base;//栈底指针
	SElemType *top;//栈顶指针
	int stacksize;//栈可用的最大容量
}SqStack;

SqStack s;

Status InitStack(SqStack &s)//初始化栈
{
	s.base = new SElemType[MAXSIZE];
	if (!s.base) return NOSPACE;

	s.top = s.base;//top=base即为空栈
	s.stacksize = MAXSIZE;

	return OK;
}

Status Push(SqStack &s, SElemType e)//入栈
{
	if (s.top - s.base >= s.stacksize) return ERROR;//判断是否还有空间

	*s.top++ = e;

	return OK;
}

Status Pop(SqStack &s, SElemType &e)//出栈
{
	if (s.top == s.base) return ERROR;//空栈无法出栈

	s.top--;
	e = *s.top;

	return OK;
}

Status DeleteStack(SqStack &s)//删除栈
{
	if (!s.base) return ERROR;

	delete []s.base;

	return OK;
}

///////////////////////////////////////////////////

int main()
{
	SqStack test;
	InitStack(test);

	char a[100];
	int a_num[100], b_num[100],c_num[100];
	int i;

	cout << "输入字符串：";
	cin>>a;

	for (i = 0; i < strlen(a); i++)
	{
		a_num[i] = a[i];//将字符串中每个字符转化为ASCII码入栈
		Push(test, a_num[i]);
	}

	for (i = 0; i < strlen(a) / 2; i++)//单双数不影响
	{
		b_num[i] = a_num[i];//将前一半ASCII码赋给另一个数组
	}

	for (i = 0; i < strlen(a)/2; i++)
	{
		Pop(test, c_num[i]);//出栈后一半ASCII码
	}

	for (i = 0; i < strlen(a) / 2; i++)//判断前后是否相等，相等即为回文字符
	{
		if (b_num[i] != c_num[i])
		{
			cout << "不是回文";
			return 0;
		}
	}
	cout << "是回文";

	DeleteStack(test);

	return 0;
}