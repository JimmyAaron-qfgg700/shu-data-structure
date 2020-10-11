#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <iomanip>

using namespace std;

typedef int  SElemType;
enum Status { OK, ERROR, NOSPACE ,TRUE=1 ,FLASE=0};

#define MAXSIZE 100

//////////////////////////////////////////////////////////

typedef struct//定义双栈结构体
{
	int top[2], bot[2];//两个栈的栈顶和栈底
	SElemType *v;//栈数组
	int m;//栈最大可容纳元素个数
}DblStack;

DblStack s;

Status InitStack(DblStack &s,int m)//初始化双栈
{
	s.v = new int[m];
	if (!s.bot) return NOSPACE;

	s.bot[0] = -1;
	s.bot[1] = m;
	s.top[0] = -1;
	s.top[1] = m;

	s.m = MAXSIZE;

	return OK;
}

Status FullStack(DblStack s)//判断两个栈是否有冲突
{
	if (s.top[0] + 1 >= s.top[1]) return TRUE;
	else return FLASE;
}

Status EmptyStack0(DblStack s)//判断0号栈是否为空
{
	if (s.top[0] == -1) return TRUE;
	else return FLASE;
}

Status EmptyStack1(DblStack s,int m)//判断1号栈是否为空
{
	if (s.top[1] == m) return TRUE;
	else return FLASE;
}

Status Push0(DblStack &s, SElemType &e)//0号栈入栈
{
	if (FullStack(s) == 1) return NOSPACE;//判断是否栈满
	else
	{
		s.top[0]++;
		s.v[s.top[0]] = e;
	}

	return OK;
}

Status Push1(DblStack &s, SElemType &e)//1号栈入栈
{
	if (FullStack(s) == 1) return NOSPACE;//判断是否栈满
	else
	{
		s.top[1]--;
		s.v[s.top[1]] = e;
	}

	return OK;
}

Status Pop0(DblStack &s, SElemType &e)//0号栈出栈
{
	if (EmptyStack0(s) == 1) return ERROR;//判断是否栈空
	else
	{
		e = s.v[s.top[0]];
		s.top[0]--;

		return OK;
	}
}

Status Pop1(DblStack& s, SElemType& e,int m)//1号栈出栈
{
	if (EmptyStack1(s,m) == 1) return ERROR;//判断是否栈空
	else
	{
		
		e = s.v[s.top[1]];
		s.top[1]++;

		return OK;
	}
}

////////////////////////////////////////////////////////////////

int main()
{
	DblStack test;
	InitStack(test, MAXSIZE);

	int i;
	int a[5] = { 10,20,300,40,50 }, b[5], c[5], d[5] = {9,8,5,3,1};//测试数组

	for (i = 0; i < 5; i++)//分别入栈
	{
		Push0(test, a[i]);
		Push1(test, d[i]);
	}

	for (i = 0; i < 5; i++)//0号栈出栈
	{
		Pop0(test, b[i]);
		cout << b[i]<<" ";
	}

	cout << endl;

	for (i = 0; i < 5; i++)//1号栈出栈
	{
		Pop1(test, c[i],MAXSIZE);
		cout << c[i] << " ";
	}
	return 0;
}