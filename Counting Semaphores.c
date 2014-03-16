/* 计数信号量 Counting Semaphores */ 
SEM_ID semCCreate
	(
	int options,		/* 选项 */
	int initialCount	/* 初始计数 */
	）

/* exp */
countSem = semCCreate(SEM_Q_FIFO, 0 );

/* 计数信号量实现资源状况监视的例子 */
/* 头文件 */
#include "vxWorks.h"
#include "semLib.h"

/* 全局变量 */
SEM_ID countSem = NULL;		/* 信号量ID */

/*********************************************************
* init - 初始化程序
* 负责创建一个计数信号量
* RETURNS：OK， ERROR
* 信号量初始个数initialCount为0
* options选为先到先出方式 
*/
STATUS init()
{
	countSem = semCCreate(SEM_Q_FIFO, 0 );
	if (countSem == NULL)		/* 创建操作失败 */
	{
		return ERROR;
	}

	return OK;
}

/**************************************************************
* task1 - 生产资源的任务
* 生产完单位个数的资源后，调用semGive()记录资源的增加
*/
task1()
{
	...

	...				/* 生产单位个数的资源 */
	semGive(countSem);		/* 资源数量加1 */
	...
}

/**************************************************************
* task2 - 使用资源的任务
* 使用完单位个数的资源后，调用semTake()记录资源的减少
* 当技术信号量变为不可用时，阻塞（Pend）等待。
*/
task2()
{

	...
	semTake(countSem, WAIT_FOREVER);		/* 等待，直到有资源可用,资源数量减1 */ 


	...				/* 处理资源 */
}

