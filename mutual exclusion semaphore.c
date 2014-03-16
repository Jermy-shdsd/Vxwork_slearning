/* ���������ź��� */
SEM_ID semMCreate
	(
	int options /* ѡ�� */
	)


/* ����һ���������ȼ��̳У�Priority Inheritance���Ļ����ź��� */
semId = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE)

/* exp for �ݹ���� */

/* ͷ�ļ� */
#include "vxWorks.h"
#include "semLib.h"

/* ȫ�ֱ��� */
SEM_ID mutualSem = NULL;  /* �ź���ID */

/**************************************************************************
* init - ��ʼ������
* ���𴴽�һ�������ź��� 
* RETURNS��OK��ERROR
* ����options��Ϊ���ȼ���ʽ 
*/
STATUS init(void)
{
	mutualSem = semMCreate(SEM_Q_PRIORITY);
	if (mutualSem == NULL)		/* ��������ʧ�� */
	{
		return ERROR;
	}

	return OK;
}

/*****************************************************************************
* funcB - ��Ҫ�����ٽ���Դ�ĺ���
* �ɶԵص���semTake()��semGive()�����ٽ���Դ
*/
funcB()
{
	semTake(mutualSem, WAIT_FOREVER);
	printf("funcB: GOt mutual-exclusion semaphore\n");
	...
	semGive(mutualSem);
	printf("funcB: Releases mutual-exclusion semaphore\n");
}

