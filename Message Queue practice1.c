/* ���������ĵ�����Ϣͨ�� */ 
#include "vxWorks.h"
#include "msgQLib.h"

/* �궨�� */
#define MAX_MSGS	(10)			/* ��Ϣ���г��� */
#define MAX_MSG_LEN	(100)			/* ��Ϣ��������Ϣ����󳤶� */
#define STACK_SIZE	20000			/* �����ÿ������Ķ�ջ��С */
#define DELAY_TICKS	50				/* ������Ϣ�ļ��ʱ�� */

/* ȫ�ֱ��� */
int tidTask1;						/* ����ID���� */
int tidTask2;
MSG_Q_ID myMsgQId;					/* ��Ϣ����ID���� */

/* �������� */
STATUS proStart(void);
STATUS task1(void);
STATUS task2(void);
void progStop(void);

/**********************************************************************
* progStart - ����ʵ������ 
* ���𴴽���Ϣ���������� 
* RETURNS�� OK��ERROR
*/
STATUS progStart(void)
{
	/* ������Ϣ���� */
	myMsQId = msgQCreate(MAX_MSGS, MAX_MSG_LEN, MSG_Q_PRIORITY);
	if (myMsgQid == NULL)
	{
		return (ERROR);
	}

	/* �������� */
	tidTask1 = taskSpawn("tTask1", 200, 0, STACK_SIZE,
						(FUNCPTR)task1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	tidTask2 = taskSpawn("tTask2", 220, 0, STACK_SIZE,
						(FUNCPTR)task2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	return (OK);
}

/* ���巢����Ϣ������ */
#define MESSAGE "Greetings from Task 1"

/****************************************************************************
* task1 - �����Ե�����Ϣ���з�����Ϣ
* ���������Ե�����Ϣ���з�����Ϣ 
* ���÷������ȼ�ΪMSG_PRI_NORMAL��timeout����ΪWAIT_FOREVER
* RETURN: OK
*/
STATUS task1(void)
{
	FOREVER
	{
		/* ����Ϣ���з���һ����ͨ���ȼ�����Ϣ��
		 * ����Ϣ������ʱ��������pend���ȴ�
		 */
		if (msgQSend(myMsgQId, MESSAGE, sizeof(MESSAGE), WAIT_FOREVER, MSG_PRI_NORMAL) == ERROR)
		{
			return (ERROR);
		}

		/* �ӳ٣��Ա�֤��Ϣ��tTask2���� */
		taskDelay(DELAY_TICKS);
	}
	return (OK);
}

/****************************************************************************
* task2 - ������Ϣ����ʾ
* ����Ϣ���н�����Ϣ������Ϣ���п�ʱ������Pend���ȴ����յ���Ϣ����ʾ 
* RETURN: OK
*/
STATUS task2(void)
{
	char msgBuf[MAX_MSG_LEN];

	FOREVER
	{
		/* �����Ϣ���ܻ��� */
		memset (msgBuf, 0, MAX_MSG_LEN);

		/* ����Ϣ���н�����Ϣ������Ϣ���п�ʱ������Pend���ȴ� */
		if (msgQReceive(myMsgQId, msgBuf, MAX_MSG_LEN, WAIT_FOREVER) == ERROR)
		{
			return (ERROR);
		}

		/* �����ܵ���Ϣ��ʾ���� */
		printf("Message from task 1: %s\n", msgBuf);
	}

	return (OK);
}

/**********************************************************************
* progStop - ֹͣʵ������ 
* ���ñ�������ֹͣ��ʵ������ɾ�������������ͷ���Ϣ������Դ 
* RETURNS�� N/A
*/
void progStop(void)
{
	/* ɾ������������ */
	taskDelete(tidTask1);
	taskDelete(tidTask2);

	/* �ͷ��ź�����Դ */
	msgQDelete(myMsgQId);

	printf("BYE!\n");

	return;
}