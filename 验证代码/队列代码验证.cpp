// IOtest.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"

//头文件包含
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

  
using namespace std; //用到的主要是cin cout
 
#define  MAX_QUEUE_NUMBER    100                 //可申请队列的长度
#define  MAX_QUEUE_LEN       (1024*1024*4)       //申请的总的内存大小


//管理队列的控制块结构体
typedef struct	BufferInfo  
{
	int QLen;
	int Front;
	int Rear;
	int OffSet;
	int IsUse;
} QUEUE_INFO;

//队列控制块序号，对应唯一一个环形队列
typedef int  QHandle; 


//在Data段申请内存
QUEUE_INFO myQueueInfo[MAX_QUEUE_NUMBER];           //环形队列控制块数组
unsigned char   BYTEBufferMemory[MAX_QUEUE_LEN];    //环形队列内存
int  CurNotUseBYTEBufferAddr = 0;                   //未分配内存首指针


//队列维护接口
void    BuffersInit();                                          // 初始化队列控制块信息
QHandle GetQueue(int DataLen);                                  // 申请一个新的缓存资源

int   GetUsedQueueLen(QHandle Queue);                           // 获取队列中当前有效数据长度
int   GetNoUsedQueueLen(QHandle Queue);                         // 获取没有使用的内存长度
int   GetQueueLen(QHandle Queue);                               // 获取环形队列总长度

int   ReleaseData(QHandle Queue, int Len);                      // 释放队列掉头部数据

int   GetDatas(QHandle Queue, unsigned char * Data, int Len);   // 获取环形队列数据
unsigned char  GetData(QHandle Queue, int index);               // 获取环形队列数据

int   InsertDataEx(QHandle Queue, unsigned char *Data, int Len);// 插入一段数据到环形队列
int   InsertData(QHandle Queue, unsigned char Data);            // 插入一个字节数据到环形队列

void  ClearQueueData(QHandle Queue);                            // 清空队列中的数据


QHandle myQueue = -1;       //定义一个全局队列句柄
void FramDataPro();         //数据帧处理函数
void FramDataProThread();   //线程函数，专门处理队列中的数据
 


///////////////////////////////////////////////////
void  BuffersInit()    //环形队列控制块初始化
{
	int i;
	for (i = 0; i<MAX_QUEUE_NUMBER; i++)
	{
		myQueueInfo[i].IsUse = 0;
		myQueueInfo[i].QLen = 0;
		myQueueInfo[i].Front = 0;
		myQueueInfo[i].Rear = 0;
		myQueueInfo[i].OffSet = 0;
	}
}

QHandle GetQueue(int DataLen)   //申请环形队列内存
{
	int i;
	for (i = 0; i<MAX_QUEUE_NUMBER; i++)
	{
		if (myQueueInfo[i].IsUse == 0)  //找到空位
		{
			if ((CurNotUseBYTEBufferAddr + DataLen)<MAX_QUEUE_LEN) //申请容量合适
			{
				myQueueInfo[i].IsUse = 0xFFFF;     //表示已经申请内存
				myQueueInfo[i].QLen = DataLen;     //初始化缓冲去内存长度
				myQueueInfo[i].Front = 0;
				myQueueInfo[i].Rear = 0;

				myQueueInfo[i].OffSet = CurNotUseBYTEBufferAddr;
				CurNotUseBYTEBufferAddr = CurNotUseBYTEBufferAddr + DataLen;
				 
				return i;
			}
			else //申请容量大于空闲容量
			{
				return -1;
			}
		}
	}

	return -1;
}

// 获取有效数据长度
int GetUsedQueueLen(QHandle Queue)
{
	int Len = 0;
	 
	if (myQueueInfo[Queue].Rear >= myQueueInfo[Queue].Front) //尾指针在首指针之后
	{
		Len = myQueueInfo[Queue].Rear - myQueueInfo[Queue].Front;
	}
	else //尾指针在首指针之前
	{
		Len = myQueueInfo[Queue].QLen - myQueueInfo[Queue].Front + myQueueInfo[Queue].Rear;
	} 

	return Len;
}

