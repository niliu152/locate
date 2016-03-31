#ifndef __TCP_HTTP_TEST_SERVER_H__
#define __TCP_HTTP_TEST_SERVER_H__




typedef struct device_Info{
	char imei[20];
	char num[15];
	char longitude[15];
	char latitude[15];
	char id[5];
	char reserved1;
	char reserved2;
	char reserved3;
}device_info;


int data_splice(device_info *device,char *str)
{
	int len;

	if(str == NULL|| device == NULL)
		return -1;


	len = sprintf(str,"%s",device->imei);
	strcat(str,",");
	len+=1;
	
	len += sprintf(str+len,"%s",device->num);
	strcat(str,",");
	len+=1;

	len += sprintf(str+len,"%s",device->longitude);
	strcat(str,",");
	len+=1;

	len += sprintf(str+len,"%s",device->latitude);
	strcat(str,",");
	len+=1;

	len += sprintf(str+len,"%s",device->id);
	strcat(str,",");	
	len+=1;	

	len += sprintf(str+len,"%d",device->reserved1);
	strcat(str,",");	
	len+=1;	

	len += sprintf(str+len,"%d",device->reserved2);
	strcat(str,",");	
	len+=1;	

	len += sprintf(str+len,"%d",device->reserved3);
	return len;
	
}
unsigned char http_splice(char *str,char *buf)
{

	int len;
	if(str == NULL||buf == NULL)
		return -1;
	len = sprintf(buf,"POST /api/v1/position_system/device HTTP/1.1\r\n");			
	len += sprintf(buf+len,"HOST: hua1268.com\r\n");
	len += sprintf(buf + len,"Cache-Control: no-cache\r\n");				
	len += sprintf(buf + len,"Content-Type: application/x-www-form-urlencoded\r\n");	
	len += sprintf(buf + len,"Content-length: 64\r\n\r\n");  				
	len += sprintf(buf + len,"data=");							
	len += sprintf(buf + len,"%s",str);							

	return len;
/*
test ok
	len = sprintf(buf,"POST /api/v1/position_system/device HTTP/1.1\r\n");			
	len += sprintf(buf+len,"HOST: hua1268.com\r\n");
	len += sprintf(buf + len,"Cache-Control: no-cache\r\n");				
	len += sprintf(buf + len,"Content-Type: application/x-www-form-urlencoded\r\n");	
	len += sprintf(buf + len,"Content-length: 64\r\n\r\n");  				
	len += sprintf(buf + len,"data=");							
	len += sprintf(buf + len,"%s",str);	

*/

}



#endif
