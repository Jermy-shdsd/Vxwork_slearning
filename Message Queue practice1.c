/* 两个任务间的单向信息通信 */ 
#include "vxWorks.h"
#include "msgQLib.h"

/* 宏定义 */
#define MAX_MSGS	(10)			/* 消息队列长度 */
#define MAX_MSG_LEN	(100)			/* 消息队列中消息的最大长度 */
#define STACK_SIZE	20000			/* 分配给每个任务的堆栈大小 */
#define DELAY_TICKS	50				/* 发送消息的间隔时间 */

/* 全局变量 */
int tidTask1;						/* 任务ID定义 */
int tidTask2;
MSG_Q_ID myMsgQId;					/* 消息队列ID定义 */

/* 函数声明 */
STATUS proStart(void);
STATUS task1(void);
STATUS task2(void);
void progStop(void);

/**********************************************************************
* progStart - 启动实例程序 
* 负责创建消息队列与任务 
* RETURNS： OK，ERROR
*/
STATUS progStart(void)
{
	/* 创建消息队列 */
	myMsQId = msgQCreate(MAX_MSGS, MAX_MSG_LEN, MSG_Q_PRIORITY);
	if (myMsgQid == NULL)
	{
		return (ERROR);
	}

	/* 创建任务 */
	tidTask1 = taskSpawn("tTask1", 200, 0, STACK_SIZE,
						(FUNCPTR)task1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	tidTask2 = taskSpawn("tTask2", 220, 0, STACK_SIZE,
						(FUNCPTR)task2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	return (OK);
}

/* 定义发送信息的内容 */
#define MESSAGE "Greetings from Task 1"

/****************************************************************************
* task1 - 周期性地向消息队列发送消息
* 用于周期性地向消息队列发送信息 
* 设置发送优先级为MSG_PRI_NORMAL，timeout参数为WAIT_FOREVER
* RETURN: OK
*/
STATUS task1(void)
{
	FOREVER
	{
		/* 向消息队列发送一条普通优先级的消息，
		 * 当消息队列满时，阻塞（pend）等待
		 */
		if (msgQSend(myMsgQId, MESSAGE, sizeof(MESSAGE), WAIT_FOREVER, MSG_PRI_NORMAL) == ERROR)
		{
			return (ERROR);
		}

		/* 延迟，以保证消息被tTask2处理 */
		taskDelay(DELAY_TICKS);
	}
	return (OK);
}

/****************************************************************************
* task2 - 接受消息并显示
* 从消息队列接受消息，当消息队列空时阻塞（Pend）等待，收到消息后显示 
* RETURN: OK
*/
STATUS task2(void)
{
	char msgBuf[MAX_MSG_LEN];

	FOREVER
	{
		/* 清空消息接受缓冲 */
		memset (msgBuf, 0, MAX_MSG_LEN);

		/* 从消息队列接受消息，当消息队列空时阻塞（Pend）等待 */
		if (msgQReceive(myMsgQId, msgBuf, MAX_MSG_LEN, WAIT_FOREVER) == ERROR)
		{
			return (ERROR);
		}

		/* 将接受的消息显示出来 */
		printf("Message from task 1: %s\n", msgBuf);
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
	/* 删除创建的任务 */
	taskDelete(tidTask1);
	taskDelete(tidTask2);

	/* 释放信号量资源 */
	msgQDelete(myMsgQId);

	printf("BYE!\n");

	return;
}