// IOtest.cpp : �������̨Ӧ�ó������ڵ㡣
//

//#include "stdafx.h"

//ͷ�ļ�����
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

  
using namespace std; //�õ�����Ҫ��cin cout
 

//////////////////////////////////////////////////////
//�������
typedef int  SElemType;
enum Status { OK, ERROR, NOSPACE};

#define   MAXSIZE     100
typedef	 struct	{
	SElemType	*base;  // ջ��ָ��
	SElemType	*top;  // ջ��ָ��
	int		stacksize;  // ջ���õ��������
}	SqStack;

SqStack S;

////////////////////////////////////////////////
//ջ�Ļ�������
//��ʼ��ջ
Status  InitStack(SqStack &s)
{
	s.base = new  SElemType[MAXSIZE];  // ����洢�ռ�
	if (!s.base)
	{
		return  NOSPACE;
	}
		
	s.top = s.base;		// top��ʼΪbase,��ʾջΪ��
	s.stacksize = MAXSIZE;	// �������

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
//��ջ
Status  Push(SqStack &s, SElemType e)
{
	if (s.top - s.base >= s.stacksize)
	{
		return  ERROR;
	}
		
	*s.top++ = e;

	return  OK;
}
//��ջ
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
//�Ƿ��������
bool IsEmpty(SqStack &s)
{
	if (s.top == s.base)
	{
		return   true;
	}

	 

	return false;
}
//��������ʾ�б�ڵ�����
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
//Ӧ��1������ת��
void TenToEgiht(int Source,string &re)
{
	SqStack tS;
	InitStack(tS); 

	do        //����ת������
	{
		int remainder = Source % 8; //������
		Push(tS,remainder);         //������ջ

		Source = Source / 8;        //����
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
//Ӧ��2 ���ɳ�����
//����������¼
void CheckIn(SqStack &S,unsigned int Addr)
{
	for (int i = 0; i < S.top - S.base;i++)
	{
		cout << "   ";
	}

	Push(S,Addr);
	cout << "->���뺯��:" << Addr  << endl;

}

//�������ڼ��
void CheckOut(SqStack &S, unsigned int Addr)
{
	for (int i = 0; i < S.top - S.base-1; i++)
	{
		cout << "   ";
	}

	cout << "<-�߳�����:" << Addr << endl;

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
		cout << "��ջ����" << endl;
	}
	
	if (IsRight == false) //����·�����󣬽��д�����
	{
		///////////////////
		cout << "����ִ��·������" << "��ǰ��ַ:" << Addr << "   ��ȷ��ַ:" << e << endl;
	}

	if (S.top == S.base)
	{
		cout << endl << endl;
	}
}


//����1
int fun1()
{
	CheckIn(S,(int)fun1);

	int a = 0;
	a++;

	CheckOut(S, (int)fun1);

	return a;
}


//����2
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


//����3
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
//Ӧ�ã�쳲���������
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
		cout << "������ʮ������:";
		cin >> x;
		TenToEgiht(x, result);
		cout << "��Ӧ�˽�����Ϊ:"<<result << endl<<endl;
	}

	//��ʼ��ջ
	//InitStack(S);

	//CheckIn(S, (int)_tmain);

	//����ջ�Ĺ���
	//fun1();
	//fun2();
	//fun3();

	//CheckOut(S, (int)_tmain);
	//������ͣ
	system("PAUSE");

	return 0; 
}


