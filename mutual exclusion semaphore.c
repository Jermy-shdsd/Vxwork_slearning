/* 创建互斥信号量 */
SEM_ID semMCreate
	(
	int options /* 选项 */
	)


/* 创建一个具有优先级继承（Priority Inheritance）的互斥信号量 */
semId = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE)

/* exp for 递归访问 */

/* 头文件 */
#include "vxWorks.h"
#include "semLib.h"

/* 全局变量 */
SEM_ID mutualSem = NULL;  /* 信号量ID */

/**************************************************************************
* init - 初始化程序
* 负责创建一个互斥信号量 
* RETURNS：OK，ERROR
* 本例options设为优先级方式 
*/
STATUS init(void)
{
	mutualSem = semMCreate(SEM_Q_PRIORITY);
	if (mutualSem == NULL)		/* 创建操作失败 */
	{
		return ERROR;
	}

	return OK;
}

/*****************************************************************************
* funcB - 需要访问临界资源的函数
* 成对地调用semTake()、semGive()访问临界资源
*/
funcB()
{
	semTake(mutualSem, WAIT_FOREVER);
	printf("funcB: GOt mutual-exclusion semaphore\n");
	...
	semGive(mutualSem);
	printf("funcB: Releases mutual-exclusion semaphore\n");
}

