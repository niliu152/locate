
#ifdef __CUSTOMER_CODE__
#include "custom_feature_def.h"
#include "ril.h"
#include "ril_telephony.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"

/*****************************************************************
* debug print
* need to initialize the __DBG_PORT__ first
******************************************************************/
#define DBG_BUF_LEN  (512)
static char _DBG_BUF_[DBG_BUF_LEN]={0};
#define __DBG_PORT__  UART_PORT1
#define UART_PRINT(...)  QL_TRACE_LOG(__DBG_PORT__,_DBG_BUF_,DBG_BUF_LEN,__VA_ARGS__)


#define SERIAL_RX_BUFFER_LEN  2048

static u32 Location_timer =TIMER_ID_USER_START + 0x3;
static u32 Location_time = 60000;	//1min
//static u8 Location_cnt = 5;		//5*1min
#define R_CNT 5
#define G_CNT 2
static u8 cnt=0;
static Enum_SerialPort Locate_UartPort  = UART_PORT1;
static u8 m_RxBuf_Uart1[SERIAL_RX_BUFFER_LEN];
static u16 rlen = 256;

static void InitializeSerialPort(void);
static void CallBack_UART_Hdlr(Enum_SerialPort port, Enum_UARTEventType msg, bool level, void* customizedPara);
static s32 ATResponse_Handler(char* line, u32 len, void* userData);
static device_Info device;
static void GPIO_Init(void)
{
	//gsm
	Enum_PinName  gpioPin = PINNAME_NETLIGHT;
	Enum_PinLevel gpioLvl = PINLEVEL_LOW;

	Ql_GPIO_Init(gpioPin, PINDIRECTION_OUT, gpioLvl, PINPULLSEL_DISABLE);
	//power
	gpioPin = PINNAME_PCM_SYNC;
	gpioLvl = PINLEVEL_LOW;
	Ql_GPIO_Init(gpioPin, PINDIRECTION_IN, gpioLvl, PINPULLSEL_DISABLE);
}

#if 0
void proc_main_task(s32 taskId)
{
    s32 ret;
    ST_MSG msg;
    Enum_PinLevel gpioLvl = PINLEVEL_HIGH;
	InitializeSerialPort();	
	
	UART_PRINT("OpenCPU: Customer Application\r\n");
	GPIO_Init();

	while(1)
    {
		UART_PRINT("\r\nMain Task Running...\r\n");
        Ql_Sleep(500);
        gpioLvl = ~gpioLvl & 0x01;
        Ql_GPIO_SetLevel(PINNAME_NETLIGHT, gpioLvl);
		Ql_GPIO_SetLevel(PINNAME_PCM_CLK, gpioLvl);
		Ql_GPIO_SetLevel(PINNAME_PCM_IN, gpioLvl);
		//Ql_GPIO_SetLevel(PINNAME_PCM_OUT, gpioLvl);
		//Ql_GPIO_SetLevel(PINNAME_PCM_SYNC, gpioLvl);

		UART_PRINT("<-- PCM_OUT Level:%d,PCM_SYNC_Level:%d -->\r\n", Ql_GPIO_GetLevel(PINNAME_PCM_OUT),Ql_GPIO_GetLevel(PINNAME_PCM_SYNC));
		
    }

    // START MESSAGE LOOP OF THIS TASK
    while(TRUE)
    {
        Ql_OS_GetMessage(&msg);
        switch(msg.message)
        {
        case MSG_ID_RIL_READY:
            UART_PRINT("<-- RIL is ready -->\r\n");
            Ql_RIL_Initialize();
            break;
        case MSG_ID_URC_INDICATION:
            //UART_PRINT("<-- Received URC: type: %d, -->\r\n", msg.param1);
            switch (msg.param1)
            {
            case URC_SYS_INIT_STATE_IND:
                UART_PRINT("<-- Sys Init Status %d -->\r\n", msg.param2);
                break;
            case URC_SIM_CARD_STATE_IND:
                UART_PRINT("<-- SIM Card Status:%d -->\r\n", msg.param2);
                break;
            case URC_GSM_NW_STATE_IND:
                UART_PRINT("<-- GSM Network Status:%d -->\r\n", msg.param2);
                break;
            case URC_GPRS_NW_STATE_IND:
                UART_PRINT("<-- GPRS Network Status:%d -->\r\n", msg.param2);
                break;
            case URC_CFUN_STATE_IND:
                UART_PRINT("<-- CFUN Status:%d -->\r\n", msg.param2);
                break;
            case URC_COMING_CALL_IND:
                {
                    ST_ComingCall* pComingCall = (ST_ComingCall*)msg.param2;
                    UART_PRINT("<-- Coming call, number:%s, type:%d -->\r\n", pComingCall->phoneNumber, pComingCall->type);
                    break;
                }
            case URC_CALL_STATE_IND:
                UART_PRINT("<-- Call state:%d\r\n", msg.param2);
                break;
            case URC_NEW_SMS_IND:
                UART_PRINT("<-- New SMS Arrives: index=%d\r\n", msg.param2);
                break;
            case URC_MODULE_VOLTAGE_IND:
                UART_PRINT("<-- VBatt Voltage Ind: type=%d\r\n", msg.param2);
                break;
            default:
                UART_PRINT("<-- Other URC: type=%d\r\n", msg.param1);
                break;
            }
            break;
        default:
            break;
        }
    }
}
#endif

