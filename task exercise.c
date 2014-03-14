/*�����Ϊ���н������������ѭ������
/*void Run_to_end_Task(void)
{
	Step 1: Initialize application
	Step 2: Create endless loop tasks
	Step 3: Create kernel objects
	Step 4: Delete or suspend this task
}
 ����һ��ʹ��return��ʹ�������ؾͿ���ɾ�����õ�����һ�����������ܹ��õ�cpu */

/*void ContinuousLoopTask(void)
{
	Initialization code

	Loop Forever
	{
		main loop
		make one or more blocking calls
	}
} 
��ѭ�������У�������Ҫһ���������ʹ������ʱ����CPU��ֻ���������п���ʹ�����ȼ��������ܹ��õ�CPU��
Ȼ��ʼִ�У��ź�����ʹ�þ������������� */

/* ������Ӧ������ 
�����񴩼���������ģ��һ������ͬ���ռ�ϵͳ�� 
Send Task ÿ�����ͷ�һ���ź���dataSemId������ģ���յ����ݣ�
Receive Task �����ź���datdaSemId��Ȼ������Ӧ�Ĵ��� */

/* ͷ�ļ� */ 
#include "vxworks.h"
#include "stdio.h"
#include "stdlib.h"
#include "taskLib.h"
#include "sysLib.h"

/* �궨�� */
#define STACK_SIZE 2000			/* �����ջ��СΪ2000bytes */

/* ȫ�ֱ��� */
SEM_ID dataSemId;				/* ͬ���ź��� */
int tidSend;					/* Send Task����id */
int tidReceive;					/* Receive Task����id */

/* �������� */
void progStart(void);
void taskSend(void);
void raskReceive(void);
void SendInit(void);
void ReceiveInit(void);
void progStop(void);

/**********************************************************************************************
* progStart - start ����
* ���𴴽�Send Task��Recieve Task������ʼ����
* RETURNS�� N/A
*/
void progStart(void)
{
	/* �����ź��� */
	dataSemId = semBCreate(SEM_QFIFO, SEM_EMPTY);

	/* ��������*/
	tidSend = taskSpawn ("tSend", 200, 0 ,STACK_SIZE,
							(FUNCPTER)taskSend,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	tidReceive = taskSpawn ("tReceive", 220, 0 ,STACK_SIZE,
							(FUNCPTER)taskReceive,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	return;
}

/**********************************************************************************
* taskSend - ������������
* ÿ�������ͷ�һ���ź���dataSemId������ģ���յ�������
*/
void taskSend(void)
{
	/* Send Task�ĳ�ʼ�� */
	SendInit������

	/* ��ѭ�� */
	while(1)
	{
		/* ������ʱ���� */
		taskDelay(sysClkRateGet() * 2);
		/* �ͷ��ź���������ʾ�յ����� */
		semGive(dataSemId);
	}

	return;
}

/**********************************************************************************
* taskReceive - ������������
* �ȴ��ź��� dataSemId�������ж��Ƿ��յ����� 
*/
void taskReceive(void)
{
	/* Receive Task�ĳ�ʼ�� */
	ReceiveInit();

	/* ��ѭ�� */
	while(1)
	{
		/* �ȴ��ź����������ݣ� */
		semTake(dataSemId, WAIT_FOREVER);
		printf("\nReceive a data");
	}

	return;
}

/************************************************************************************
* SendInit - ��ʼ�� Send Task
* ��ʼ��Send Task
*/
void SendInit(void)
{
	/* ��ʼ������ */
	printf("\nInitial Send Task");
	return;
}

/************************************************************************************
* ReceiveInit - ��ʼ�� Receive Task
* ��ʼ��Receive Task
*/
void ReceiveInit(void)
{
	/* ��ʼ������ */
	printf("\nInitial Receive Task");
	return;
}

/************************************************************************************
* progStop - ��ֹ��������
* ɾ���ź�����ɾ������������ֹ�������С�
*/
void progStop(void)
{
	/* ɾ���ź��� */
	semDelete(dataSemId);

	/* ɾ�������ź��� */
	taskDelete(tidSend);
	taskDelete(tidReceive);

	printf("The End\n");

	return;
}