// IOtest.cpp : �������̨Ӧ�ó������ڵ㡣
//

//#include "stdafx.h"

//ͷ�ļ�����
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

  
using namespace std; //�õ�����Ҫ��cin cout
 
#define  MAX_QUEUE_NUMBER    100                 //��������еĳ���
#define  MAX_QUEUE_LEN       (1024*1024*4)       //������ܵ��ڴ��С


//������еĿ��ƿ�ṹ��
typedef struct	BufferInfo  
{
	int QLen;
	int Front;
	int Rear;
	int OffSet;
	int IsUse;
} QUEUE_INFO;

//���п��ƿ���ţ���ӦΨһһ�����ζ���
typedef int  QHandle; 


//��Data�������ڴ�
QUEUE_INFO myQueueInfo[MAX_QUEUE_NUMBER];           //���ζ��п��ƿ�����
unsigned char   BYTEBufferMemory[MAX_QUEUE_LEN];    //���ζ����ڴ�
int  CurNotUseBYTEBufferAddr = 0;                   //δ�����ڴ���ָ��


//����ά���ӿ�
void    BuffersInit();                                          // ��ʼ�����п��ƿ���Ϣ
QHandle GetQueue(int DataLen);                                  // ����һ���µĻ�����Դ

int   GetUsedQueueLen(QHandle Queue);                           // ��ȡ�����е�ǰ��Ч���ݳ���
int   GetNoUsedQueueLen(QHandle Queue);                         // ��ȡû��ʹ�õ��ڴ泤��
int   GetQueueLen(QHandle Queue);                               // ��ȡ���ζ����ܳ���

int   ReleaseData(QHandle Queue, int Len);                      // �ͷŶ��е�ͷ������

int   GetDatas(QHandle Queue, unsigned char * Data, int Len);   // ��ȡ���ζ�������
unsigned char  GetData(QHandle Queue, int index);               // ��ȡ���ζ�������

int   InsertDataEx(QHandle Queue, unsigned char *Data, int Len);// ����һ�����ݵ����ζ���
int   InsertData(QHandle Queue, unsigned char Data);            // ����һ���ֽ����ݵ����ζ���

void  ClearQueueData(QHandle Queue);                            // ��ն����е�����


QHandle myQueue = -1;       //����һ��ȫ�ֶ��о��
void FramDataPro();         //����֡������
void FramDataProThread();   //�̺߳�����ר�Ŵ�������е�����
 


///////////////////////////////////////////////////
void  BuffersInit()    //���ζ��п��ƿ��ʼ��
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

QHandle GetQueue(int DataLen)   //���뻷�ζ����ڴ�
{
	int i;
	for (i = 0; i<MAX_QUEUE_NUMBER; i++)
	{
		if (myQueueInfo[i].IsUse == 0)  //�ҵ���λ
		{
			if ((CurNotUseBYTEBufferAddr + DataLen)<MAX_QUEUE_LEN) //������������
			{
				myQueueInfo[i].IsUse = 0xFFFF;     //��ʾ�Ѿ������ڴ�
				myQueueInfo[i].QLen = DataLen;     //��ʼ������ȥ�ڴ泤��
				myQueueInfo[i].Front = 0;
				myQueueInfo[i].Rear = 0;

				myQueueInfo[i].OffSet = CurNotUseBYTEBufferAddr;
				CurNotUseBYTEBufferAddr = CurNotUseBYTEBufferAddr + DataLen;
				 
				return i;
			}
			else //�����������ڿ�������
			{
				return -1;
			}
		}
	}

	return -1;
}

// ��ȡ��Ч���ݳ���
int GetUsedQueueLen(QHandle Queue)
{
	int Len = 0;
	 
	if (myQueueInfo[Queue].Rear >= myQueueInfo[Queue].Front) //βָ������ָ��֮��
	{
		Len = myQueueInfo[Queue].Rear - myQueueInfo[Queue].Front;
	}
	else //βָ������ָ��֮ǰ
	{
		Len = myQueueInfo[Queue].QLen - myQueueInfo[Queue].Front + myQueueInfo[Queue].Rear;
	} 

	return Len;
}

