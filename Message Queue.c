/* ������Ϣ���У� ������һ��ͨѶ���� */
MSG_Q_ID msgQCreate
	(
	int maxMsgs,		/* ��Ϣ���г��ȣ���Ϣ�����������Ϣ������ */
	int maxMsgLength,	/* ��Ϣ��������Ϣ����󳤶� */
	int options			/* ѡ�� */
	)

/* exp */ 
msgQId = msgQCreate( 10, 20, MSG_Q_FIFO | MSG_Q_EVENTSEND_ERR_NOTIFY)

/* ɾ����Ϣ���� */
STATUS msgQDelete
	(
	MSQ_Q_ID msgQId		/* ��Ҫɾ������Ϣ���е�ID */
	) 

	/* exp */
msgQDelete(msgQid)

/* ��Ϣ���еķ��� */
STATUS msgQSend
	(
	MSG_Q_ID msgQId,	/* ��Ϣ����ID */ 
	char *buffer,		/* ָ��Ҫ���͵���Ϣ��ָ�� */
	UINT nBytes,		/* ��Ϣ���� */
	int timeout,		/* ��ʱ������tick��*/
	int priority		/* MSG_PRI_NORMAL or MSG_PRI_URGENT 
						 * һ����˵����Ϣ�ǰ�FIFO��˳��ͨ����Ϣ���еģ�
						 * ���ú���ʱ�����ȼ�����priorityΪMSG_PRI_NORMAL
						 * �����Ҫ����������Ϣ������Ϣ����ͷ������ָ�����ȼ�����priorityΪMSG_PRI_URGENT�������ǽ�����
						 */
	)

/* ��Ϣ���еĽ��� */
int msgQReceive
	(
	MSG_Q_ID msgQId,
	char *buffer,		/* ָ�������Ϣ�Ļ���ָ�� */
	UINT maxNBytes,		/* ���ܻ�����ֽ��� */
	int timeout
	��

