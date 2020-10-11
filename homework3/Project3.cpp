#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <iomanip>

using namespace std;

#define MAXSIZE 100

//////////////////////////////////////////////////////

typedef int QElemType;
enum Status { OK, ERROR, NOSPACE };

typedef struct
{
	QElemType *base;
	int front;//队头指针
	int rear;//队尾指针
	int tag;//空满标志符
}SqQueue;

SqQueue s;

Status InitQueue(SqQueue &s)//初始化循环队列
{
	s.base = new QElemType[MAXSIZE];

	if (!s.base) return NOSPACE;

	s.front = s.rear = 0;//空队列

	s.tag = 0;//标识符=0，未满

	return OK;
}

int QueueLength(SqQueue s)//队列长度
{
	if (s.tag == 1) return 0;//满队列
	return ((s.rear-s.front + MAXSIZE)% MAXSIZE);
}

Status EnQueue(SqQueue &s,int e)//插入队列
{
	if (QueueLength(s) == MAXSIZE) return NOSPACE;

	s.base[s.rear] = e;
	s.rear = (s.rear + 1) % MAXSIZE;

	if (s.rear == s.front && s.tag == 0) s.tag = 1;//判断队列是否满

	return OK;
}

Status DeQueue(SqQueue &s, int &e)
{
	if (QueueLength(s) == 0) return ERROR;

	e = s.base[s.front];
	s.front = (s.front + 1) % MAXSIZE;

	s.tag = 0;//队列不为满

	return OK;
}

///////////////////////////////////////////////////////////////
int main()
{
	SqQueue test;
	InitQueue(test);

	int a[5] = { 1,3,5,7,9 },b[5];//测试数组
	int i;

	for (i = 0; i < 5; i++)
	{
		EnQueue(test, a[i]);//插入队列
	}

	for (i = 0; i < 5; i++)
	{
		DeQueue(test, b[i]);//从队列里删除
		cout << b[i] << " ";
	}

	return 0;
}