/* ͷ�ļ� */ 
#include "vxworks.h"
#include "semLib.h"

/* ȫ�ֱ��� */
SEM_ID semMutex = NULL;			/* �ź���ID */ 


//���ӣ�ʹ�ö������ź����������񻥳�
/***********************************************************
*
* init - ��ʼ������
*
* ���𴴽�һ���������ź������ź�����ʼΪ��״̬
*
* RETURNS�� OK�� ERROR
*
* ��������optionsΪ���ȼ���ʽ
*/
STATUS init(void)
{
	semMutex = semBCreate(SEM_Q_PRIORITY, SEM_FULL);

	if (semMutex == NULL)			/* ��������ʧ�� */
	{
		return ERROR;
	}

	return OK;
}

/***************************************************************
* 
* funcA - ��Ҫ�����ٽ���Դ�ĺ���
*
* �ɶԵص���semTake()��semGive()�����ٽ���Դ
*
* RETURNS: OK, ERROR
*/
STATUS funcA(void)
{
	...			/* ���� */ 
	if ��semTake(semMutex, WAIT_FOREVER) == OK)
	{
		... /* ���ʱ�semMutex�����������ٽ���Դ */
	}
	else
	{
		return ERROR;
	}
	semGive(semMutex);
	...

	return OK;
}


//���ӣ�ʹ�ö������ź�����������ͬ����task1���ȼ�����task2����Ҫtask1���е�ĳһ��֮�������task2���С�
/* ͷ�ļ� */ 
#include "vxworks.h"
#include "semLib.h"

/* ȫ�ֱ��� */
SEM_ID syncSem = NULL; /* �ź���ID */ 


/***************************************************************
*
* init - ��ʼ������
*
* ���𴴽�һ���������ź������ź�����ʼΪ��״̬
*
*RETURNS�� OK�� ERROR
*
* optionsΪFIFO
*/
STATUS init ()
{
	syncSem = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
	if (syncSem == NULL)
	{
		return ERROR;
	}

	return OK;
}

/*****************************************************************
*
* task1 - һ���ϵ����ȼ�������
*
* �����Ҫͬ���Ĺ���֮��ִ��semGive����
*/
task1()
{
	...				/* ��Ҫ��ִ�еĹ��� */

	semGive(syncSem);		/* �ͷ�ͬ���ź�����ѭ��task2����ִ�� */

	...
}

/*****************************************************************
*
* task2 - һ���ϸ����ȼ�������
*
* �����Ҫͬ���Ĺ���֮��ִ��semTake����
*/
task2()
{
	semTake(syncSem, WAIT_FOREVER);			/* �ȴ���֪��task1��ɱ�Ҫ���� */ 
	...
}	