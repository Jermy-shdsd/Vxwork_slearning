#include "vxWorks.h"
#include "ioLib.h"
#include "pipeDrv.h"

#define MAX_MSGS	(10)		/*  �ܵ��������Ϣ���� */
#define MAX_MSG_LEN	(100)		/* ��Ϣ���� */
#define STACK_SIZE	20000		/* �����ÿ������Ķ�ջ��С */
#define DELAY_TICKS	50			/* ��ܵ�д����Ϣ�ļ��ʱ�� */

int tidTask1;
int tidTask2;
int demoPipeFd;			/* �ܵ��ļ������� */

STATUS progStart(void);
STATUS task1(void);
STATUS task2(void);
void progStop(void);

STATUS progStart(void)
{
	int result;			/* ��¼�������صĽ�� */

	/* ���𴴽�һ����Ϊ��/pipe/demo���Ĺܵ� */
	result = pipeDevCreate("/pipe/demo", MAX_MSGS, MAX_MSG_LEN);
	if (result == ERROR)
	{
		return (ERROR);
	}

	/* ���ܵ��򿪣������ܵ����ļ�������������demoPipeFd */
	demoPipeFd == open("/pipe/demo", O_RDWR, 0);
	if (demoPipeFd == ERROR)
	{
		return (ERROR);
	}

	tidTask1 = taskSpawn("tTask1", 200, 0, STACK_SIZE,
						(FUNCPTR)task1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	tidTask2 = taskSpawn("tTask2", 220, 0, STACK_SIZE,
						(FUNCPTR)task1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	return (OK);
}


/* ���巢����Ϣ������ */
#define MESSAGE "Greetings from Task 1"

/* task1: �����Ե���ܵ�д����Ϣ */
STATUS task1(void)
{
	FOREVER
	{
		/* ��ܵ�д��һ����Ϣ */
		write(demoPipeFd, MESSAGE, sizeof(MESSAGE));

		taskDelay(DELAY_TICKS);
	}

	return (OK);
}

/* task2: �ӹܵ�������Ϣ����ʾ */
STATUS task2(void)
{
	char msgBuf[MAX_MSG_LEN];		/* ���ڽ�����Ϣ */
	int len = 0;					/* ������յ���Ϣ�س��� */
	int result;						/* ���ڱ���select()�ķ��ؽ�� */
	fd_set readFd;					/* �ļ����������Σ����������ܵ��Ķ����� */

	FOREVER
	{
		/* ��ʼ������ */
		memset(msgBuf, 0, MAX_MSG_LEN);
		len = 0;

		/* ����һ������Ϣ�����ܵ���ʱ��������PEND����select()�ϵȴ� */
		while (len < sizeof(MESSAGE))
		{
			/* �����ܵ��Ķ����� */
			FD_ZERO(&readFd);
			FD_SET(demoPipeFd, &readFd);
			result = select(sizeof(fd_set), &readFd, NULL, NULL, NULL);

			if (result <= 0)
			{
				return (ERROR);
			}

			/* �ӹܵ��ж������� */
			len += read(demoPipeFd, msgBuf, sizeof(MESSAGE) - len);
		}

		printf("Message from task 1: &s\n", msgBuf);
	}

	return (OK);
}

void progStop(void)
{
	taskDelete(tidTask1);
	taskDelete(tidTask2);

	close(demoPipedFd);
	pipeDevDelete("pipe/demo", TRUE);

	printf("BYE!\n");
	return;
}



