
#include <stdio.h>
#include "./device.h"
#include <string.h>



/*
*GPGSV:�õ�GPGSV������,�ɼ���������,���Ǳ��,��������,���Ƿ�λ��,���������
*GPGGA:�õ�GPS״̬,���ڶ�λ��������,���θ߶�
*GPGSA:�õ���λ����,��λ���Ǳ��,PDOPλ�þ�������,HDOPλ�þ�������,VDOPλ�þ�������
*GPRMC:�õ�UTCʱ�䣬��γ�ȣ�
*GPVTG:�õ���������
*/









unsigned char *gprmc = "$GPRMC,023543.00,A,2308.28715,N,11322.09875,E,0.195,,240213,,,A*78";
int main(char argc,char **argv)
{
	nmea_msg gpsx;
	memset(&gpsx,0,sizeof(nmea_msg));
	

	
	NMEA_GPRMC_Analysis(&gpsx,gprmc);	//GPGSV����
	nmea_MsgInfo(&gpsx);
	
	

	return 0;
}
