/**********************************************************************
* �������ź�����������״̬��Flag�����ź�����(SEM_FULL)ʱ�����ڿ���״̬���ź�����(SEM_EMPTY)ʱ�����ڲ����ã�
* �ɿ����������á���semTake�� �ɲ����������á���semGive��
*/
SEM_ID semBCreate
	(
	int optiions,		/* ѡ�������������ȼ�SEM_Q_PRIORITY���Ƚ��ȳ�SEM_Q_FIFO */
	SEM_B_STATE initialState /* ��ʼ״̬ */
	��

/* �����������ź�����FIFO�Ŷӷ�ʽ����ʼ״̬Ϊ�� */ 
semID = semBCreate(SEM_Q_FIFO, SEM_FULL)
semID = semBCreate(SEM_Q_FIFO | SEM_EVENTSEND_ERR_NOTIFY, SEM_FULL)		/* ���¼���ص�ȡֵ��semGive����ERROR */

STATUS semDelete ( SEM_ID semID ��		/* ��Ҫɾ����ϲ����ID */ 


if (semTake(semID, NO_WAIT) == OK)
{
	semDelete(semID);
}

/* �������ź����Ļ�ȡ���ͷ� */
STATUS semTake
	(
	SEM_ID semId,		/* ��Ҫ��ȡ���ź���ID */
	int timeout			/* ��ʱʱ�䣨tick��  */
	)

STATUS semGive
	(
	SEM_ID semId		/* ��Ҫ�ͷŵ��ź���ID */
	)

/* semFlush()��һ������ͨ�����ô˺�������������ĳ�������ź����ϵ��������� */
STATUS semFlush
	(
	SEM_ID semId		/* �ź���ID����Ҫ����(Unpend)���������ϵ����� */
	��