// IOtest.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"

//头文件包含
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

  
using namespace std; //用到的主要是cin cout
 

//////////////////////////////////////////////////////
//相关声明
typedef int  SElemType;
enum Status { OK, ERROR, NOSPACE};

#define   MAXSIZE     100
typedef	 struct	{
	SElemType	*base;  // 栈底指针
	SElemType	*top;  // 栈顶指针
	int		stacksize;  // 栈可用的最大容量
}	SqStack;

SqStack S;

////////////////////////////////////////////////
//栈的基本操作
//初始化栈
Status  InitStack(SqStack &s)
{
	s.base = new  SElemType[MAXSIZE];  // 申请存储空间
	if (!s.base)
	{
		return  NOSPACE;
	}
		
	s.top = s.base;		// top初始为base,表示栈为空
	s.stacksize = MAXSIZE;	// 最大容量

	return OK;
}
Status  DeleteStack(SqStack &s)
{ 
	if (!s.base)
	{
		return  ERROR;
	}

	delete []s.base;

	return OK;
}
//入栈
Status  Push(SqStack &s, SElemType e)
{
	if (s.top - s.base >= s.stacksize)
	{
		return  ERROR;
	}
		
	*s.top++ = e;

	return  OK;
}
//出栈
Status Pop(SqStack &s, SElemType &e)
{
	if (s.top == s.base)
	{
		return   ERROR;
	}
		
	s.top--;
	e = *s.top;
	

	return OK;
}
//是否存在数据
bool IsEmpty(SqStack &s)
{
	if (s.top == s.base)
	{
		return   true;
	}

	 

	return false;
}
//遍历并显示列表节点数据
void SPrint(SqStack &S)
{
	SElemType * ptr = S.base;

	for (int i = 0; i < S.stacksize;i++)
	{
		cout << ptr[i] << " ";  

		if ((S.base + i) == S.top)
		{
			break;
		}
	}

	cout << endl;
}

////////////////////////////////////////////////////////////////////
//应用1：数制转换
void TenToEgiht(int Source,string &re)
{
	SqStack tS;
	InitStack(tS); 

	do        //数制转换过程
	{
		int remainder = Source % 8; //求余数
		Push(tS,remainder);         //余数入栈

		Source = Source / 8;        //求倍数
	} while (Source != 0);          

	re.clear();

	while (!IsEmpty(tS))  //
	{
		int remainder = 0;
		if (Pop(tS, remainder) == OK)
		{
			re = re + char(remainder+'0');
		}
		else
		{
			break;
		} 
	}

	DeleteStack(tS);
}
 
////////////////////////////////////////////////////////////////////
//应用2 ：飞程序监测
//函数进入点记录
void CheckIn(SqStack &S,unsigned int Addr)
{
	for (int i = 0; i < S.top - S.base;i++)
	{
		cout << "   ";
	}

	Push(S,Addr);
	cout << "->进入函数:" << Addr  << endl;

}

//函数出口检查
void CheckOut(SqStack &S, unsigned int Addr)
{
	for (int i = 0; i < S.top - S.base-1; i++)
	{
		cout << "   ";
	}

	cout << "<-走出函数:" << Addr << endl;

	bool IsRight = false;
	 
	SElemType e=0;
	if (Pop(S, e)==OK)
	{ 
		if (e == Addr)
		{
			IsRight = true;
		}
	}
	else
	{
		cout << "出栈出错" << endl;
	}
	
	if (IsRight == false) //程序路径错误，进行错误处理
	{
		///////////////////
		cout << "函数执行路径错误！" << "当前地址:" << Addr << "   正确地址:" << e << endl;
	}

	if (S.top == S.base)
	{
		cout << endl << endl;
	}
}


//函数1
int fun1()
{
	CheckIn(S,(int)fun1);

	int a = 0;
	a++;

	CheckOut(S, (int)fun1);

	return a;
}


//函数2
int fun2()
{
	CheckIn(S, (int)fun2);
	int RetrunData = 0;

	fun1();
	int a = 2;
	a++;

	RetrunData = a;

	if (a == 3)
	{
		cout << "error" << endl;
		RetrunData = 0;
	}

	CheckOut(S, (int)fun2);

	return RetrunData;
}


//函数3
int fun3()
{
	CheckIn(S, (int)fun3);

	fun2();
	int a = 2;
	a++;

	if (a == 3)
	{
		cout << "error" << endl;
		return  0;
	}
	 
	CheckOut(S, (int)fun3);

	return a;
}


//////////////////////////////////////////////////////
//应用：斐波那契数列
long Fib(long n)
{
	if (n == 1 || n == 2)
	{
		return 1;
	}

	return Fib(n - 1) + Fib(n - 2);
}


long Fib2(long n) 
{
	if (n == 1 || n == 2) 
	{
		return 1;
	}

	int t1 = 1; 
	int t2 = 1;
	int t3 = 0;
		
	for (int i = 3; i <= n; i++)
	{
			t3 = t1 + t2;
			t1 = t2; 
			t2 = t3;
	}
		
	return t3; 
}

///////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{ 
	int x = 0;
	while (x != 100)
	{ 
		string result;
		cout << "请输入十进制数:";
		cin >> x;
		TenToEgiht(x, result);
		cout << "对应八进制数为:"<<result << endl<<endl;
	}

	//初始化栈
	//InitStack(S);

	//CheckIn(S, (int)_tmain);

	//测试栈的功能
	//fun1();
	//fun2();
	//fun3();

	//CheckOut(S, (int)_tmain);
	//程序暂停
	system("PAUSE");

	return 0; 
}


