/* 头文件 */ 
#include "vxworks.h"
#include "semLib.h"

/* 全局变量 */
SEM_ID semMutex = NULL;			/* 信号量ID */ 


//例子：使用二进制信号量进行任务互斥
/***********************************************************
*
* init - 初始化程序
*
* 负责创建一个二进制信号量，信号量初始为满状态
*
* RETURNS： OK， ERROR
*
* 本例设置options为优先级方式
*/
STATUS init(void)
{
	semMutex = semBCreate(SEM_Q_PRIORITY, SEM_FULL);

	if (semMutex == NULL)			/* 创建操作失败 */
	{
		return ERROR;
	}

	return OK;
}

/***************************************************************
* 
* funcA - 需要访问临界资源的函数
*
* 成对地调用semTake()、semGive()访问临界资源
*
* RETURNS: OK, ERROR
*/
STATUS funcA(void)
{
	...			/* 函数 */ 
	if （semTake(semMutex, WAIT_FOREVER) == OK)
	{
		... /* 访问被semMutex保护起来的临界资源 */
	}
	else
	{
		return ERROR;
	}
	semGive(semMutex);
	...

	return OK;
}


//例子：使用二进制信号量进行任务同步，task1优先级低于task2，需要task1运行到某一点之后才允许task2运行。
/* 头文件 */ 
#include "vxworks.h"
#include "semLib.h"

/* 全局变量 */
SEM_ID syncSem = NULL; /* 信号量ID */ 


/***************************************************************
*
* init - 初始化程序
*
* 负责创建一个二进制信号量，信号量初始为控状态
*
*RETURNS： OK， ERROR
*
* options为FIFO
*/
STATUS init ()
{
	syncSem = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
	if (syncSem == NULL)
	{
		return ERROR;
	}

	return OK;
}

/*****************************************************************
*
* task1 - 一个较低优先级的任务
*
* 完成需要同步的工作之后，执行semGive（）
*/
task1()
{
	...				/* 需要先执行的工作 */

	semGive(syncSem);		/* 释放同步信号量，循序task2继续执行 */

	...
}

/*****************************************************************
*
* task2 - 一个较高优先级的任务
*
* 完成需要同步的工作之后，执行semTake（）
*/
task2()
{
	semTake(syncSem, WAIT_FOREVER);			/* 等待，知道task1完成必要工作 */ 
	...
}	