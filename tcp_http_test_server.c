
//#include <stdio.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <time.h> 
#include <errno.h> 
#include <signal.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/wait.h> 
#include <sys/time.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include<netdb.h>



#define IPSTAR "192.168.1.45"
#define ipaddr "112.74.124.112"
#define PORT 80
#if 1


static int socket_open(void)
{
	int socketld;
	struct sockaddr_in serv_addr;
	int status;
	socketld = socket(AF_INET, SOCK_STREAM, 0);
	if(socketld<0)
	{
		printf("error,create a socket failed!\n");
		return -1;
	}
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	if(inet_pton(AF_INET, ipaddr, &serv_addr.sin_addr) <= 0 )

	 { 
		printf("创建网络连接失败,本线程即将终止--inet_pton error!\n"); 
		exit(0);
	 }								      
	status = connect(socketld,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if(status!=0)
	{
		printf("error,Connecting failed!\n");
		//closesocket(socketld);
		return -1;
	}
	printf("connected!\r\n");

	return socketld;

}



static int socket_recv(int socketfd)
{
	int h,i;
        fd_set t_set1; 
	char buf[4096];
	struct timeval  tv; 
	FD_ZERO(&t_set1); 
        FD_SET(socketfd, &t_set1); 
	while(1)
	{ 
               sleep(2); 
               tv.tv_sec= 0; 
               tv.tv_usec= 0; 
               h= 0; 
               printf("--------------->1\n"); 
               h= select(socketfd+1,&t_set1,NULL,NULL,&tv); 
               printf("--------------->2\n"); 
 
               //if (h == 0) continue; 
                if(h < 0) 
		{ 
                       close(socketfd); 
                       printf("在读取数据报文时SELECT检测到异常，该异常导致线程终止！\n"); 
                       return-1; 
                }; 
 
               if(h > 0){ 
                       memset(buf, 0, 4096); 
                       i= read(socketfd, buf, 4095); 
                       if(i==0){ 
                               close(socketfd); 
                               printf("读取数据报文时发现远端关闭，该线程终止！\n"); 
                               return-1; 
                      } 
 
                       printf("%s\n", buf); 
               } 
       } 
       close(socketfd); 
 
       return 0; 

}
int  smtpIp(char * url)
{
	struct in_addr addr;
	struct hostent *iphost;
	 char *IP;
	 IP = (char *)malloc(sizeof(char)*32);
	memset(IP,0,sizeof(IP));
	iphost = gethostbyname("www.baidu.com");
	if(iphost == NULL)
	{
		printf(" gethostbyname error for host:%s\n",url);
		return 0; 
	}
	memcpy(&addr.s_addr,iphost->h_addr_list[0],iphost->h_length);
	IP=inet_ntoa(addr);
	printf("IP:%s\n",IP);


}

/*
static const char *protocol ="POST /api/devices\n"
"HOST 192.168.1.45:3000"
"content-type: application/x-www-form-urlencoded\n"
"postman-token: e3b3f534-d2fc-d3bd-e341-a62e3a492f16\n"
"cache-control: no-cache\n\n"
"id=11111&lat=1&lng=1\r\n\r\n"

*/





int main(char argc, char **argv)
{
	char str1[4096],*str;
	int ret;
	int sockld;

       


	sockld = socket_open();
	if(sockld<0)
	{
		return -1;
	}
	printf("open ok!!\n");
	/*
	strcat(str1,"POST /api/devices HTTP/1.1\r\n");
	strcat(str1,"HOST:192.168.1.45:3000\r\n");
	strcat(str1,"content-type: application/x-www-form-urlencoded\r\n");
	strcat(str1, "Content-Length: 23\r\n"); 
	strcat(str1,"\r\nid=123456&lat=23&lng=34");
	*/







/*一下三种方法都可以正常访问http，注意不要多空格，/等字符*/
	

	strcat(str1,"GET ");
	strcat(str1,"/ HTTP/1.1\r\n");
	strcat(str1,"HOST: ");
	//strcat(str1,"112.74.124.112\r\n\r\n");
	strcat(str1,"www.hua1268.com\r\n\r\n");
/*
	strcat(str1,"GET ");
	strcat(str1,"/ HTTP/1.1\r\n");
	strcat(str1,"HOST: ");
	//strcat(str1,"112.74.124.112\r\n\r\n");	//可以用IP或者域名直接访问，前面不能加http://但能加www.
	strcat(str1,"hua1268.com\r\n\r\n");
*/
/*
	strcat(str1,"GET ");
	strcat(str1,"/");
        strcat(str1," HTTP/1.1\r\n");//至此为http请求行的信息
        strcat(str1,"HOST:");
        strcat(str1,"hua1268.com");
        strcat(str1,"\r\n");
        strcat(str1,"Cache-Control: no-cache\r\n\r\n");
*/
	printf("%s",str1);
	ret = write(sockld,str1,strlen(str1)); 
	if(ret < 0)
	{ 
               printf("发送失败！错误代码是%d，错误信息是'%s'\n",errno, strerror(errno)); 
               exit(0); 
        }
	else
        { 
               printf("消息发送成功，共发送了%d个字节！\n\n", ret); 
        } 

	socket_recv(sockld);



	return 0;
}

#endif
#if 0




int main(char argc,char **argv)
{
	int sockld;
	int status;
	struct sockaddr_in serv_addr;
	sockld = socket(AF_INET, SOCK_STREAM, 0);
	if(sockld < 0)
	{	
		printf("create socket failed!\n");
		return -1;
	}
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(80);
	if(inet_pton(AF_INET, ipaddr, &serv_addr.sin_addr) <= 0 )

	 { 
		printf("创建网络连接失败,本线程即将终止--inet_pton error!\n"); 
		exit(0);
	 }								      
	status = connect(sockld,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if(status!=0)
	{
		printf("error,Connecting failed!\n");
		//closesocket(socketld);
		return -1;
	}
	printf("connected!\r\n");

	return sockld;
}
#endif
