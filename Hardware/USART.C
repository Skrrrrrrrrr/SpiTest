
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU RC Demo -----------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ---------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* --- QQ:  800003751 ----------------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/


#include "USART.h"


COMx_Define	COM1,COM2,COM3,COM4;
u8 	xdata RX1_Buffer[RX_Length];	//���ջ���
u8 	xdata RX2_Buffer[RX_Length];	//���ջ���



//========================================================================
// ����: void	InitUartValue(COMx_Define *COMx)
// ����: ���ڳ�ʼ������������
// ����: *COMx: ��ʼ������ָ��.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void	InitUartValue(COMx_Define *COMx)
{
	u8	i;
	COMx->TX_read  = 0;
	COMx->TX_Busy  = 0;
	COMx->RX_write = 0;
	for(i=0; i<RX_Length; i++)	COMx->RX_Buffer[i] = 0;
}


//========================================================================
// ����: u8 USART_Configuration(u8 UARTx, COMx_InitDefine *COMx)
// ����: ���ڳ�ʼ��������
// ����: UARTx: Ҫ��ʼ���Ĵ���, ȡֵΪ USART1, USART2, USART3, USART4.
//       *COMx: ��ʼ������ָ��.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
u8 USART_Configuration(u8 UARTx, COMx_InitDefine *COMx)
{
	u32	j;
	
	if(UARTx == USART1)
	{
		COM1.id = 1;
		COM1.RX_Buffer = RX1_Buffer;
		InitUartValue(&COM1);

		if(COMx->UART_Mode > UART_9bit_BRTx)	return 2;	//ģʽ����
	//	if(COMx->UART_Polity == PolityHigh)		PS = 1;	//�����ȼ��ж�
	//	else									PS = 0;	//�����ȼ��ж�
		SCON = (SCON & 0x3f) | COMx->UART_Mode;
		if((COMx->UART_Mode == UART_9bit_BRTx) ||(COMx->UART_Mode == UART_8bit_BRTx))	//�ɱ䲨����
		{
			j = (MAIN_Fosc / 4) / COMx->UART_BaudRate;	//��1T����
			if(j >= 65536UL)	return 2;	//����
			j = 65536UL - j;
			if(COMx->UART_BRT_Use == BRT_Timer1)
			{
				TR1 = 0;
				AUXR &= ~0x01;		//S1 BRT Use Timer1;
				TMOD &= ~(1<<6);	//Timer1 set As Timer
				TMOD &= ~0x30;		//Timer1_16bitAutoReload;
				AUXR |=  (1<<6);	//Timer1 set as 1T mode
				TH1 = (u8)(j>>8);
				TL1 = (u8)j;
				ET1 = 0;	//��ֹ�ж�
				INT_CLKO &= ~0x02;	//�����ʱ��
				TR1  = 1;
			}
			else if(COMx->UART_BRT_Use == BRT_Timer2)
			{
				AUXR &= ~(1<<4);	//Timer stop
				AUXR |= 0x01;		//S1 BRT Use Timer2;
				AUXR &= ~(1<<3);	//Timer2 set As Timer
				AUXR |=  (1<<2);	//Timer2 set as 1T mode
				TH2 = (u8)(j>>8);
				TL2 = (u8)j;
				IE2  &= ~(1<<2);	//��ֹ�ж�
				AUXR |=  (1<<4);	//Timer run enable
			}
			else return 2;	//����
		}
		else if(COMx->UART_Mode == UART_ShiftRight)
		{
			AUXR &= ~(1<<5);	//�̶�������SysClk/12
		}
		else if(COMx->UART_Mode == UART_9bit)	//�̶�������SysClk*2^SMOD/64
		{
			PCON &= ~(1<<7);	//�̶�������SysClk/64
		}
		if(COMx->UART_Interrupt == ENABLE)	ES = 1;	//�����ж�
		else								ES = 0;	//��ֹ�ж�
		if(COMx->UART_RxEnable == ENABLE)	REN = 1;	//��������
		else								REN = 0;	//��ֹ����
		P_SW1 = (P_SW1 & 0x3f) | (COMx->UART_P_SW & 0xc0);	//�л�IO
		return	0;
	}

	if(UARTx == USART2)
	{
		COM2.id = 2;
		COM2.RX_Buffer = RX2_Buffer;
		InitUartValue(&COM2);

		if((COMx->UART_Mode == UART_9bit_BRTx) ||(COMx->UART_Mode == UART_8bit_BRTx))	//�ɱ䲨����
		{
			if(COMx->UART_Mode == UART_9bit_BRTx)	S2CON |=  (1<<7);	//9bit
			else									S2CON &= ~(1<<7);	//8bit
			j = (MAIN_Fosc / 4) / COMx->UART_BaudRate;	//��1T����
			if(j >= 65536UL)	return 2;	//����
			j = 65536UL - j;
			AUXR &= ~(1<<4);	//Timer stop
			AUXR &= ~(1<<3);	//Timer2 set As Timer
			AUXR |=  (1<<2);	//Timer2 set as 1T mode
			TH2 = (u8)(j>>8);
			TL2 = (u8)j;
			IE2  &= ~(1<<2);	//��ֹ�ж�
			AUXR |=  (1<<4);	//Timer run enable
		}
		else	return 2;	//ģʽ����
		if(COMx->UART_Interrupt == ENABLE)	IE2   |=  1;		//�����ж�
		else								IE2   &= ~1;		//��ֹ�ж�
		if(COMx->UART_RxEnable == ENABLE)	S2CON |=  (1<<4);	//��������
		else								S2CON &= ~(1<<4);	//��ֹ����
		P_SW2 = (P_SW2 & ~1) | (COMx->UART_P_SW & 0x01);	//�л�IO
		return	0;
	}

	return	3;	//��������
}


