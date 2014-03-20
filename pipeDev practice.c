#include "vxWorks.h"
#include "ioLib.h"
#include "pipeDrv.h"

#define MAX_MSGS	(10)		/*  管道中最大消息个数 */
#define MAX_MSG_LEN	(100)		/* 消息长度 */
#define STACK_SIZE	20000		/* 分配给每个任务的堆栈大小 */
#define DELAY_TICKS	50			/* 向管道写入消息的间隔时间 */

int tidTask1;
int tidTask2;
int demoPipeFd;			/* 管道文件描述符 */

STATUS progStart(void);
STATUS task1(void);
STATUS task2(void);
void progStop(void);

STATUS progStart(void)
{
	int result;			/* 记录函数返回的结果 */

	/* 负责创建一个名为“/pipe/demo”的管道 */
	result = pipeDevCreate("/pipe/demo", MAX_MSGS, MAX_MSG_LEN);
	if (result == ERROR)
	{
		return (ERROR);
	}

	/* 将管道打开，并将管道的文件描述符保存至demoPipeFd */
	demoPipeFd == open("/pipe/demo", O_RDWR, 0);
	if (demoPipeFd == ERROR)
	{
		return (ERROR);
	}

	tidTask1 = taskSpawn("tTask1", 200, 0, STACK_SIZE,
						(FUNCPTR)task1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	tidTask2 = taskSpawn("tTask2", 220, 0, STACK_SIZE,
						(FUNCPTR)task1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	return (OK);
}


/* 定义发送消息的内容 */
#define MESSAGE "Greetings from Task 1"

/* task1: 周期性地向管道写入信息 */
STATUS task1(void)
{
	FOREVER
	{
		/* 向管道写入一条消息 */
		write(demoPipeFd, MESSAGE, sizeof(MESSAGE));

		taskDelay(DELAY_TICKS);
	}

	return (OK);
}

/* task2: 从管道读出消息并显示 */
STATUS task2(void)
{
	char msgBuf[MAX_MSG_LEN];		/* 用于接收消息 */
	int len = 0;					/* 保存接收的消息地长度 */
	int result;						/* 用于保存select()的返回结果 */
	fd_set readFd;					/* 文件描述符几何，用于侦听管道的读缓冲 */

	FOREVER
	{
		/* 初始化数据 */
		memset(msgBuf, 0, MAX_MSG_LEN);
		len = 0;

		/* 读出一整条消息，当管道空时，阻塞（PEND）在select()上等待 */
		while (len < sizeof(MESSAGE))
		{
			/* 侦听管道的读缓冲 */
			FD_ZERO(&readFd);
			FD_SET(demoPipeFd, &readFd);
			result = select(sizeof(fd_set), &readFd, NULL, NULL, NULL);

			if (result <= 0)
			{
				return (ERROR);
			}

			/* 从管道中读出数据 */
			len += read(demoPipeFd, msgBuf, sizeof(MESSAGE) - len);
		}

		printf("Message from task 1: &s\n", msgBuf);
	}

	return (OK);
}

void progStop(void)
{
	taskDelete(tidTask1);
	taskDelete(tidTask2);

	close(demoPipedFd);
	pipeDevDelete("pipe/demo", TRUE);

	printf("BYE!\n");
	return;
}



