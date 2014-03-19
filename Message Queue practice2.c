/* ���ڿͻ���/�������ܹ��Ķ�������ͨѶ 
 * ��Ϊ������������tServer��NUM_CLIENT����Ϊ�ͻ�������tClient
 * ÿ������ӵ��һ�������Լ�����Ϣ����
 */

#include "vxWorks.h"
#include "msgQLib.h"

#define MAX_MSGS		(10)				/* ��Ϣ���г��� */
#define MAX_MSG_LEN		sizeof(MESSAGE)		/* ��Ϣ��������Ϣ����󳤶� */
#define STACK_SIZE		20000				/* �����ÿ������Ķ�ջ��С */
#define DELAY_TICKS		50					/* client������Ϣ�ļ��ʱ�� */
#define NUM_CLINET		2					/* �ͻ�����ĸ��� */

/* MESSAGE ID���壬����MESSAGE������mSendId��mRecvId�����ֶ� */
#define MID_CLIENT(id)	(id)				/* id = 0...��NUM_CLIENT - 1�� */
#define MID_SERVER		NUM_CLIENT			/* server��MESSAGE ID, ΪCLient������ID+1 */ 

/* ���Ͷ��� */
typedef struct _MESSAGE
{
	int mSendId;							/* ���������MESSAGE ID */
	int mRecvId;							/* ���������MESSAGE ID */
	int mData;								/* ��Ϣ�д��ݵ����� */
}MESSAGE��

/* ȫ�ֱ��� */
int tidServer;								/* ����ID���� */
int tidClient[NUM_CLIENT];					
MSG_Q_ID msgQIdServer;						/* ��Ϣ����ID���� */
MSG_Q_ID msgQIdClient[NUM_CLIENT];

/* �������� */
STATUS progStart(void);
STATUS server(void);
STATUS client(int id);
void progStop(void);