void Location_Timer_handler(u32 timerId, void* param)
{
	s32 ret;
	*((s32*)param) +=1;
	if(Location_timer== timerId)
	{
		UART_PRINT("<-- Timer_handler,timer %d run %d times-->\r\n",timerId);
		cnt++;
	}

}
static void Get_DeviceId(Device_Info *device)
{
	//TODO
	device->id = 0;
}
static void Get_DeviceNum(Device_Info *device)
{
	device->sim = "18318066360"
}

static void DeviceBspInit()
{
	s32 ret;
	Ql_memset( device, 0,sizeof(device));
	InitializeSerialPort();
	GPIO_Init();
	cnt=0;
	ret = Ql_Timer_Register(Location_timer, Location_Timer_handler, NULL);
	if(QL_RET_OK!=ret)
	{
		UART_PRINT("\r\nTimer Register error!!\r\n");
		return;
	}
	Ql_Timer_Start(Location_timer, Location_time,TRUE);
	// GET DEVICE INFO
	Get_DeviceID(&device);
	Get_DeviceNum(&device);
	
	//register or next	
	
	//TODO
	
}
static void Locate_report(device_Info *data);
{
	//	ret = Ql_RIL_SendATCmd(m_RxBuf_Uart1, totalBytes, ATResponse_Handler, NULL, 0);

	
}

void proc_main_task(s32 taskId)
{
	s32 ret;
    	ST_MSG msg;
	
	DeviceBspInit();
	
	if(cnt == R_CNT)
	{
		cnt =0;
		//TODO report;
		Locate_report();
	}
	else if (cnt == G_CNT)
	{
		
	}

	
}






static void InitializeSerialPort(void)//m_myUartPort is same with __DBG_PORT__
{
		
    	s32 ret;


	ret = Ql_UART_Register(Locate_UartPort, CallBack_UART_Hdlr, NULL);
	if (ret < QL_RET_OK)
	{
	  	  UART_PRINT("Fail to register serial port[%d], ret=%d\r\n", Locate_UartPort, ret);
	}
	ret = Ql_UART_Open(Locate_UartPort, 115200, FC_NONE);
	if (ret < QL_RET_OK)
	{
	 	   UART_PRINT("Fail to open serial port[%d], ret=%d\r\n", Locate_UartPort, ret);
	}
}
static s32 ReadSerialPort(Enum_SerialPort port, /*[out]*/u8* pBuffer, /*[in]*/u32 bufLen)
{
    s32 rdLen = 0;
    s32 rdTotalLen = 0;
    if (NULL == pBuffer || 0 == bufLen)
    {
        return -1;
    }
    Ql_memset(pBuffer, 0x0, bufLen);
    while (1)
    {
        rdLen = Ql_UART_Read(port, pBuffer + rdTotalLen, bufLen - rdTotalLen);
        if (rdLen <= 0)  // All data is read out, or Serial Port Error!
        {
            break;
        }
        rdTotalLen += rdLen;
        // Continue to read...
    }
    if (rdLen < 0) // Serial Port Error!
    {
        UART_PRINT("Fail to read from port[%d]\r\n", port);
        return -99;
    }
    return rdTotalLen;
}

static void CallBack_UART_Hdlr(Enum_SerialPort port, Enum_UARTEventType msg, bool level, void* customizedPara)
{
    //UART_PRINT("CallBack_UART_Hdlr: port=%d, event=%d, level=%d, p=%x\r\n", port, msg, level, customizedPara);
	switch (msg)
	{
		case EVENT_UART_READY_TO_READ:
		{

			if (m_myUartPort == port)
			{
				s32 totalBytes = ReadSerialPort(port, m_RxBuf_Uart1, sizeof(m_RxBuf_Uart1));
				if (totalBytes <= 0)
				{
					UART_PRINT("<-- No data in UART buffer! -->\r\n");
					return;
				}	
				//todo uart data analysis
				//	ret = Ql_RIL_SendATCmd(m_RxBuf_Uart1, totalBytes, ATResponse_Handler, NULL, 0);
				
			} 

			break;
		}
	case EVENT_UART_READY_TO_WRITE:
		
		break;
	default:
		break;
	}
}

static s32 ATResponse_Handler(char* line, u32 len, void* userData)
{
	Ql_UART_Write(m_myUartPort, line, len);

	if (Ql_RIL_FindLine(line, len, "OK"))
	{  
		return  RIL_ATRSP_SUCCESS;
	}
	else if (Ql_RIL_FindLine(line, len, "ERROR"))
	{  
		return  RIL_ATRSP_FAILED;
	}
	else if (Ql_RIL_FindString(line, len, "+CME ERROR"))
	{
		return  RIL_ATRSP_FAILED;
	}
	else if (Ql_RIL_FindString(line, len, "+CMS ERROR:"))
	{
		return  RIL_ATRSP_FAILED;
	}
	return RIL_ATRSP_CONTINUE; //continue wait
}

#endif // __CUSTOMER_CODE__