/*************** װ�ش��ڷ��ͻ��� *******************************/
void TX_write2buff(u8 UARTx,u8 dat)	//д�뷢�ͻ��壬ָ��+1
{

	switch (UARTx)
	{
	case USART1:
		TX1_Buffer[COM1.TX_write] = dat;	//װ���ͻ���
		if(++COM1.TX_write >= TX_Length)	COM1.TX_write = 0;

		if(COM1.B_TX_busy == 0)		//����
		{  
			COM1.B_TX_busy = 1;		//��־æ
			TI = 1;					//���������ж�
		}
		break;
	case USART2:
		TX2_Buffer[COM2.TX_write] = dat;	//װ���ͻ���
		if(++COM2.TX_write >= TX_Length)	COM2.TX_write = 0;

		if(COM2.B_TX_busy == 0)		//����
		{  
			COM2.B_TX_busy = 1;		//��־æ
			TI = 1;					//���������ж�
		}		break;
	default:
		break;
	}


}
//========================================================================
// ����: void PrintString(u8 UARTx, u8 *puts)
// ����: �ַ������ͺ�����
// ����: UARTx: Ҫ�����Ĵ���, ȡֵΪ USART1, USART2, USART3, USART4.
//       *puts: Ҫ���͵��ַ���ָ��.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
// void PrintString(u8 UARTx, u8 *puts)
// {
// 	for (; *puts != 0;	puts++)
// 	{
// 		if(UARTx == USART1)
// 		{
// 			COM1.TX_Busy = 1;			//��־����æ
// 			SBUF = *puts;				//��һ���ֽ�
// 			while(COM1.TX_Busy > 0);	//�ȴ��������
// 		}
// 		else if(UARTx == USART2)
// 		{
// 			COM2.TX_Busy = 1;			//��־����æ
// 			S2BUF = *puts;				//��һ���ֽ�
// 			while(COM2.TX_Busy > 0);	//�ȴ��������
// 		}
// 	}
// }


void PrintString(u8 UARTx,u8 *puts)
{
	for (; *puts != 0;	puts++)  TX_write2buff(UARTx,*puts); 	//����ֹͣ��0����

}

/********************* UART1�жϺ���************************/
void UART1_int (void) interrupt UART1_VECTOR
{
	if(RI)
	{
		RI = 0;
		RX1_Buffer[COM1.RX_write] = SBUF;
		if(++COM1.RX_write >= RX_Length)	COM1.RX_write = 0;
	}

	if(TI)
	{
		TI = 0;
		COM1.TX_Busy = 0;
	}
}

/********************* UART2�жϺ���************************/
void UART2_int (void) interrupt UART2_VECTOR
{
	if(RI2)
	{
		CLR_RI2();
		RX2_Buffer[COM2.RX_write] = S2BUF;
		if(++COM2.RX_write >= RX_Length)	COM2.RX_write = 0;
	}

	if(TI2)
	{
		CLR_TI2();
		COM2.TX_Busy = 0;
	}

}