// ��ȡ�հ׳���
int GetNoUsedQueueLen(QHandle Queue)
{
	int Len = myQueueInfo[Queue].QLen - GetUsedQueueLen(Queue)-1; //�ܳ��ȼ�ȥ�������ݳ���

	return Len;
}

int   GetQueueLen(QHandle Queue) //��ȡ���ζ��г���
{
	return 	myQueueInfo[Queue].QLen;
}

// �ͷŵ�����           
int ReleaseData(QHandle Queue, int Len)
{
	int Re = GetUsedQueueLen(Queue);
	 
	if (Re >= Len) //�������ݴ����ͷ�����
	{
		myQueueInfo[Queue].Front = (myQueueInfo[Queue].Front + Len) % myQueueInfo[Queue].QLen;
		Re = Len;
	}
	else //��������С���ͷ�����
	{
		myQueueInfo[Queue].Front = (myQueueInfo[Queue].Front + Re) % myQueueInfo[Queue].QLen;
	}

	return Re;
}

// ��ȡ���ζ�������     
int GetDatas(QHandle Queue, unsigned char * Data, int Len)
{
	int Re = 0;
	int i;

	if (GetUsedQueueLen(Queue) >= Len) //���ζ������ݸ������ڶ�ȡ����
		Re = Len;
	else   //���ζ������ݸ���С�ڶ�ȡ����
		Re = GetUsedQueueLen(Queue);

	if (Re > 0) //��ȡ
	{
		for (i = 0; i < Re; i++)
			Data[i] = BYTEBufferMemory[myQueueInfo[Queue].OffSet + (myQueueInfo[Queue].Front + i) % myQueueInfo[Queue].QLen];
	}

	return Re;
}

// ��ȡ���ζ�������     
unsigned char GetData(QHandle Queue, int index)
{

	unsigned char Re = BYTEBufferMemory[myQueueInfo[Queue].OffSet + (myQueueInfo[Queue].Front + index) % myQueueInfo[Queue].QLen];

	return Re;
}