// 获取空白长度
int GetNoUsedQueueLen(QHandle Queue)
{
	int Len = myQueueInfo[Queue].QLen - GetUsedQueueLen(Queue)-1; //总长度减去已有数据长度

	return Len;
}

int   GetQueueLen(QHandle Queue) //获取环形队列长度
{
	return 	myQueueInfo[Queue].QLen;
}

// 释放掉数据           
int ReleaseData(QHandle Queue, int Len)
{
	int Re = GetUsedQueueLen(Queue);
	 
	if (Re >= Len) //已有数据大于释放数据
	{
		myQueueInfo[Queue].Front = (myQueueInfo[Queue].Front + Len) % myQueueInfo[Queue].QLen;
		Re = Len;
	}
	else //已有数据小于释放数据
	{
		myQueueInfo[Queue].Front = (myQueueInfo[Queue].Front + Re) % myQueueInfo[Queue].QLen;
	}

	return Re;
}

// 获取环形队列数据     
int GetDatas(QHandle Queue, unsigned char * Data, int Len)
{
	int Re = 0;
	int i;

	if (GetUsedQueueLen(Queue) >= Len) //环形队列数据个数大于读取个数
		Re = Len;
	else   //环形队列数据个数小于读取个数
		Re = GetUsedQueueLen(Queue);

	if (Re > 0) //读取
	{
		for (i = 0; i < Re; i++)
			Data[i] = BYTEBufferMemory[myQueueInfo[Queue].OffSet + (myQueueInfo[Queue].Front + i) % myQueueInfo[Queue].QLen];
	}

	return Re;
}

// 获取环形队列数据     
unsigned char GetData(QHandle Queue, int index)
{

	unsigned char Re = BYTEBufferMemory[myQueueInfo[Queue].OffSet + (myQueueInfo[Queue].Front + index) % myQueueInfo[Queue].QLen];

	return Re;
}

// 插入一段数据到环形队列 
int InsertDataEx(QHandle Queue, unsigned char *Data, int Len)
{
	int Re = Len;
	int i;

	if (Len > 0) //长度有效
	{

		if ( GetNoUsedQueueLen(Queue) >= Len ) //空闲数据量大于要插入的数据量
		{

			for (i = 0; i < Len; i++) //插入数据
				BYTEBufferMemory[myQueueInfo[Queue].OffSet + (myQueueInfo[Queue].Rear + i) % myQueueInfo[Queue].QLen] = Data[i];

			//更新尾指针
			myQueueInfo[Queue].Rear = (myQueueInfo[Queue].Rear + Len) % myQueueInfo[Queue].QLen;
			 
		}
		else
		{
			Re = -1;
		}
	}
	else
	{
		Re = -1;
	}

	return Re;
}

// 插入一个字节数据到环形队列 
int InsertData(QHandle Queue, unsigned char Data)
{
	int Re = 1;

	if (GetNoUsedQueueLen(Queue) > 0) //存在空闲位置
	{
		Re = 1;

		//插入数据
		BYTEBufferMemory[myQueueInfo[Queue].OffSet + myQueueInfo[Queue].Rear%myQueueInfo[Queue].QLen] = Data;

		//更新尾指针
		myQueueInfo[Queue].Rear = (myQueueInfo[Queue].Rear + 1) % myQueueInfo[Queue].QLen;

	}
	else
	{
		Re = -1;
	}


	return Re;
}

// 清空环形队列数据
void ClearQueueData(QHandle Queue)
{
	if ((Queue >= MAX_QUEUE_NUMBER) || (Queue<0))
	{

		return;
	}

	myQueueInfo[Queue].Front = 0;
	myQueueInfo[Queue].Rear = 0;
}
  

///////////////////////////////////////////////////////////////
//处理队列，多数情况下在另外的低优先级线程中单独处理，一方面
//可以充分利用空闲时间，另一方面避免频繁中断引起的数据覆盖问题
void FramDataProThread()
{
	while (myQueue>=0)
	{ 
		FramDataPro();

		Sleep(1000); //线程处理代码运行周期最好间隔一段时间  
	}

	if (myQueue < 0)
	{
		cout << "队列无效，退出数据处理线程" << endl;
	}
}


void FramDataPro()
{
	int DataLen = GetUsedQueueLen(myQueue);
	unsigned char tempData[256];

	if (DataLen>7)  //可能存在一帧数据
	{
		GetDatas(myQueue, tempData,6);

		if (tempData[0] == 0x1A && tempData[1] == 0xCF && tempData[2] == 0xFC && tempData[3] == 0x1D ) //帧头正确
		{
			cout << "接收到帧头：0x1FCFFC1D" 
				 << " 帧序号："  << int(tempData[4]) 
				 << " 数据长度：" << int(tempData[5]) 
				 << " 当前长度：" << DataLen-6;

			int FramDataLen = tempData[5];
			if (FramDataLen > 0)
			{
				if (DataLen >= (FramDataLen+7))   //数据量够
				{
					cout << endl << "数据量已够，计算校验...";

					GetDatas(myQueue, tempData, 7 + FramDataLen); //获取剩余数据

					//计算校验和
					unsigned char Check = 0x00;
					for (int i = 0; i < FramDataLen; i++)
					{
						Check = Check + tempData[6+i];
					}

					if (Check == tempData[6 + FramDataLen])  //校验正确
					{
						cout << endl  << "校验正确，有效数据为:" << endl;
						for (int i = 0; i < FramDataLen; i++)
						{
							cout << hex << +(int)tempData[6 + i] << " ";
						}
						cout << endl << "本帧数据处理结束！^_^" <<endl <<endl;

						ReleaseData(myQueue, 7 + FramDataLen);
					}
					else
					{
						cout << "校验错误，实际计算结果：" << hex << (int)Check 
							 << "  帧内存放结果：0x" << hex << (int)tempData[6 + FramDataLen] 
							 << endl;

						ReleaseData(myQueue, 1);
					}
				}
				else
				{
					cout  << "  正在等待数据..." << endl;
				}
			}
			else
			{
				cout << "帧数据长度错误" << endl;
				ReleaseData(myQueue, 1);
			}
		}
		else
		{
			cout << "帧头错误:" << (int)tempData[0] << (int)tempData[1] << (int)tempData[2] << (int)tempData[3] << endl;
			ReleaseData(myQueue, 1);
		}
	}
}

///////////////////////////////////////////////////////////////////////
int main()
{  
	///////////////////////////////////////////////////////
	unsigned char RvData[256];
	int Len = 0;
	//帧头
	RvData[0] = 0x1A;
	RvData[1] = 0xCF;
	RvData[2] = 0xFC;
	RvData[3] = 0x1D;

	//帧序号
	RvData[4] = 0x01;

	//有效数据长度
	Len = 5;
	RvData[5] = 0x05;

	//有效数据
	RvData[6] = 0x30;
	RvData[7] = 0x31;
	RvData[8] = 0x32;
	RvData[9] = 0x33;
	RvData[10] = 0x34;
	 
	//累加和校验
	RvData[6 + Len] = 0x00;
	for (int i = 0; i < Len;i++)
	{
		RvData[6 + Len] = RvData[6 + Len] + RvData[6+i];
	}
	Len = Len + 7;



	/////////////////////////////////////////////////////////
	//下面对利用环形队列进行接帧验证 
	BuffersInit();                                          // 初始化队列控制块信息
	myQueue =  GetQueue(1024);                      // 申请一个新的缓存资源
	if (myQueue < 0)
	{
		cout << "创建队列错误！请检查内存是否够用！" << endl;
	}
	else
	{ 
		///////////////////////////////////////////////////////
		//启动帧数据处理线程
		DWORD ThreadID;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FramDataProThread, NULL, 0, &ThreadID);



		///////////////////////////////////////////////////////
		//将数据加入到环形队列,一般在外部中断函数中进行操作
		for (int i = 0; i < Len; i++)
		{
			if (InsertData(myQueue,RvData[i]) < 1)
			{
				cout << "队列已满！" << endl;
			}
			else
			{
				cout << "插入数据"  <<i+1 <<  ":  0x" << hex << int(RvData[i]) << endl;
			}

			Sleep(5000);
		}
	}
	 

	//程序暂停
	system("PAUSE");

	return 0; 
}


