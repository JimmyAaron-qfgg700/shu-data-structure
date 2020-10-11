#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

typedef int ElemType; 
enum Status { OK,ERROR };

typedef struct DLnode//定义结构体 
{
	ElemType data;
	struct DLnode *prior;
	struct DLnode *next;
}DLnode,*DLinkList;

Status InitList(DLinkList &L)//初始化双向链表 
{
 	L=new DLnode;
 	L->next=NULL;
 	L->prior=NULL;
 	
 	return OK;
}

void PrintList(DLinkList L)//打印双向链表 
{
	DLnode *p=L->next;
	while(p!=L)
	{
 		cout << p->data <<" ";
 		p=p->next;
	}
}

Status CreateList(DLinkList &L,int &n)//创建双向链表 
{
	L=new DLnode;
	L->next=L;
	L->prior=L;
	DLnode *p=L;
	
	if(n<1)
	{
		return ERROR;
	}
	
	for(int i=0;i<n;i++)
	{
		cout<<"请输入第"<<i+1<<"个元素的值:";
		struct DLnode *s;
		s=new DLnode;
		cin >> s->data;
		p->next=s;
		s->next=L;
		s->prior=p;
		p=s;
	}
	
	return OK;
}

void Change(DLinkList &p)//交换节点 
{
	DLnode *q=p->prior;
	q->prior->next=p;
	p->prior=q->prior;
	q->next=p->next;
	q->prior=p;
	p->next->prior=q;
	p->next=q;
}

int main()
{
	DLinkList L;
	
	InitList(L);
	
	cout<<"请输入链表元素个数:";
	int n;
	cin>>n;
	
	CreateList(L,n);
	PrintList(L);
	cout<<endl;
	
	cout<<"请输入要交换的结点的值:";
	DLnode *s=new DLnode;
	cin>>s->data;
	
	DLnode *p=L->next;
	
	while(p)//遍历，找到要交换的节点 
	{
		if(p->data==s->data)
		{
			Change(p);
			
			break;
 		}
		else
		{
			p=p->next;
		}
	}
 	
	PrintList(L);
 	
 	return 0; 
} 

