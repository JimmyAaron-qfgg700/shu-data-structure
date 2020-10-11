#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <iomanip>

using namespace std;

typedef int  ElemType;
enum Status { OK=1, ERROR=0, NOSPACE=0 };

//////////////////////////////////////////////////////////

typedef struct
{
	int totalnum;//二维数组元素个数
	int m;//行
	int n;//列
	ElemType *data;//存放数据的一维数组
}Array;

Status InitArray(Array &test, int m, int n,int totalnum)//初始化数组
{
	if (m < 0 || n < 0) return ERROR;
	if (m * n != totalnum) return ERROR;//下标非法情况
	if (!test.data) return ERROR;

	test.data = new int[totalnum];

	test.m = m;
	test.n = n;
	test.totalnum = totalnum;

	return OK;
}

Status AssignArray(Array &test, int m, int n, int &t)//输入数组元素
{
	if (m<0 || m>=test.m) return ERROR;
	if (n<0 || n>=test.n) return ERROR;

	int position;
	position = m * test.n + n;//确定二维数组的位置

	if (position < test.totalnum)
	{
		test.data[position] = t;
		return OK;
	}
	return ERROR;
}

int ValueArray(Array test, int m, int n)//输出数组元素
{
	if (m<0 || m>=test.m) return ERROR;
	if (n<0 || n>=test.n) return ERROR;

	int position;
	position = m * test.n + n;
	if (position < test.totalnum)
	{
		return test.data[position];
	}
	return 0;
}

Status DestoryArray(Array &test)//删除数组
{
	if (!test.data) return ERROR;

	delete []test.data;

	return OK;
}

///////////////////////////////////////////////////////////
int same(Array &test, int m, int n, int t)//遍历整个数组
{
	int i, j, t1;
	int count = 0;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			t1 = ValueArray(test, i, j);
			if (t == t1) count++;
		}
	}

	return count;//返回相同元素个数
}

int main()
{
	int i, j;
	int t, sign = -1;
	int b[10][10];

	Array test;

	int m, n;
	cout << "行数m=";
	cin >> m;
	cout << "列数n=";
	cin >> n;

	InitArray(test, m, n, m * n);

	for (i = 0; i < m; i++)//输入数组元素
	{
		for (j = 0; j < n; j++)
		{
			cin >> b[i][j];
		}
	}

	for (i = 0; i < m; i++)//放入数组结构体中
	{
		for (j = 0; j < n; j++)
		{
			AssignArray(test, i, j, b[i][j]);
		}
	}
	
	for (i = 0; i < m; i++)//去数组中每一个元素放入same函数中判断是否有重复
	{
		for (j = 0; j < n; j++)
		{
			t=ValueArray(test, i, j);

			sign = same(test, m, n, t);

			if ( sign > 1)//若>1，则有元素重复
			{
				cout << "no";

				return 0;
			}
		}
	}

	cout << "yes";

	DestoryArray(test);

	return 0;
}
/////////////////////////////////////////////////////////////////////
//时间复杂度为O(n^4)