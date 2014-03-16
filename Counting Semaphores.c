/* �����ź��� Counting Semaphores */ 
SEM_ID semCCreate
	(
	int options,		/* ѡ�� */
	int initialCount	/* ��ʼ���� */
	��

/* exp */
countSem = semCCreate(SEM_Q_FIFO, 0 );

/* �����ź���ʵ����Դ״�����ӵ����� */
/* ͷ�ļ� */
#include "vxWorks.h"
#include "semLib.h"

/* ȫ�ֱ��� */
SEM_ID countSem = NULL;		/* �ź���ID */

/*********************************************************
* init - ��ʼ������
* ���𴴽�һ�������ź���
* RETURNS��OK�� ERROR
* �ź�����ʼ����initialCountΪ0
* optionsѡΪ�ȵ��ȳ���ʽ 
*/
STATUS init()
{
	countSem = semCCreate(SEM_Q_FIFO, 0 );
	if (countSem == NULL)		/* ��������ʧ�� */
	{
		return ERROR;
	}

	return OK;
}

/**************************************************************
* task1 - ������Դ������
* �����굥λ��������Դ�󣬵���semGive()��¼��Դ������
*/
task1()
{
	...

	...				/* ������λ��������Դ */
	semGive(countSem);		/* ��Դ������1 */
	...
}

/**************************************************************
* task2 - ʹ����Դ������
* ʹ���굥λ��������Դ�󣬵���semTake()��¼��Դ�ļ���
* �������ź�����Ϊ������ʱ��������Pend���ȴ���
*/
task2()
{

	...
	semTake(countSem, WAIT_FOREVER);		/* �ȴ���ֱ������Դ����,��Դ������1 */ 


	...				/* ������Դ */
}

