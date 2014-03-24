#include <vxWorks.h>
#include <semLib.h>
#include <errnoLib.h>
#include <sigLib.h>

#define STACK_SIZE 20000			

SEM_ID semId;			/* �ź���ID������������pend���źŽ������� */
int tidSigReceiver;		/* ����ID */

STATUS sigReceiver(void);
STATUS txSignal(void);
void sigHandler(int sig);

STATUS progStart(void)
{
	/* ���ɶ������ź�������״̬Ϊ������ */
	semId = semBCreate(SEM_Q_FIFO, SEM_EMPTY);

	/* �������� */
	tidSigReceiver = taskSpawn("tSigReceiver", 200, 0, STACT_SIZE,
							(FUNCPTR)sigReceiver, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	printf("\nRun cmd \"txSignal() \" in shell to send a signal.\n";
	
	return (OK);
}

/*********************************************************************
* sigReceiver - ���źŴ���������ض��ź� 
* �����źŴ���������ض��źŰ󶨣�Ȼ�����������ȴ��ź�
*/
STATUS sigReceiver(void)
{
	/* ���źŴ���������ź�SIGUSR1  */
	signal(SIGUSR1, sigHandler);

	/* ���Լ�������pend������ */
	semTake(semId, WAIT_FOREVER);

	return (OK);
}

/*********************************************************************
* sigHandler - �źŴ������
* �յ��źź��ӡһЩ�򵥵���Ϣ
*/

void sigHandler(int sig)			/* �����յ����źű��� */
{
	/* ��ʵ����ֻ����SIGUSR1�ź� */
	if (sig != SIGUSR1)
	{
		logMsg("\nsigHandler: Invalid sig %d\n", sig, 0, 0, 0, 0, 0);
		return;
	}
	/* ��ӡ����ǰ������źű��� 
	logMsg("\nsigHandler is hangling sig :%d!\n", sig, 0, 0, 0, 0, 0);