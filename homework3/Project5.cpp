#include<stdio.h>
#include<string.h>
#include<iostream>

using namespace std;

void insert(char *s, char *t, int pos)
{
	char  backward[50];
	int i, j, length, length_b;
	int count = 0;

	length = strlen(t);

	for (i = pos,j=0; i < strlen(s); i++, j++)//将被插入位置后的字符串复制到临时字符数组
	{
		backward[j] = s[i];
		count++;
	}

	length_b = count;

	for (i = pos,j = 0; i < pos+length;i++, j++)//将待插入数组插入位置之后
	{
		s[i] = t[j];
	}

	for (i = pos + length,j = 0; i < pos + length + length_b; i++,j++)//将临时字符数组元素插入到插入元素之后
	{
		s[i] = backward[j];
	}

	for (i = 0; i < pos + length + length_b; i++)//输出数组
	{
		cout << s[i];
	}
}

int main()
{
	char test[50];
	char in[10];
	int pos;

	cout << "被插入字符串：";
	cin >> test;
	cout << "待插入字符串：";
	cin >> in;
	cout << "插入位置：";
	cin >> pos;

	insert(test, in, pos);

	return 0;
}