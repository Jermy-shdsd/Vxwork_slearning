/* 基于客户端/服务器架构的多个任务间通讯 
 * 作为服务器的任务tServer与NUM_CLIENT个作为客户的任务tClient
 * 每个任务拥有一个属于自己的消息队列
 */

#include "vxWorks.h"
#include "msgQLib.h"

#define MAX_MSGS		(10)				/* 消息队列长度 */
#define MAX_MSG_LEN		sizeof(MESSAGE)		/* 消息队列中消息的最大长度 */
#define STACK_SIZE		20000				/* 分配给每个任务的堆栈大小 */
#define DELAY_TICKS		50					/* client发送消息的间隔时间 */
#define NUM_CLINET		2					/* 客户程序的个数 */

/* MESSAGE ID定义，用于MESSAGE类型中mSendId、mRecvId两个字段 */
#define MID_CLIENT(id)	(id)				/* id = 0...（NUM_CLIENT - 1） */
#define MID_SERVER		NUM_CLIENT			/* server的MESSAGE ID, 为CLient中最大的ID+1 */ 

/* 类型定义 */
typedef struct _MESSAGE
{
	int mSendId;							/* 发送任务的MESSAGE ID */
	int mRecvId;							/* 接受任务的MESSAGE ID */
	int mData;								/* 消息中传递的数据 */
}MESSAGE；

/* 全局变量 */
int tidServer;								/* 任务ID定义 */
int tidClient[NUM_CLIENT];					
MSG_Q_ID msgQIdServer;						/* 消息队列ID定义 */
MSG_Q_ID msgQIdClient[NUM_CLIENT];

/* 函数声明 */
STATUS progStart(void);
STATUS server(void);
STATUS client(int id);
void progStop(void);

/**********************************************************************
* progStart - 启动实例程序 
* 负责创建消息队列与任务，其中对于不同的客户任务，通过id参数来区别 
* RETURNS： OK，ERROR
*/
STATUS progStart(void);
{
	int id;			/* 用来区分不同的客户任务 */

	/* 创建消息队列 */
	msgQIdServer = msgQCreate(MAX_MSGS, MAX_MSG_LEN, MSG_Q_PRIORITY);
	if (msgQIdServer == NULL)
	{
		return (ERROR);
	}

	for (id = 0; id < NUM_CLIENT; id++）
	{
		msgQIdClient[id] = msgQCreate(MAX_MSGS, MAX_MSG_LEN, MSG_Q_PRIRITY);
		if (msgQIdClient[id] == NULL)
		{
			return (ERROR);
		}
	}

	/* 创建任务 */ 
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
* server - 处理来自各个客户任务的消息 
* 从消息队列接受信息，做简单地处理并回应ACK（应答）
* RETURNS： OK，ERROR
*/
STATUS server(void)
{
	MESSAGE rxMsg;；			/* 用于从消息队列中接受信息 */
	MESSAGE txMsg；			/* 用于向消息队列中发送信息 */

	/* 初始化txMsg，填写mSendId字段，表明消息的发送者是tServer */
	txMsg.mSendId = MID_SERVER;

	FOREVER
	{
		/* 从消息队列接受信息，当消息队列空时阻塞（pend）等待 */
		if (msgQReceive(msgQIdServer, (char *)&rxMag, MAX_MSG_LEN, WAIT_FOREVER) == ERROR)
		{
			return (ERROR);
		}
	

		/* 处理消息 */

		/* 接收到的消息应该是发给自己的 */
		if (rxMsg.mRecvId != MID_SERVER)
		{
			return (ERROR);
		}

		/* 接收到的消息配置应该是某个tClient发出的 */
		if ((rxMsg.mSenId < MID_CLIENT(0)) || (rxMsg.mSendId > MID_CLIENT(NUM_CLIENT - 1)))
		{
			return (ERROR);
		}

		/* 显示接收的数据 */
		printf("\ntServer: receive a date %d from tClient%d\n", rxMsg.mData, = rxMsg.mSendId);

		/* 根据接收到的消息配置回应消息通讯txMsg */
		txMsg.mRecvId = rxMsg.mSendId;
		txMsg.mData = rxMsg.mData;

		/* 向对应的消息队列发送ACK（回应），
		 * 当消息队列满时，阻塞（pend）等待
		 */
		if (msgQSend(msgQIdClient[rxMsg.mSendId], (char*)&txMsg, sizeof(MESSAGE), WAIT_FOREVER, MSG_PRI_NORMAL) == ERROR)
		{
			return (ERROR);
		}
	}

	return (OK);
}

/**********************************************************************
* client - 向服务器任务发请求消息 
* 周期性地生成数据，将数据发送给服务器任务，
* 并等待服务器任务将该任务作为ACK回传回来
* RETURNS： OK，ERROR
*/
STATUS client (int id)		/* 用来区分不同的客户任务，创建第一个客户任务的id取为0，其后创建的依此+1 */
{
	MESSAGE rxMsg;；			/* 用于从消息队列中接受信息 */
	MESSAGE txMsg；			/* 用于向消息队列中发送信息 */

	/* 初始化txMsg，填写mSendId与mRecvId字段 */
	txMsg.mSendId = MID_CLIENT(id);
	txMsg.mRecvId = MID_SERVER;

	FOREVER
	{
		/* 得到一个随机的数据，用于发送 */
		txMsg.mData = rand();

		/* 向对应的服务器任务的消息队列发送消息，
		 * 当消息队列满时，阻塞（pend）等待
		 */
		if (msgQsend(msgQIdServer, (char*)&txMsg, sizeof(MESSAGE), 
					WAIT_FORVER, MSG_PRI_NORMAL) == ERROR)
		{
			return (ERROR);
		}

		/* 显示发送的数据 */
		printf("\tClient%d: transmit a data %d to tServer\m", id, txMsg.mData);

		/* 接受服务器任务的ACK，设置timeout为20 ticks */
		if (msgQReceive(msgQIdClient[id], (char*)&rxMsg, MAX_MSG_LEN, 20) == ERROR)
		{
			printf("\ntClient%d: wait ACK timeout!!!\n");
			continue;
		}

		/* 检测是否收到正确的ACK */
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
* progStop - 停止实例程序 
* 调用本函数来停止本实例程序，删除创建的任务并释放消息队列资源 
* RETURNS： N/A
*/
void progStop(void)
{
	int id;		/* 用来区分不同的客户任务 */

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