/**********************************************************************
* progStart - ����ʵ������ 
* ���𴴽���Ϣ�������������ж��ڲ�ͬ�Ŀͻ�����ͨ��id���������� 
* RETURNS�� OK��ERROR
*/
STATUS progStart(void);
{
	int id;			/* �������ֲ�ͬ�Ŀͻ����� */

	/* ������Ϣ���� */
	msgQIdServer = msgQCreate(MAX_MSGS, MAX_MSG_LEN, MSG_Q_PRIORITY);
	if (msgQIdServer == NULL)
	{
		return (ERROR);
	}

	for (id = 0; id < NUM_CLIENT; id++��
	{
		msgQIdClient[id] = msgQCreate(MAX_MSGS, MAX_MSG_LEN, MSG_Q_PRIRITY);
		if (msgQIdClient[id] == NULL)
		{
			return (ERROR);
		}
	}

	/* �������� */ 
	tidServer = taskSpawn("tServer", 220, 0, STACK_SIZE,
						(FUNCPTR)server, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	for (id = 0; id < NUM_CLIENT; id++)
	{
		char tempName[20];

		sprintf(tempName, "tClient%d", id);
		tidClient[id] = taskSpawn(tempName, 200, 0, STACK_SIZE,
						(FUNCPTR)client, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}

	return (OK);
}

/**********************************************************************
* server - �������Ը����ͻ��������Ϣ 
* ����Ϣ���н�����Ϣ�����򵥵ش�����ӦACK��Ӧ��
* RETURNS�� OK��ERROR
*/
STATUS server(void)
{
	MESSAGE rxMsg;��			/* ���ڴ���Ϣ�����н�����Ϣ */
	MESSAGE txMsg��			/* ��������Ϣ�����з�����Ϣ */

	/* ��ʼ��txMsg����дmSendId�ֶΣ�������Ϣ�ķ�������tServer */
	txMsg.mSendId = MID_SERVER;

	FOREVER
	{
		/* ����Ϣ���н�����Ϣ������Ϣ���п�ʱ������pend���ȴ� */
		if (msgQReceive(msgQIdServer, (char *)&rxMag, MAX_MSG_LEN, WAIT_FOREVER) == ERROR)
		{
			return (ERROR);
		}
	

		/* ������Ϣ */

		/* ���յ�����ϢӦ���Ƿ����Լ��� */
		if (rxMsg.mRecvId != MID_SERVER)
		{
			return (ERROR);
		}

		/* ���յ�����Ϣ����Ӧ����ĳ��tClient������ */
		if ((rxMsg.mSenId < MID_CLIENT(0)) || (rxMsg.mSendId > MID_CLIENT(NUM_CLIENT - 1)))
		{
			return (ERROR);
		}

		/* ��ʾ���յ����� */
		printf("\ntServer: receive a date %d from tClient%d\n", rxMsg.mData, = rxMsg.mSendId);

		/* ���ݽ��յ�����Ϣ���û�Ӧ��ϢͨѶtxMsg */
		txMsg.mRecvId = rxMsg.mSendId;
		txMsg.mData = rxMsg.mData;

		/* ���Ӧ����Ϣ���з���ACK����Ӧ����
		 * ����Ϣ������ʱ��������pend���ȴ�
		 */
		if (msgQSend(msgQIdClient[rxMsg.mSendId], (char*)&txMsg, sizeof(MESSAGE), WAIT_FOREVER, MSG_PRI_NORMAL) == ERROR)
		{
			return (ERROR);
		}
	}

	return (OK);
}

/**********************************************************************
* client - �����������������Ϣ 
* �����Ե��������ݣ������ݷ��͸�����������
* ���ȴ����������񽫸�������ΪACK�ش�����
* RETURNS�� OK��ERROR
*/
STATUS client (int id)		/* �������ֲ�ͬ�Ŀͻ����񣬴�����һ���ͻ������idȡΪ0����󴴽�������+1 */
{
	MESSAGE rxMsg;��			/* ���ڴ���Ϣ�����н�����Ϣ */
	MESSAGE txMsg��			/* ��������Ϣ�����з�����Ϣ */

	/* ��ʼ��txMsg����дmSendId��mRecvId�ֶ� */
	txMsg.mSendId = MID_CLIENT(id);
	txMsg.mRecvId = MID_SERVER;

	FOREVER
	{
		/* �õ�һ����������ݣ����ڷ��� */
		txMsg.mData = rand();

		/* ���Ӧ�ķ������������Ϣ���з�����Ϣ��
		 * ����Ϣ������ʱ��������pend���ȴ�
		 */
		if (msgQsend(msgQIdServer, (char*)&txMsg, sizeof(MESSAGE), 
					WAIT_FORVER, MSG_PRI_NORMAL) == ERROR)
		{
			return (ERROR);
		}

		/* ��ʾ���͵����� */
		printf("\tClient%d: transmit a data %d to tServer\m", id, txMsg.mData);

		/* ���ܷ����������ACK������timeoutΪ20 ticks */
		if (msgQReceive(msgQIdClient[id], (char*)&rxMsg, MAX_MSG_LEN, 20) == ERROR)
		{
			printf("\ntClient%d: wait ACK timeout!!!\n");
			continue;
		}

		/* ����Ƿ��յ���ȷ��ACK */
		printf("\ntClient%d: receive the ACK from tServer\n", id);
		if (txMsg.mData == rxMsg.mData)
		{
			printf(", and data check OK!\n");
		}
		else
		{
			printf("\n, but data check ERROR!\n");
			printf("tx data(%d) != ACK data(%d)\n",
					txMsg.mData, rxMsg.mData);
		}

		taskDelay(DELAY_TICKS + (txMsg.mData % DELAY_TICKS));
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
	int id;		/* �������ֲ�ͬ�Ŀͻ����� */

	for (id = 0; id < NUM_CLIENT; id++)
	{
		taskDelete(tidClient[id]);
	}
	taskDelete(tidServer);

	msgQDelete(msgQIdServer);
	for (id = 0; id < NUM_CLIENT; id++)
	{
		msgQDelete(msgQIdClient[id]);
	}

	printf("BYE!\n");

	return;
}