// ����һ�����ݵ����ζ��� 
int InsertDataEx(QHandle Queue, unsigned char *Data, int Len)
{
	int Re = Len;
	int i;

	if (Len > 0) //������Ч
	{

		if ( GetNoUsedQueueLen(Queue) >= Len ) //��������������Ҫ�����������
		{

			for (i = 0; i < Len; i++) //��������
				BYTEBufferMemory[myQueueInfo[Queue].OffSet + (myQueueInfo[Queue].Rear + i) % myQueueInfo[Queue].QLen] = Data[i];

			//����βָ��
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

// ����һ���ֽ����ݵ����ζ��� 
int InsertData(QHandle Queue, unsigned char Data)
{
	int Re = 1;

	if (GetNoUsedQueueLen(Queue) > 0) //���ڿ���λ��
	{
		Re = 1;

		//��������
		BYTEBufferMemory[myQueueInfo[Queue].OffSet + myQueueInfo[Queue].Rear%myQueueInfo[Queue].QLen] = Data;

		//����βָ��
		myQueueInfo[Queue].Rear = (myQueueInfo[Queue].Rear + 1) % myQueueInfo[Queue].QLen;

	}
	else
	{
		Re = -1;
	}


	return Re;
}

// ��ջ��ζ�������
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
//������У����������������ĵ����ȼ��߳��е�������һ����
//���Գ�����ÿ���ʱ�䣬��һ�������Ƶ���ж���������ݸ�������
void FramDataProThread()
{
	while (myQueue>=0)
	{ 
		FramDataPro();

		Sleep(1000); //�̴߳����������������ü��һ��ʱ��  
	}

	if (myQueue < 0)
	{
		cout << "������Ч���˳����ݴ����߳�" << endl;
	}
}


void FramDataPro()
{
	int DataLen = GetUsedQueueLen(myQueue);
	unsigned char tempData[256];

	if (DataLen>7)  //���ܴ���һ֡����
	{
		GetDatas(myQueue, tempData,6);

		if (tempData[0] == 0x1A && tempData[1] == 0xCF && tempData[2] == 0xFC && tempData[3] == 0x1D ) //֡ͷ��ȷ
		{
			cout << "���յ�֡ͷ��0x1FCFFC1D" 
				 << " ֡��ţ�"  << int(tempData[4]) 
				 << " ���ݳ��ȣ�" << int(tempData[5]) 
				 << " ��ǰ���ȣ�" << DataLen-6;

			int FramDataLen = tempData[5];
			if (FramDataLen > 0)
			{
				if (DataLen >= (FramDataLen+7))   //��������
				{
					cout << endl << "�������ѹ�������У��...";

					GetDatas(myQueue, tempData, 7 + FramDataLen); //��ȡʣ������

					//����У���
					unsigned char Check = 0x00;
					for (int i = 0; i < FramDataLen; i++)
					{
						Check = Check + tempData[6+i];
					}

					if (Check == tempData[6 + FramDataLen])  //У����ȷ
					{
						cout << endl  << "У����ȷ����Ч����Ϊ:" << endl;
						for (int i = 0; i < FramDataLen; i++)
						{
							cout << hex << +(int)tempData[6 + i] << " ";
						}
						cout << endl << "��֡���ݴ��������^_^" <<endl <<endl;

						ReleaseData(myQueue, 7 + FramDataLen);
					}
					else
					{
						cout << "У�����ʵ�ʼ�������" << hex << (int)Check 
							 << "  ֡�ڴ�Ž����0x" << hex << (int)tempData[6 + FramDataLen] 
							 << endl;

						ReleaseData(myQueue, 1);
					}
				}
				else
				{
					cout  << "  ���ڵȴ�����..." << endl;
				}
			}
			else
			{
				cout << "֡���ݳ��ȴ���" << endl;
				ReleaseData(myQueue, 1);
			}
		}
		else
		{
			cout << "֡ͷ����:" << (int)tempData[0] << (int)tempData[1] << (int)tempData[2] << (int)tempData[3] << endl;
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
	//֡ͷ
	RvData[0] = 0x1A;
	RvData[1] = 0xCF;
	RvData[2] = 0xFC;
	RvData[3] = 0x1D;

	//֡���
	RvData[4] = 0x01;

	//��Ч���ݳ���
	Len = 5;
	RvData[5] = 0x05;

	//��Ч����
	RvData[6] = 0x30;
	RvData[7] = 0x31;
	RvData[8] = 0x32;
	RvData[9] = 0x33;
	RvData[10] = 0x34;
	 
	//�ۼӺ�У��
	RvData[6 + Len] = 0x00;
	for (int i = 0; i < Len;i++)
	{
		RvData[6 + Len] = RvData[6 + Len] + RvData[6+i];
	}
	Len = Len + 7;



	/////////////////////////////////////////////////////////
	//��������û��ζ��н��н�֡��֤ 
	BuffersInit();                                          // ��ʼ�����п��ƿ���Ϣ
	myQueue =  GetQueue(1024);                      // ����һ���µĻ�����Դ
	if (myQueue < 0)
	{
		cout << "�������д��������ڴ��Ƿ��ã�" << endl;
	}
	else
	{ 
		///////////////////////////////////////////////////////
		//����֡���ݴ����߳�
		DWORD ThreadID;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FramDataProThread, NULL, 0, &ThreadID);



		///////////////////////////////////////////////////////
		//�����ݼ��뵽���ζ���,һ�����ⲿ�жϺ����н��в���
		for (int i = 0; i < Len; i++)
		{
			if (InsertData(myQueue,RvData[i]) < 1)
			{
				cout << "����������" << endl;
			}
			else
			{
				cout << "��������"  <<i+1 <<  ":  0x" << hex << int(RvData[i]) << endl;
			}

			Sleep(5000);
		}
	}
	 

	//������ͣ
	system("PAUSE");

	return 0; 
}


