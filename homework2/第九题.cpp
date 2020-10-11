#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

typedef int ElemType; 
enum Status { OK,ERROR };

typedef struct DLnode//����ṹ�� 
{
	ElemType data;
	struct DLnode *prior;
	struct DLnode *next;
}DLnode,*DLinkList;

Status InitList(DLinkList &L)//��ʼ��˫������ 
{
 	L=new DLnode;
 	L->next=NULL;
 	L->prior=NULL;
 	
 	return OK;
}

void PrintList(DLinkList L)//��ӡ˫������ 
{
	DLnode *p=L->next;
	while(p!=L)
	{
 		cout << p->data <<" ";
 		p=p->next;
	}
}

Status CreateList(DLinkList &L,int &n)//����˫������ 
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
		cout<<"�������"<<i+1<<"��Ԫ�ص�ֵ:";
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

void Change(DLinkList &p)//�����ڵ� 
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
	
	cout<<"����������Ԫ�ظ���:";
	int n;
	cin>>n;
	
	CreateList(L,n);
	PrintList(L);
	cout<<endl;
	
	cout<<"������Ҫ�����Ľ���ֵ:";
	DLnode *s=new DLnode;
	cin>>s->data;
	
	DLnode *p=L->next;
	
	while(p)//�������ҵ�Ҫ�����Ľڵ� 
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

