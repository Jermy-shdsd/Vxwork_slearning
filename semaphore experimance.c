/* Practice of Semaphore: Coordination of multi-tasking By using semaphore */

/*  头文件 */
#include "vxWorks.h"
#include "stdio.h"
#include "stdlib.h"
#include "semLib.h"
#include "taskLiv.h"

/* 宏定义 */
#define NUM_SAMPLE 10				/* 组成一个样本的数据的个数 */
#define DELAY_TICKS 8				/* 模拟产生数据的间隔时间 */
#define STACK_SIZE 20000			/* 分配给每个任务的堆栈大小 */

/* 类型定义 */
typedef struct _LIST_NODE
{
	int data;		/* 节点数据 */
	struct _LIST_NODE *pNextNode;		/* 指向下一个节点的指针 */
}LIST_NOTE;

/* 全局变量 */
int tidCosmos;						/* 任务ID定义 */
int tidSchlep;
int tidCrunch;
int tidMonitor;
int cosmicData = 0;					/* 模拟产生的数据 */
int result = 0;						/* 样本处理结果 */
LIST_NOTE *pCurrNode = NULL;		/* 数据列表的头指针 */
SEM_ID dataSemId;					/* 信号量ID，用于同步，数据可用时释放 */
SEM_ID syncSemId;					/* 信号量ID，用于同步，样本可用时释放 */
SEM_ID nodeListGuardSemId;			/* 信号量ID，用于互斥，保护数据列表 */

/* 函数声明 */
void cosmos(void);
void nodeAdd(int data);
void schlep(void);
void nodeScrap(void);
void crunch(void);
void monitor(void);
void progStop(void);

/****************************************************************
* progStart - 启动实例程序
* 负责创建信号量与任务 
* RETURNS：OK
*/
STATUS progStart(void)
{
	/* 创建信号量 */
	syncSemId = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
	dataSemId = semBCreate(SEM_Q_FIFO, SEM_EMPTY); 
	nodeListGuardSemId = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE | SEM_DELETE_SAFE);

	/* 初始化变量 */
	pCurrNode = NULL;

	/* 创建任务 */
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
* cosmos - 仿真接受中断的服务程序
* 仿真了数据接受ISR（中段服务程序）所作的工作
* 每次施放一次信号量代表数据到来一次
* RETURNS： N/A
*/
void cosmos(void)
{
	FOREVER
	{
		/* 产生数据，
		 * 生成一个伪随机整数，范围 = [0 - RAND_MAX]
		 */
		cosmicData = rand();
		/* 通知数据接收任务 */
		semGive(dataSemId);

		/* 延迟，保证产生的数据被接受任务处理 */
		taskDelay(DELAY_TICKS);
	}

	return;
}

/*******************************************************************************
* nodeAdd - 将样本数据存储到数据链表
* 申请一个数据节点来保存样本数据，并将节点前出入数据链表中
* RETURNS： N/A
*/
void nodeAdd ( int data )   /* 输入：需要存入的数据 */
{
	LIST_NODE *node;		/* 用于创建新的数据节点 */

	if ((node = (LIST_NODE *) malloc(sizeof(LIST_NODE))) != NULL)
	{
		/* 保存样本数据 */
		node -> data = data;

		/* 将节点前插入数据链表中 */
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
* schlep - 将采集到的数据组成一个样本
* 将采集到的数据，每NUM_SAMPLE个一组，组成一个样本，样本用全局的数据链表存储
* 每组成一个样本之后，利用二进制信号量的同步功能，唤醒（unpend）样本处理任务
* RETURNS： N/A
*/
void schlep(void)
{
	int i;

	FOREVER
	{
		for(i = 0; i < NUM_SAMPLE; i++)
		{
			/* 等待数据 */
			semTake(dataSemId, WAIT_FOREVER);

			/* 保存数据 */
			nodeAdd(cosmocData);
		}

		/* 将单个样本发给tCrunch */
		semGive(syncSemId);
	}

	return;
}


/*******************************************************************************
* nodeScrap - 从数据链表中删除首个数据节点
* 互斥访问数据链表，完成从数据链表中删除收个数据节点的工作
* RETURNS： N/A
*/

void nodeScrap(void)
{
	LIST_NODE *pTmpNode;		/* 指向首个数据节点 */

	/* 互斥访问数据链表 */
	semTake(nodeListGuardSemId, WAIT_FOREVER);
	if (pCurrNode != NULL)
	{
		pTmpNote = pCurrNode;
		pCurrNode = pCurrNode -> pNextNode;
		free(pTmpNode);
	}

	/* 互斥结束 */
	semGive(nodeListGuardSemId);

	return;
}

/*******************************************************************************
* crunch - 处理样本数据
* 利用二进制信号量的同步功能，等待tSchlep发出一个样本
* 对收到的样本数据进行求和处理
* RETURNS： N/A
*/
void crunch(void)
{
	int sampleSum;		/* 保存样本数据之和 */

	FOREVER
	{
		/* 数据清零 */
		sampleSum = 0;

		/* 同步，等待样本 */
		semTake(syncSemId, WAIT_FOREVER);

		/* 互斥访问数据链表 */
		semTake(nodeListGuardSemId, WAIT_FOREVER);

		while (pCurrNode != NULL)
		{
			/* 对样本数据求和 */
			sampleSum += pCurrNode -> data;

			/* 递归访问，删除数据 */
			nodeScrap();
		}

		/* 互斥结束 */
		semGive(nodeListGuardSemId);

		/* 更新结果 */
		result = sampleSum;
	}
	return;
}


/*******************************************************************************
* monitor - 监视程序运行情况
* 监视程序运行情况，并显示结果
* 当result > average时，显示HOT，反之显示COOL
* RETURNS： N/A
*/
void monitor(void)
{
	BOOL isHot = FALSE;			/* 标志： TRUE = 热； FALSE = 冷 */
	BOOL average = 0;

	/* 一个样本中的数据综述为NUM_SAMPLE，每个数据的取值范围为[0 - RAND_MAX] */
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
* progStop - 停止实例程序
* 调用本函数来停止本实例程序，删除创建的任务并释放信号量资源
* RETURNS： N/A
*/
void progStop(void)
{
	/* 互斥信号量有删除保护功能，必须成功获得之后再进行删除任务的工作
	 * 
	 * nodeScrap()中需要使用互斥信号量，必须在删除互斥信号量之前调用
	 */
	result = semTake(nodeListGuardSemId, WAIT_FOREVER);

	if (result == OK)
	{
		taskDelete(tidCosmos);
		taskDelete(tidSchlep);
		taskDelete(tidCrunch);
		taskDelete(tidMonitor);

		/* 清空数据链表 */
		while (pCurrNode != NULL)
		{
			nodeScrap();
		}
	}

	/* 释放信号量资源 */
	semDelete(dataSemId);
	semDelete(syncSemId);
	semDelete(nodeListGuardSemId);

	printf("BYE!\n");
	return;
}
