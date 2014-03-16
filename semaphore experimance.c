/* Practice of Semaphore: Coordination of multi-tasking By using semaphore */

/*  ͷ�ļ� */
#include "vxWorks.h"
#include "stdio.h"
#include "stdlib.h"
#include "semLib.h"
#include "taskLiv.h"

/* �궨�� */
#define NUM_SAMPLE 10				/* ���һ�����������ݵĸ��� */
#define DELAY_TICKS 8				/* ģ��������ݵļ��ʱ�� */
#define STACK_SIZE 20000			/* �����ÿ������Ķ�ջ��С */

/* ���Ͷ��� */
typedef struct _LIST_NODE
{
	int data;		/* �ڵ����� */
	struct _LIST_NODE *pNextNode;		/* ָ����һ���ڵ��ָ�� */
}LIST_NOTE;

/* ȫ�ֱ��� */
int tidCosmos;						/* ����ID���� */
int tidSchlep;
int tidCrunch;
int tidMonitor;
int cosmicData = 0;					/* ģ����������� */
int result = 0;						/* ���������� */
LIST_NOTE *pCurrNode = NULL;		/* �����б��ͷָ�� */
SEM_ID dataSemId;					/* �ź���ID������ͬ�������ݿ���ʱ�ͷ� */
SEM_ID syncSemId;					/* �ź���ID������ͬ������������ʱ�ͷ� */
SEM_ID nodeListGuardSemId;			/* �ź���ID�����ڻ��⣬���������б� */

/* �������� */
void cosmos(void);
void nodeAdd(int data);
void schlep(void);
void nodeScrap(void);
void crunch(void);
void monitor(void);
void progStop(void);

