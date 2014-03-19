/* 创建消息队列， 任务间的一种通讯机制 */
MSG_Q_ID msgQCreate
	(
	int maxMsgs,		/* 消息队列长度（消息队列中最大信息个数） */
	int maxMsgLength,	/* 消息队列中消息的最大长度 */
	int options			/* 选项 */
	)

/* exp */ 
msgQId = msgQCreate( 10, 20, MSG_Q_FIFO | MSG_Q_EVENTSEND_ERR_NOTIFY)

/* 删除消息队列 */
STATUS msgQDelete
	(
	MSQ_Q_ID msgQId		/* 需要删除的消息队列的ID */
	) 

	/* exp */
msgQDelete(msgQid)

/* 消息队列的发送 */
STATUS msgQSend
	(
	MSG_Q_ID msgQId,	/* 消息队列ID */ 
	char *buffer,		/* 指向要发送的消息的指针 */
	UINT nBytes,		/* 消息长度 */
	int timeout,		/* 超时参数（tick）*/
	int priority		/* MSG_PRI_NORMAL or MSG_PRI_URGENT 
						 * 一般来说，消息是按FIFO的顺序通过消息队列的，
						 * 调用函数时的优先级参数priority为MSG_PRI_NORMAL
						 * 如果需要将新来的消息插在消息队列头部，需指定优先级参数priority为MSG_PRI_URGENT，表明是紧急的
						 */
	)

/* 消息队列的接受 */
int msgQReceive
	(
	MSG_Q_ID msgQId,
	char *buffer,		/* 指向接收信息的缓冲指针 */
	UINT maxNBytes,		/* 接受缓冲的字节数 */
	int timeout
	）

