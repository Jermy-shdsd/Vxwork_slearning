#include <vxWorks.h>
#include <semLib.h>
#include <errnoLib.h>
#include <sigLib.h>

#define STACK_SIZE 20000			

SEM_ID semId;			/* 信号量ID，用于阻塞（pend）信号接收任务 */
int tidSigReceiver;		/* 任务ID */

STATUS sigReceiver(void);
STATUS txSignal(void);
void sigHandler(int sig);

STATUS progStart(void)
{
	/* 生成二进制信号量，初状态为不可用 */
	semId = semBCreate(SEM_Q_FIFO, SEM_EMPTY);

	/* 启动任务 */
	tidSigReceiver = taskSpawn("tSigReceiver", 200, 0, STACT_SIZE,
							(FUNCPTR)sigReceiver, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	printf("\nRun cmd \"txSignal() \" in shell to send a signal.\n";
	
	return (OK);
}

/*********************************************************************
* sigReceiver - 绑定信号处理程序与特定信号 
* 负责将信号处理程序与特定信号绑定，然后阻塞起来等待信号
*/
STATUS sigReceiver(void)
{
	/* 绑定信号处理程序与信号SIGUSR1  */
	signal(SIGUSR1, sigHandler);

	/* 将自己阻塞（pend）起来 */
	semTake(semId, WAIT_FOREVER);

	return (OK);
}

/*********************************************************************
* sigHandler - 信号处理程序
* 收到信号后打印一些简单的信息
*/

void sigHandler(int sig)			/* 任务收到的信号编码 */
{
	/* 本实例中只处理SIGUSR1信号 */
	if (sig != SIGUSR1)
	{
		logMsg("\nsigHandler: Invalid sig %d\n", sig, 0, 0, 0, 0, 0);
		return;
	}
	/* 打印出当前处理的信号编码 
	logMsg("\nsigHandler is hangling sig :%d!\n", sig, 0, 0, 0, 0, 0);