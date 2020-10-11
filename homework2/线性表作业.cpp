#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

typedef int ElemType; 
enum Status { OK,ERROR,TRUE=1,FALSE=0 };

typedef struct LNode //定义线性表 
{
	ElemType data;
	struct LNode *next; 
	struct LNode *pri;
}LNode,*LinkList;

Status InitList(LinkList &L)//初始化链表 
{
	L=new LNode;
	L->next=NULL;
	
	return OK;
} 

Status CreateList(LinkList &L, int n) //创建链表-后插法 
{
	LNode *r=L;
	
	if(n<1)
	{
		return ERROR;
	}
	
	for (int i = 0; i<n; i++)
	{
		LNode *p = new LNode;
		cout << "请输入第"<< i+1 <<"个节点的数据:";
		cin >> p->data;

		p->next = NULL;
		r->next = p;
		r = p;
	}
	
	return OK;
}

Status CreateList_array(LinkList &L, int n,int *Data) //创建数组链表-后插法 
{
	LNode *r=L;
	
	if(n<1)
	{
		return ERROR;
	}
	
	for(int i=0;i<n;i++)
	{
		LNode *p=new LNode;
		p->data=Data[i];
		p->next=NULL; r->next=p;
		r=p; 
	}
	
	return OK;
}

Status InsertList(LinkList &L,int i,ElemType e)//插入链表 
{
	LNode * p = L; 
	int j = 0;

	while (p&&j<i - 1) 
	{
		p = p->next;
		j++;
	}

	if (!p || j > i - 1)
	{
		return ERROR;
	}
	
	LNode * s = new LNode;  
	s->data = e;
	s->next = p->next;
	p->next = s;

	return OK;
}

void PrintList(LinkList &L)//输出链表 
{
	LNode *p=L->next;
	
	while(p)
	{
		cout<<p->data<<" ";
		p=p->next;
	}
}

Status Traversal(LinkList L,int num)//遍历链表是否有重复数据 
{
	LNode *p=L->next;
	
	while(p)
	{
		if(p->data==num) return TRUE;
			
		p=p->next;
	}
	return FALSE; 
}

void DeleteList(LinkList &L)//删除整个链表 
{
	LNode *p=L;
	
	while(p)
	{
		LNode *r=p->next;
		
		delete p;
		p=r;
	}
}

///////////////////////////////////////////////////////////////////////

int minus_array(LinkList A,LinkList B,LinkList C)//差集 
{
	int count=0,n=1;
	LNode *p=A->next; LNode *q=B->next;
	
	while(p)
	{
		if(Traversal(B,p->data)==0)
		{
			InsertList(C,n,p->data);
			count++;
			n++;
		}
			
		p=p->next;
	}
	return count;
}

void four(LinkList &A,LinkList &B,LinkList &C)//第四题 
{
	int count=0;
	int a,b;
	
	cout<<"A链表元素个数：";
	cin>>a; 
	cout<<"B链表元素个数：";
	cin>>b;
	
	cout<<endl;
	cout<<"A链表"<<endl;
	CreateList(A,a);
	cout<<endl;
	
	cout<<"B链表"<<endl;
	CreateList(B,b);
	cout<<endl;
	
	//int Adata[6] = { 2, 4, 6, 8 ,10 ,12};
	//CreateList_array(A, 6, Adata);

	//int Bdata[7] = { 2, 4, 6, 9, 11, 15, 20 };
	//CreateList_array(B, 7, Bdata);
	
	count=minus_array(A,B,C);
	
	PrintList(A);
	cout<<endl; 
	PrintList(B);
	cout<<endl;
	cout<<"差集："; 
	PrintList(C);
	cout<<endl;
	cout<<"个数："<<count;
	cout<<endl;
	
	system("pause"); 
}

void six(LinkList &A)//第六题 
{
	//int Adata[7] = { 2, 1000 ,-220 ,14 ,4 ,0 ,-1 };
	//CreateList_array(A, 7, Adata);
	
	int n;
	
	cout<<"元素个数：" ;
	cin>>n;
	cout<<endl;
	
	CreateList(A,n); 
	
	LNode *p=A->next;
	
	int max=p->data;
	
	while(p)
	{
		if(p->data > max) max=p->data;
		p=p->next;
	}
	
	cout<<endl<<max;
}

/////////////////////////////////////////////////////////////////////////////
int main()
{
	int num; 
	
	LinkList A;
	LinkList B;
	LinkList C;
	
	InitList(A);
	InitList(B);
	InitList(C);
	
	cout<<"题号："; 
	cin>>num ;
	
	switch(num)
	{
		case 4:
			four(A,B,C);
			break;
		case 6:
			six(A);	
			break;
	}
	
	DeleteList(A);
	DeleteList(B);
	DeleteList(C);
	
	return 0;
}
