/**********************************************************************
* 二进制信号量具有两种状态（Flag）：信号量满(SEM_FULL)时，处于可用状态；信号量空(SEM_EMPTY)时，处于不可用；
* 由可用至不可用——semTake； 由不可用至可用——semGive。
*/
SEM_ID semBCreate
	(
	int optiions,		/* 选项：基于任务的优先级SEM_Q_PRIORITY；先进先出SEM_Q_FIFO */
	SEM_B_STATE initialState /* 初始状态 */
	）

/* 创建二进制信号量，FIFO排队方式，初始状态为满 */ 
semID = semBCreate(SEM_Q_FIFO, SEM_FULL)
semID = semBCreate(SEM_Q_FIFO | SEM_EVENTSEND_ERR_NOTIFY, SEM_FULL)		/* 与事件相关的取值，semGive返回ERROR */

STATUS semDelete ( SEM_ID semID ）		/* 需要删除的喜好两ID */ 


if (semTake(semID, NO_WAIT) == OK)
{
	semDelete(semID);
}

/* 二进制信号量的获取与释放 */
STATUS semTake
	(
	SEM_ID semId,		/* 需要获取的信号量ID */
	int timeout			/* 超时时间（tick）  */
	)

STATUS semGive
	(
	SEM_ID semId		/* 需要释放的信号量ID */
	)

/* semFlush()：一个任务通过调用此函数唤醒阻塞在某二进制信号量上的所有任务 */
STATUS semFlush
	(
	SEM_ID semId		/* 信号量ID，需要唤醒(Unpend)阻塞在其上的任务 */
	）