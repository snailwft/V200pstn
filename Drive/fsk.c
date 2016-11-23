#include "config.h"
#include "fsk.h"
#include "uart.h"

ST_FSK_MEG_STATE stFskMeg;
uchar fsk_ucgetflag = 0;

void fsk_init()
{
	memset(&stFskMeg, 0x0, sizeof(stFskMeg));
}

int CheckFSKMessage(uchar * DataBuf, uchar DataLength)
{
	uchar ucTemp;
	uchar ucTemp1;
	uchar ucStartPoint;
	uchar ucGetFskStep;
	uint  uiCheckSum;

	ucStartPoint = 0;
	ucGetFskStep = 1;
	uiCheckSum = 0;
	fsk_init();
	
	for(ucTemp = 0; ucTemp < DataLength; ucTemp++)						//�������յ������ݷŽ�����
	{
		stFskMeg.ucFSK_Buf[stFskMeg.ucRecCnt] = DataBuf[ucTemp];// ȫ�ֱ���ucFSK_Buf �ڶ�����Ҫ���¸�ֵ
		if(stFskMeg.ucRecCnt < MAX_FSKBUF)
		{
			stFskMeg.ucRecCnt++;
		}
		else
		{
			stFskMeg.ucRecCnt = 0;
		}
	}

	for(ucTemp = 0; ucTemp < stFskMeg.ucRecCnt; ucTemp++)
	{
		if(ucGetFskStep == 1)
		{
			if(stFskMeg.ucFSK_Buf[ucTemp] == COMPLEX_MODE)
			{
				uart_send("message COMPLEX_MODE", strlen("message COMPLEX_MODE"));
				ucStartPoint = ucTemp;							//FSK�����Ǹ������ݸ�ʽ
				ucGetFskStep = 2;
			}
			else if(stFskMeg.ucFSK_Buf[ucTemp] == SINGLE_MODE)
			{
				uart_send("message SINGLE_MODE", strlen("message SINGLE_MODE"));
				ucStartPoint = ucTemp;							//FSK�����ǵ����ݸ�ʽ
				ucGetFskStep = 3;
			}

			else if(stFskMeg.ucFSK_Buf[ucTemp] == FSK_HEAD_ID)
			{
				continue;
			}
#if 0
			else
			{
				uart_send("message isn't fsk", strlen("message isn't fsk"));
				stFskMeg.ucGetFlag = 0;				//���յ������־
				stFskMeg.ucRecCnt = 0;
				fsk_ucgetflag = 0;
				return -1; // error message
			}
#endif
		}
		else if(ucGetFskStep == 2)
		{
			if(stFskMeg.ucRecCnt < (ucStartPoint + 2 + stFskMeg.ucFSK_Buf[ucStartPoint + 1]))	//��֤���������ٴ���
			{
				uart_send("message not complete", strlen("message not complete"));
				return 0;
			}
			for(ucTemp1 = ucStartPoint; ucTemp1 < (ucStartPoint + 2 + stFskMeg.ucFSK_Buf[ucStartPoint + 1]); ucTemp1++)
			{
				uiCheckSum += stFskMeg.ucFSK_Buf[ucTemp1];
			}
			uiCheckSum &= 0x00ff;
			ucGetFskStep = 4;
		}
		else if(ucGetFskStep == 3)
		{
			if(stFskMeg.ucRecCnt < (ucStartPoint + 2 + stFskMeg.ucFSK_Buf[ucStartPoint + 1]))	//��֤���������ٴ���
			{
				uart_send("message not complete", strlen("message not complete"));
				return 0;
			}
			for(ucTemp1 = ucStartPoint; ucTemp1 < (ucStartPoint + 2 + stFskMeg.ucFSK_Buf[ucStartPoint + 1]); ucTemp1++)
			{
				uiCheckSum += stFskMeg.ucFSK_Buf[ucTemp1];
			}
			uiCheckSum &= 0x00ff;
			
#if 1		// �Լ� ��ȡʱ�� add snail
			for (ucTemp1 = 0; ucTemp1 < 8; ucTemp1++) //��ȡʱ��
			{
				stFskMeg.ucTime[ucTemp1] = stFskMeg.ucFSK_Buf[ucStartPoint + 2 + ucTemp1];
			}
#endif
			
			for(ucTemp1 = 8; ucTemp1 < stFskMeg.ucFSK_Buf[ucStartPoint + 1]; ucTemp1++)		//ȡ���� ��ôֱ�ӵ���8
			{			
				stFskMeg.ucFskNum[ucTemp1 - 8] = stFskMeg.ucFSK_Buf[ucStartPoint + 2 + ucTemp1];
			}
			stFskMeg.ucNumLength = stFskMeg.ucFSK_Buf[ucStartPoint + 1] - 8;

			//HT9032_PD_SET;
			CLR_BIT(LPC_GPIO1,DATA,9);  	 	// ����PDWN��������ģʽ
			stFskMeg.ucGetFlag = 1;				//���յ������־
			fsk_ucgetflag = 1;
			stFskMeg.ucRecCnt = 0;
			return 1;
		}
		else if(ucGetFskStep == 4)
		{
#if 1
			if (stFskMeg.ucFSK_Buf[ucTemp] == COMPLEX_M_TIME)
			{
				for(ucTemp1 = 0; ucTemp1 < stFskMeg.ucFSK_Buf[ucTemp + 1]; ucTemp1++)		//ȡʱ��
				{			
					stFskMeg.ucTime[ucTemp1] = stFskMeg.ucFSK_Buf[ucTemp + 2 + ucTemp1];
				}
			}
#endif
			if(stFskMeg.ucFSK_Buf[ucTemp] == COMPLEX_M_DIAL)
			{
				for(ucTemp1 = 0; ucTemp1 < stFskMeg.ucFSK_Buf[ucTemp + 1]; ucTemp1++)		//ȡ����
				{					
					stFskMeg.ucFskNum[ucTemp1] = stFskMeg.ucFSK_Buf[ucTemp + 2 + ucTemp1];
				}
				stFskMeg.ucNumLength = stFskMeg.ucFSK_Buf[ucTemp + 1];
				CLR_BIT(LPC_GPIO1,DATA,9); // ����PDWN��������ģʽ
				stFskMeg.ucRecCnt = 0;
				stFskMeg.ucGetFlag = 1;			//���յ������־
				fsk_ucgetflag = 1;
				//HT9032_PD_SET;
				return 1;
			}
		}
	}
	return 0;
}

