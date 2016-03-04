
#include <stdio.h>
#include "./device.h"
#include <string.h>



/*
*GPGSV:得到GPGSV的条数,可见卫星总数,卫星编号,卫星仰角,卫星方位角,卫星信噪比
*GPGGA:得到GPS状态,用于定位的卫星数,海拔高度
*GPGSA:得到定位类型,定位卫星编号,PDOP位置精度因子,HDOP位置精度因子,VDOP位置精度因子
*GPRMC:得到UTC时间，经纬度，
*GPVTG:得到地面速率
*/









unsigned char *gprmc = "$GPRMC,023543.00,A,2308.28715,N,11322.09875,E,0.195,,240213,,,A*78";
int main(char argc,char **argv)
{
	nmea_msg gpsx;
	memset(&gpsx,0,sizeof(nmea_msg));
	

	
	NMEA_GPRMC_Analysis(&gpsx,gprmc);	//GPGSV解析
	nmea_MsgInfo(&gpsx);
	
	

	return 0;
}