/****************************************************************
* progStart - ����ʵ������
* ���𴴽��ź��������� 
* RETURNS��OK
*/
STATUS progStart(void)
{
	/* �����ź��� */
	syncSemId = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
	dataSemId = semBCreate(SEM_Q_FIFO, SEM_EMPTY); 
	nodeListGuardSemId = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE | SEM_DELETE_SAFE);

	/* ��ʼ������ */
	pCurrNode = NULL;

	/* �������� */
	tidCosmos = taskSpawn("tCosmos", 200, 0, STACK_SIZE,
							(FUNCPTR)cosmos, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);					
	tidSchlep = taskSpawn("tSchlep", 220, 0, STACK_SIZE,
							(FUNCPTR)schleo, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	tidCrunch = taskSpawn("tCrunch", 240, 0, STACK_SIZE,
							(FUNCPTR) crunch, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	tidMonitor = taskSpawn("tMonitor", 250, 0, STACK_SIZE,
							(FUNCPTR) monitor, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	return (OK);
}

/*******************************************************************************
* cosmos - ��������жϵķ������
* ���������ݽ���ISR���жη�����������Ĺ���
* ÿ��ʩ��һ���ź����������ݵ���һ��
* RETURNS�� N/A
*/
void cosmos(void)
{
	FOREVER
	{
		/* �������ݣ�
		 * ����һ��α�����������Χ = [0 - RAND_MAX]
		 */
		cosmicData = rand();
		/* ֪ͨ���ݽ������� */
		semGive(dataSemId);

		/* �ӳ٣���֤���������ݱ����������� */
		taskDelay(DELAY_TICKS);
	}

	return;
}

/*******************************************************************************
* nodeAdd - ���������ݴ洢����������
* ����һ�����ݽڵ��������������ݣ������ڵ�ǰ��������������
* RETURNS�� N/A
*/
void nodeAdd ( int data )   /* ���룺��Ҫ��������� */
{
	LIST_NODE *node;		/* ���ڴ����µ����ݽڵ� */

	if ((node = (LIST_NODE *) malloc(sizeof(LIST_NODE))) != NULL)
	{
		/* ������������ */
		node -> data = data;

		/* ���ڵ�ǰ�������������� */
		semTake(nodeListGuardSemId, WAIT_FOREVER);
		node -> pNextNode = pCurrNode;
		pCurrNode = node;
		semGive(nodeListGuardsemId);
	}
	else
	{
		printf("nodeAdd: Out of Memory.\n");
		taskSusPend(0);
	}

	return;
}


/*******************************************************************************
* schlep - ���ɼ������������һ������
* ���ɼ��������ݣ�ÿNUM_SAMPLE��һ�飬���һ��������������ȫ�ֵ���������洢
* ÿ���һ������֮�����ö������ź�����ͬ�����ܣ����ѣ�unpend��������������
* RETURNS�� N/A
*/
void schlep(void)
{
	int i;

	FOREVER
	{
		for(i = 0; i < NUM_SAMPLE; i++)
		{
			/* �ȴ����� */
			semTake(dataSemId, WAIT_FOREVER);

			/* �������� */
			nodeAdd(cosmocData);
		}

		/* ��������������tCrunch */
		semGive(syncSemId);
	}

	return;
}


/*******************************************************************************
* nodeScrap - ������������ɾ���׸����ݽڵ�
* �����������������ɴ�����������ɾ���ո����ݽڵ�Ĺ���
* RETURNS�� N/A
*/

void nodeScrap(void)
{
	LIST_NODE *pTmpNode;		/* ָ���׸����ݽڵ� */

	/* ��������������� */
	semTake(nodeListGuardSemId, WAIT_FOREVER);
	if (pCurrNode != NULL)
	{
		pTmpNote = pCurrNode;
		pCurrNode = pCurrNode -> pNextNode;
		free(pTmpNode);
	}

	/* ������� */
	semGive(nodeListGuardSemId);

	return;
}

/*******************************************************************************
* crunch - ������������
* ���ö������ź�����ͬ�����ܣ��ȴ�tSchlep����һ������
* ���յ����������ݽ�����ʹ���
* RETURNS�� N/A
*/
void crunch(void)
{
	int sampleSum;		/* ������������֮�� */

	FOREVER
	{
		/* �������� */
		sampleSum = 0;

		/* ͬ�����ȴ����� */
		semTake(syncSemId, WAIT_FOREVER);

		/* ��������������� */
		semTake(nodeListGuardSemId, WAIT_FOREVER);

		while (pCurrNode != NULL)
		{
			/* ������������� */
			sampleSum += pCurrNode -> data;

			/* �ݹ���ʣ�ɾ������ */
			nodeScrap();
		}

		/* ������� */
		semGive(nodeListGuardSemId);

		/* ���½�� */
		result = sampleSum;
	}
	return;
}


/*******************************************************************************
* monitor - ���ӳ����������
* ���ӳ����������������ʾ���
* ��result > averageʱ����ʾHOT����֮��ʾCOOL
* RETURNS�� N/A
*/
void monitor(void)
{
	BOOL isHot = FALSE;			/* ��־�� TRUE = �ȣ� FALSE = �� */
	BOOL average = 0;

	/* һ�������е���������ΪNUM_SAMPLE��ÿ�����ݵ�ȡֵ��ΧΪ[0 - RAND_MAX] */
	average = RAND_MAX * NUM_SAMPLE / 2

	FOREVER
	{
		if ((!isHot) && (result >= average))
		{
			isHot = TRUE;
			printf("HOT\n");
		}
		else if ((isHot) && (result < average))
		{
			isHot = FALSE;
			printf("COOL\n");
		}
	}
	return
}

/*******************************************************************************
* progStop - ֹͣʵ������
* ���ñ�������ֹͣ��ʵ������ɾ�������������ͷ��ź�����Դ
* RETURNS�� N/A
*/
void progStop(void)
{
	/* �����ź�����ɾ���������ܣ�����ɹ����֮���ٽ���ɾ������Ĺ���
	 * 
	 * nodeScrap()����Ҫʹ�û����ź�����������ɾ�������ź���֮ǰ����
	 */
	result = semTake(nodeListGuardSemId, WAIT_FOREVER);

	if (result == OK)
	{
		taskDelete(tidCosmos);
		taskDelete(tidSchlep);
		taskDelete(tidCrunch);
		taskDelete(tidMonitor);

		/* ����������� */
		while (pCurrNode != NULL)
		{
			nodeScrap();
		}
	}

	/* �ͷ��ź�����Դ */
	semDelete(dataSemId);
	semDelete(syncSemId);
	semDelete(nodeListGuardSemId);

	printf("BYE!\n");
	return;
}
