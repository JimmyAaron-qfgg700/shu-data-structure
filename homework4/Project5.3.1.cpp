#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

//////////////////////////////////////////////////////
typedef char TElemType;

typedef struct BiTNode//定义二叉链表
{
	TElemType data;
	BiTNode *lchild, *rchild;//左右孩子
}BiTNode,*BiTree;

void CreateBiTree(BiTree &T)//先序遍历创建二叉树
{
	char ch;
	cin >> ch;

	if (ch == '#') T = NULL;//空树
	else
	{
		T = new BiTNode;//新节点
		T->data = ch;
		CreateBiTree(T->lchild);//创建新的左子树
		CreateBiTree(T->rchild);//创建新的右子树
	}
}

void PreOrderTraverse(BiTree T)//先序遍历输出二叉树
{
	if (T)//树不为空
	{
		cout << T->data;//输出节点
		PreOrderTraverse(T->lchild);//遍历左子树
		PreOrderTraverse(T->rchild);//遍历右子树
	}
	else
	{
		cout << '#';
	}
}

int LeafCount(BiTree T)//统计叶子节点数
{
	if (!T) return 0;//空树

	if (!T->lchild && !T->rchild)//左右子树都没有，即为叶子节点
	{
		 return 1;
	}

	return LeafCount(T->lchild) + LeafCount(T->rchild);//左子树的叶子结点个数+右子树的叶子结点个数
}

void exchange(BiTree &T)//交换左右孩子
{
	if (T)//非空
	{
		if (T->lchild!=NULL && T->rchild!=NULL)//左右孩子都有
		{
			BiTNode *t = T->lchild;
			T->lchild = T->rchild;
			T->rchild = t;
		}

		if (T->lchild==NULL && T->rchild!=NULL)//只有右孩子
		{
			T->lchild = T->rchild;
			T->rchild = NULL;
		}

		if (T->rchild==NULL && T->lchild!=NULL)//只有左孩子
		{
			T->rchild = T->lchild;
			T->lchild = NULL;
		}

		exchange(T->lchild);//递归遍历左右子树
		exchange(T->rchild);
	}
}
/////////////////////////////////////////
void one()//第一题
{
	BiTree T;
	CreateBiTree(T);

	cout << LeafCount(T);
}

void three()//第三题
{
	BiTree T;
	CreateBiTree(T);
	PreOrderTraverse(T);//交换前

	cout << endl;

	exchange(T);
	PreOrderTraverse(T);//交换后
}

int main()
{
	int number;
	cout << "题号1或3：";
	cin >> number;

	switch (number)
	{
	case 1:
		one();
		break;
	case 3:
		three();
		break;
	}

	return 0;
}