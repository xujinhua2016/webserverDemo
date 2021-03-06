#include "lwip/debug.h"
#include "httpd.h"
#include "lwip/tcp.h"
#include "fs.h"
#include "lwip_comm.h"
#include "led.h"
#include "beep.h"
#include "adc.h"
#include "rtc.h"
#include "lcd.h"

#include <string.h>
#include <stdlib.h>


#include  "bsp_mem.h"  
//#include  "GE_PM.h"

extern volatile uint32_t	GE_BAT_Vol[192];
extern volatile	uint32_t	BatIntRes[192];

extern volatile	uint32_t	Temperature;	    // 环境温度
extern volatile	uint32_t	ChargeCurrentVal; // 充放电电流
extern __IO	uint8_t	 BSP_BAT_CAN_BAD_Num;

extern __IO  uint8_t   BAT_Vol_Res_Dect_Flag;		//内阻测试开关的标志量

extern volatile uint32_t 	BatVolWarnBit000_015;	// #000~#015号电池
extern volatile uint32_t 	BatVolWarnBit016_031;

extern volatile uint32_t	BatResWarnBit000_015;	// #000~#015号电池
extern volatile uint32_t 	BatResWarnBit016_031;

extern volatile	uint32_t  GroBatWarn;	 



uint16_t connStatus;

//

#define NUM_CONFIG_CGI_URIS	(sizeof(ppcURLs) / sizeof(tCGI))
#define NUM_CONFIG_SSI_TAGS	(sizeof(ppcTAGs) / sizeof(char *))
	
extern short Get_Temprate(void);  //声明Get_Temperate()函数
void RTC_Get_Time(u8 *hour,u8 *min,u8 *sec,u8 *ampm){;}; //声明RTC_Get_Timer()函数
void RTC_Get_Date(u8 *year,u8 *month,u8 *date,u8 *week){;}; //声明RTC_Get_Date()函数

//控制内阻测试的CGI handler
const char* LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
//控制配置文件修改的CGI hander
const char* BEEP_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[]);
//登陆界面提交按钮处理
const char* login_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[]);
//内阻测试提交按钮处理
const char* resTest_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[]);
//参数配置密码验证提交按钮处理
const char* confChk_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[]);
//参数配置数据提交按钮处理
const char* confDate_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[]);
	
static const char *ppcTAGs[]=  //SSI的Tag
{
	"v", //电压值
	"r", //内阻值
	"x", //电压告警位
	"y", //内阻告警位
	"o", //其他告警信息
	"s", //从机连接状态和温度电流信息查询
	"c", //配置信息数据，modbus地址和波特率
};


static const tCGI ppcURLs[]= //cgi程序
{
	{"/leds.cgi",LEDS_CGI_Handler},
	{"/beep.cgi",BEEP_CGI_Handler},
	{"/login.cgi",login_CGI_Handler},
	{"/restest.cgi",resTest_CGI_Handler},
	{"/confchk.cgi",confChk_CGI_Handler},
	{"/confdate.cgi",confDate_CGI_Handler},
};


//当web客户端请求浏览器的时候,使用此函数被CGI handler调用
static int FindCGIParameter(const char *pcToFind,char *pcParam[],int iNumParams)
{
	int iLoop;
	for(iLoop = 0;iLoop < iNumParams;iLoop ++ )
	{
		if(strcmp(pcToFind,pcParam[iLoop]) == 0)
		{
			return (iLoop); //返回iLOOP
		}
	}
	return (-1);
}


//SSIHandler中需要用到的处理电压值的函数
void volVal_Handler(char *pcInsert)
{ 
		int i;
	
    char Digit[24]; 
		char Digit1[24]; 
		char Digit2[24]; 
		char Digit3[24]; 
		uint32_t ADCVal[24];  
		
		for ( i = 0; i < 24; i++ )
		{
			ADCVal[i] = GE_BAT_Vol[i];
		}  
			
     Digit[0]= ADCVal[0]/1000;
     Digit[1]= (ADCVal[0]-(Digit[0]*1000))/100 ;
     Digit[2]= (ADCVal[0]-((Digit[0]*1000)+(Digit[1]*100)))/10;
     Digit[3]= ADCVal[0] -((Digit[0]*1000)+(Digit[1]*100)+ (Digit[2]*10));
     
		 Digit[4]= ADCVal[1]/1000;
     Digit[5]= (ADCVal[1]-(Digit[4]*1000))/100 ;
     Digit[6]= (ADCVal[1]-((Digit[4]*1000)+(Digit[5]*100)))/10;
     Digit[7]= ADCVal[1] -((Digit[4]*1000)+(Digit[5]*100)+ (Digit[6]*10));

		 Digit[8]= ADCVal[2]/1000;
     Digit[9]= (ADCVal[2]-(Digit[8]*1000))/100 ;
     Digit[10]= (ADCVal[2]-((Digit[8]*1000)+(Digit[9]*100)))/10;
     Digit[11]= ADCVal[2] -((Digit[8]*1000)+(Digit[9]*100)+ (Digit[10]*10));

		 Digit[12]= ADCVal[3]/1000;
     Digit[13]= (ADCVal[3]-(Digit[12]*1000))/100 ;
     Digit[14]= (ADCVal[3]-((Digit[12]*1000)+(Digit[13]*100)))/10;
     Digit[15]= ADCVal[3] -((Digit[12]*1000)+(Digit[13]*100)+ (Digit[14]*10));
		 
		 Digit[16]= ADCVal[4]/1000;
     Digit[17]= (ADCVal[4]-(Digit[16]*1000))/100 ;
     Digit[18]= (ADCVal[4]-((Digit[16]*1000)+(Digit[17]*100)))/10;
     Digit[19]= ADCVal[4] -((Digit[16]*1000)+(Digit[17]*100)+ (Digit[18]*10));
		 
		 Digit[20]= ADCVal[5]/1000;
     Digit[21]= (ADCVal[5]-(Digit[20]*1000))/100 ;
     Digit[22]= (ADCVal[5]-((Digit[20]*1000)+(Digit[21]*100)))/10;
     Digit[23]= ADCVal[5] -((Digit[20]*1000)+(Digit[21]*100)+ (Digit[22]*10));
		 
		 
		 Digit1[0]= ADCVal[6]/1000;
     Digit1[1]= (ADCVal[6]-(Digit1[0]*1000))/100 ;
     Digit1[2]= (ADCVal[6]-((Digit1[0]*1000)+(Digit1[1]*100)))/10;
     Digit1[3]= ADCVal[6] -((Digit1[0]*1000)+(Digit1[1]*100)+ (Digit1[2]*10));
     
		 Digit1[4]= ADCVal[7]/1000;
     Digit1[5]= (ADCVal[7]-(Digit1[4]*1000))/100 ;
     Digit1[6]= (ADCVal[7]-((Digit1[4]*1000)+(Digit1[5]*100)))/10;
     Digit1[7]= ADCVal[7] -((Digit1[4]*1000)+(Digit1[5]*100)+ (Digit1[6]*10));

		 Digit1[8]= ADCVal[8]/1000;
     Digit1[9]= (ADCVal[8]-(Digit1[8]*1000))/100 ;
     Digit1[10]= (ADCVal[8]-((Digit1[8]*1000)+(Digit1[9]*100)))/10;
     Digit1[11]= ADCVal[8] -((Digit1[8]*1000)+(Digit1[9]*100)+ (Digit1[10]*10));

		 Digit1[12]= ADCVal[9]/1000;
     Digit1[13]= (ADCVal[9]-(Digit1[12]*1000))/100 ;
     Digit1[14]= (ADCVal[9]-((Digit1[12]*1000)+(Digit1[13]*100)))/10;
     Digit1[15]= ADCVal[9] -((Digit1[12]*1000)+(Digit1[13]*100)+ (Digit1[14]*10));
		 
		 Digit1[16]= ADCVal[10]/1000;
     Digit1[17]= (ADCVal[10]-(Digit1[16]*1000))/100 ;
     Digit1[18]= (ADCVal[10]-((Digit1[16]*1000)+(Digit1[17]*100)))/10;
     Digit1[19]= ADCVal[10] -((Digit1[16]*1000)+(Digit1[17]*100)+ (Digit1[18]*10));
		 
		 Digit1[20]= ADCVal[11]/1000;
     Digit1[21]= (ADCVal[11]-(Digit1[20]*1000))/100 ;
     Digit1[22]= (ADCVal[11]-((Digit1[20]*1000)+(Digit1[21]*100)))/10;
     Digit1[23]= ADCVal[11] -((Digit1[20]*1000)+(Digit1[21]*100)+ (Digit1[22]*10));


		 Digit2[0]= ADCVal[12]/1000;
     Digit2[1]= (ADCVal[12]-(Digit2[0]*1000))/100 ;
     Digit2[2]= (ADCVal[12]-((Digit2[0]*1000)+(Digit2[1]*100)))/10;
     Digit2[3]= ADCVal[12] -((Digit2[0]*1000)+(Digit2[1]*100)+ (Digit2[2]*10));
     
		 Digit2[4]= ADCVal[13]/1000;
     Digit2[5]= (ADCVal[13]-(Digit2[4]*1000))/100 ;
     Digit2[6]= (ADCVal[13]-((Digit2[4]*1000)+(Digit2[5]*100)))/10;
     Digit2[7]= ADCVal[13] -((Digit2[4]*1000)+(Digit2[5]*100)+ (Digit2[6]*10));

		 Digit2[8]= ADCVal[14]/1000;
     Digit2[9]= (ADCVal[14]-(Digit2[8]*1000))/100 ;
     Digit2[10]= (ADCVal[14]-((Digit2[8]*1000)+(Digit2[9]*100)))/10;
     Digit2[11]= ADCVal[14] -((Digit2[8]*1000)+(Digit2[9]*100)+ (Digit2[10]*10));

		 Digit2[12]= ADCVal[15]/1000;
     Digit2[13]= (ADCVal[15]-(Digit2[12]*1000))/100 ;
     Digit2[14]= (ADCVal[15]-((Digit2[12]*1000)+(Digit2[13]*100)))/10;
     Digit2[15]= ADCVal[15] -((Digit2[12]*1000)+(Digit2[13]*100)+ (Digit2[14]*10));
		 
		 Digit2[16]= ADCVal[16]/1000;
     Digit2[17]= (ADCVal[16]-(Digit2[16]*1000))/100 ;
     Digit2[18]= (ADCVal[16]-((Digit2[16]*1000)+(Digit2[17]*100)))/10;
     Digit2[19]= ADCVal[16] -((Digit2[16]*1000)+(Digit2[17]*100)+ (Digit2[18]*10));
		 
		 Digit2[20]= ADCVal[17]/1000;
     Digit2[21]= (ADCVal[17]-(Digit2[20]*1000))/100 ;
     Digit2[22]= (ADCVal[17]-((Digit2[20]*1000)+(Digit2[21]*100)))/10;
     Digit2[23]= ADCVal[17] -((Digit2[20]*1000)+(Digit2[21]*100)+ (Digit2[22]*10));
		 
		 
		 Digit3[0]= ADCVal[18]/1000;
     Digit3[1]= (ADCVal[18]-(Digit3[0]*1000))/100 ;
     Digit3[2]= (ADCVal[18]-((Digit3[0]*1000)+(Digit3[1]*100)))/10;
     Digit3[3]= ADCVal[18] -((Digit3[0]*1000)+(Digit3[1]*100)+ (Digit3[2]*10));
     
		 Digit3[4]= ADCVal[19]/1000;
     Digit3[5]= (ADCVal[19]-(Digit3[4]*1000))/100 ;
     Digit3[6]= (ADCVal[19]-((Digit3[4]*1000)+(Digit3[5]*100)))/10;
     Digit3[7]= ADCVal[19] -((Digit3[4]*1000)+(Digit3[5]*100)+ (Digit3[6]*10));

		 Digit3[8]= ADCVal[20]/1000;
     Digit3[9]= (ADCVal[20]-(Digit3[8]*1000))/100 ;
     Digit3[10]= (ADCVal[20]-((Digit3[8]*1000)+(Digit3[9]*100)))/10;
     Digit3[11]= ADCVal[20] -((Digit3[8]*1000)+(Digit3[9]*100)+ (Digit3[10]*10));

		 Digit3[12]= ADCVal[21]/1000;
     Digit3[13]= (ADCVal[21]-(Digit3[12]*1000))/100 ;
     Digit3[14]= (ADCVal[21]-((Digit3[12]*1000)+(Digit3[13]*100)))/10;
     Digit3[15]= ADCVal[21] -((Digit3[12]*1000)+(Digit3[13]*100)+ (Digit3[14]*10));
		 
		 Digit3[16]= ADCVal[22]/1000;
     Digit3[17]= (ADCVal[22]-(Digit3[16]*1000))/100 ;
     Digit3[18]= (ADCVal[22]-((Digit3[16]*1000)+(Digit3[17]*100)))/10;
     Digit3[19]= ADCVal[22] -((Digit3[16]*1000)+(Digit3[17]*100)+ (Digit3[18]*10));
		 
		 Digit3[20]= ADCVal[23]/1000;
     Digit3[21]= (ADCVal[23]-(Digit3[20]*1000))/100 ;
     Digit3[22]= (ADCVal[23]-((Digit3[20]*1000)+(Digit3[21]*100)))/10;
     Digit3[23]= ADCVal[23] -((Digit3[20]*1000)+(Digit3[21]*100)+ (Digit3[22]*10));


     //准备添加到html中的数据
     
		 *pcInsert       = (char)(Digit[0]+0x30);//不可放到循环语句之中
		
		 for(i = 1;i < 24; i++) {
			 *(pcInsert + i) = (char)(Digit[i]+0x30);
		 }
		 
		 for(i = 0; i < 24; i++) {
			 *(pcInsert + (24 + i)) = (char)(Digit1[i]+0x30);
			
		 }
		 
		 for(i = 0; i < 24; i++) {
			 *(pcInsert + (48 + i)) = (char)(Digit2[i]+0x30);
			 
		 }
		 
		 
		 for(i = 0; i < 24; i++) {
			 *(pcInsert + (72 + i)) = (char)(Digit3[i]+0x30);
			 
		 }
		
}

//SSIHandler中需要用到的处理内阻值的函数
void resVal_Handler(char *pcInsert)
{
		int i;
		char Digit[24]; 
		char Digit1[24]; 
		char Digit2[24]; 
		char Digit3[24]; 
		uint32_t resVal[24];
		//uint32_t resVal[24] = {30,80,45,23,54,95,32,23,11,32,84,55,23,61,34,34,92,43,23,95,34,12,94,12};
		for ( i = 0; i < 24; i++ )
		{
			resVal[i] = BatIntRes[i];
		}  
        
		//获取内部温度值
		//Temperate = Get_Temprate(); //获取温度值 此处扩大了100倍
	 
			Digit[0]= resVal[0]/1000;
     Digit[1]= (resVal[0]-(Digit[0]*1000))/100 ;
     Digit[2]= (resVal[0]-((Digit[0]*1000)+(Digit[1]*100)))/10;
     Digit[3]= resVal[0] -((Digit[0]*1000)+(Digit[1]*100)+ (Digit[2]*10));
     
		 Digit[4]= resVal[1]/1000;
     Digit[5]= (resVal[1]-(Digit[4]*1000))/100 ;
     Digit[6]= (resVal[1]-((Digit[4]*1000)+(Digit[5]*100)))/10;
     Digit[7]= resVal[1] -((Digit[4]*1000)+(Digit[5]*100)+ (Digit[6]*10));

		 Digit[8]= resVal[2]/1000;
     Digit[9]= (resVal[2]-(Digit[8]*1000))/100 ;
     Digit[10]= (resVal[2]-((Digit[8]*1000)+(Digit[9]*100)))/10;
     Digit[11]= resVal[2] -((Digit[8]*1000)+(Digit[9]*100)+ (Digit[10]*10));

		 Digit[12]= resVal[3]/1000;
     Digit[13]= (resVal[3]-(Digit[12]*1000))/100 ;
     Digit[14]= (resVal[3]-((Digit[12]*1000)+(Digit[13]*100)))/10;
     Digit[15]= resVal[3] -((Digit[12]*1000)+(Digit[13]*100)+ (Digit[14]*10));
		 
		 Digit[16]= resVal[4]/1000;
     Digit[17]= (resVal[4]-(Digit[16]*1000))/100 ;
     Digit[18]= (resVal[4]-((Digit[16]*1000)+(Digit[17]*100)))/10;
     Digit[19]= resVal[4] -((Digit[16]*1000)+(Digit[17]*100)+ (Digit[18]*10));
		 
		 Digit[20]= resVal[5]/1000;
     Digit[21]= (resVal[5]-(Digit[20]*1000))/100 ;
     Digit[22]= (resVal[5]-((Digit[20]*1000)+(Digit[21]*100)))/10;
     Digit[23]= resVal[5] -((Digit[20]*1000)+(Digit[21]*100)+ (Digit[22]*10));
		 
		 
		 Digit1[0]= resVal[6]/1000;
     Digit1[1]= (resVal[6]-(Digit1[0]*1000))/100 ;
     Digit1[2]= (resVal[6]-((Digit1[0]*1000)+(Digit1[1]*100)))/10;
     Digit1[3]= resVal[6] -((Digit1[0]*1000)+(Digit1[1]*100)+ (Digit1[2]*10));
     
		 Digit1[4]= resVal[7]/1000;
     Digit1[5]= (resVal[7]-(Digit1[4]*1000))/100 ;
     Digit1[6]= (resVal[7]-((Digit1[4]*1000)+(Digit1[5]*100)))/10;
     Digit1[7]= resVal[7] -((Digit1[4]*1000)+(Digit1[5]*100)+ (Digit1[6]*10));

		 Digit1[8]= resVal[8]/1000;
     Digit1[9]= (resVal[8]-(Digit1[8]*1000))/100 ;
     Digit1[10]= (resVal[8]-((Digit1[8]*1000)+(Digit1[9]*100)))/10;
     Digit1[11]= resVal[8] -((Digit1[8]*1000)+(Digit1[9]*100)+ (Digit1[10]*10));

		 Digit1[12]= resVal[9]/1000;
     Digit1[13]= (resVal[9]-(Digit1[12]*1000))/100 ;
     Digit1[14]= (resVal[9]-((Digit1[12]*1000)+(Digit1[13]*100)))/10;
     Digit1[15]= resVal[9] -((Digit1[12]*1000)+(Digit1[13]*100)+ (Digit1[14]*10));
		 
		 Digit1[16]= resVal[10]/1000;
     Digit1[17]= (resVal[10]-(Digit1[16]*1000))/100 ;
     Digit1[18]= (resVal[10]-((Digit1[16]*1000)+(Digit1[17]*100)))/10;
     Digit1[19]= resVal[10] -((Digit1[16]*1000)+(Digit1[17]*100)+ (Digit1[18]*10));
		 
		 Digit1[20]= resVal[11]/1000;
     Digit1[21]= (resVal[11]-(Digit1[20]*1000))/100 ;
     Digit1[22]= (resVal[11]-((Digit1[20]*1000)+(Digit1[21]*100)))/10;
     Digit1[23]= resVal[11] -((Digit1[20]*1000)+(Digit1[21]*100)+ (Digit1[22]*10));


		 Digit2[0]= resVal[12]/1000;
     Digit2[1]= (resVal[12]-(Digit2[0]*1000))/100 ;
     Digit2[2]= (resVal[12]-((Digit2[0]*1000)+(Digit2[1]*100)))/10;
     Digit2[3]= resVal[12] -((Digit2[0]*1000)+(Digit2[1]*100)+ (Digit2[2]*10));
     
		 Digit2[4]= resVal[13]/1000;
     Digit2[5]= (resVal[13]-(Digit2[4]*1000))/100 ;
     Digit2[6]= (resVal[13]-((Digit2[4]*1000)+(Digit2[5]*100)))/10;
     Digit2[7]= resVal[13] -((Digit2[4]*1000)+(Digit2[5]*100)+ (Digit2[6]*10));

		 Digit2[8]= resVal[14]/1000;
     Digit2[9]= (resVal[14]-(Digit2[8]*1000))/100 ;
     Digit2[10]= (resVal[14]-((Digit2[8]*1000)+(Digit2[9]*100)))/10;
     Digit2[11]= resVal[14] -((Digit2[8]*1000)+(Digit2[9]*100)+ (Digit2[10]*10));

		 Digit2[12]= resVal[15]/1000;
     Digit2[13]= (resVal[15]-(Digit2[12]*1000))/100 ;
     Digit2[14]= (resVal[15]-((Digit2[12]*1000)+(Digit2[13]*100)))/10;
     Digit2[15]= resVal[15] -((Digit2[12]*1000)+(Digit2[13]*100)+ (Digit2[14]*10));
		 
		 Digit2[16]= resVal[16]/1000;
     Digit2[17]= (resVal[16]-(Digit2[16]*1000))/100 ;
     Digit2[18]= (resVal[16]-((Digit2[16]*1000)+(Digit2[17]*100)))/10;
     Digit2[19]= resVal[16] -((Digit2[16]*1000)+(Digit2[17]*100)+ (Digit2[18]*10));
		 
		 Digit2[20]= resVal[17]/1000;
     Digit2[21]= (resVal[17]-(Digit2[20]*1000))/100 ;
     Digit2[22]= (resVal[17]-((Digit2[20]*1000)+(Digit2[21]*100)))/10;
     Digit2[23]= resVal[17] -((Digit2[20]*1000)+(Digit2[21]*100)+ (Digit2[22]*10));
		 
		 
		 Digit3[0]= resVal[18]/1000;
     Digit3[1]= (resVal[18]-(Digit3[0]*1000))/100 ;
     Digit3[2]= (resVal[18]-((Digit3[0]*1000)+(Digit3[1]*100)))/10;
     Digit3[3]= resVal[18] -((Digit3[0]*1000)+(Digit3[1]*100)+ (Digit3[2]*10));
     
		 Digit3[4]= resVal[19]/1000;
     Digit3[5]= (resVal[19]-(Digit3[4]*1000))/100 ;
     Digit3[6]= (resVal[19]-((Digit3[4]*1000)+(Digit3[5]*100)))/10;
     Digit3[7]= resVal[19] -((Digit3[4]*1000)+(Digit3[5]*100)+ (Digit3[6]*10));

		 Digit3[8]= resVal[20]/1000;
     Digit3[9]= (resVal[20]-(Digit3[8]*1000))/100 ;
     Digit3[10]= (resVal[20]-((Digit3[8]*1000)+(Digit3[9]*100)))/10;
     Digit3[11]= resVal[20] -((Digit3[8]*1000)+(Digit3[9]*100)+ (Digit3[10]*10));

		 Digit3[12]= resVal[21]/1000;
     Digit3[13]= (resVal[21]-(Digit3[12]*1000))/100 ;
     Digit3[14]= (resVal[21]-((Digit3[12]*1000)+(Digit3[13]*100)))/10;
     Digit3[15]= resVal[21] -((Digit3[12]*1000)+(Digit3[13]*100)+ (Digit3[14]*10));
		 
		 Digit3[16]= resVal[22]/1000;
     Digit3[17]= (resVal[22]-(Digit3[16]*1000))/100 ;
     Digit3[18]= (resVal[22]-((Digit3[16]*1000)+(Digit3[17]*100)))/10;
     Digit3[19]= resVal[22] -((Digit3[16]*1000)+(Digit3[17]*100)+ (Digit3[18]*10));
		 
		 Digit3[20]= resVal[23]/1000;
     Digit3[21]= (resVal[23]-(Digit3[20]*1000))/100 ;
     Digit3[22]= (resVal[23]-((Digit3[20]*1000)+(Digit3[21]*100)))/10;
     Digit3[23]= resVal[23] -((Digit3[20]*1000)+(Digit3[21]*100)+ (Digit3[22]*10));
			
			
			
		 //准备添加到html中的数据
     
		 *pcInsert       = (char)(Digit[0]+0x30);//不可放到循环语句之中
		
		 for(i = 1;i < 24; i++) {
			 *(pcInsert + i) = (char)(Digit[i]+0x30);
		 }
		 
		 for(i = 0; i < 24; i++) {
			 *(pcInsert + (24 + i)) = (char)(Digit1[i]+0x30);
			
		 }
		 
		 for(i = 0; i < 24; i++) {
			 *(pcInsert + (48 + i)) = (char)(Digit2[i]+0x30);
			 
		 }
		 
		 
		 for(i = 0; i < 24; i++) {
			 *(pcInsert + (72 + i)) = (char)(Digit3[i]+0x30);
			 
		 }
}

//SSIHandler中需要用到的处理电压告警值的函数
void volWarn_Handler(char *pcInsert)
{	
	int i;
	
	//拼接数值
	uint32_t batVolWarn = BatVolWarnBit016_031 << 16 | (BatVolWarnBit000_015 & 0xffff);
	//24位数，对应十进制是8位，千万
	char Digit[8];
	
	
	Digit[0] = batVolWarn/10000000;
	Digit[1] = (batVolWarn - Digit[0]*10000000)/1000000;
	Digit[2] = (batVolWarn - Digit[0]*10000000 - Digit[1]*1000000)/100000;
	Digit[3] = (batVolWarn - Digit[0]*10000000 - Digit[1]*1000000 - Digit[2]*100000)/10000;
	Digit[4] = (batVolWarn - Digit[0]*10000000 - Digit[1]*1000000 - Digit[2]*100000 - Digit[3]*10000)/1000;
	Digit[5] = (batVolWarn - Digit[0]*10000000 - Digit[1]*1000000 - Digit[2]*100000 - Digit[3]*10000 - Digit[4]*1000)/100;
	Digit[6] = (batVolWarn - Digit[0]*10000000 - Digit[1]*1000000 - Digit[2]*100000 - Digit[3]*10000 - Digit[4]*1000 - Digit[5]*100)/10;
	Digit[7] = (batVolWarn - Digit[0]*10000000 - Digit[1]*1000000 - Digit[2]*100000 - Digit[3]*10000 - Digit[4]*1000 - Digit[5]*100 - Digit[6]*10);
	
	//准备添加到html中的数据
	for( i = 0; i < 8; i++) {
		 *(pcInsert + + i) = (char)(Digit[i]+0x30); 
	 }
	
}

//SSIHandler中需要用到的处理内阻告警值的函数
void resWarn_Handler(char *pcInsert)
{
	int i;
	
	//拼接数值
	uint32_t batVolWarn = BatResWarnBit016_031 << 16 | BatResWarnBit000_015;
	//24位数，对应十进制是8位，千万
	char Digit[8];
	
	
	Digit[0] = batVolWarn/10000000;
	Digit[1] = (batVolWarn - Digit[0]*10000000)/1000000;
	Digit[2] = (batVolWarn - Digit[0]*10000000 - Digit[1]*1000000)/100000;
	Digit[3] = (batVolWarn - Digit[0]*10000000 - Digit[1]*1000000 - Digit[2]*100000)/10000;
	Digit[4] = (batVolWarn - Digit[0]*10000000 - Digit[1]*1000000 - Digit[2]*100000 - Digit[3]*10000)/1000;
	Digit[5] = (batVolWarn - Digit[0]*10000000 - Digit[1]*1000000 - Digit[2]*100000 - Digit[3]*10000 - Digit[4]*1000)/100;
	Digit[6] = (batVolWarn - Digit[0]*10000000 - Digit[1]*1000000 - Digit[2]*100000 - Digit[3]*10000 - Digit[4]*1000 - Digit[5]*100)/10;
	Digit[7] = (batVolWarn - Digit[0]*10000000 - Digit[1]*1000000 - Digit[2]*100000 - Digit[3]*10000 - Digit[4]*1000 - Digit[5]*100 - Digit[6]*10);
	
	//准备添加到html中的数据
	for( i = 0; i < 8; i++) {
		 *(pcInsert + + i) = (char)(Digit[i]+0x30); 
	 }
	
}
//SSIHandler中需要用到的处理其他告警值的函数
void otrWarn_Handler(char *pcInsert)
{
	int i;
	
	//拼接数值
	uint32_t batVolWarn = GroBatWarn;
	//24位数，对应十进制是8位，千万
	char Digit[8];
	
	
	Digit[0] = batVolWarn/10000000;
	Digit[1] = (batVolWarn - Digit[0]*10000000)/1000000;
	Digit[2] = (batVolWarn - Digit[0]*10000000 - Digit[1]*1000000)/100000;
	Digit[3] = (batVolWarn - Digit[0]*10000000 - Digit[1]*1000000 - Digit[2]*100000)/10000;
	Digit[4] = (batVolWarn - Digit[0]*10000000 - Digit[1]*1000000 - Digit[2]*100000 - Digit[3]*10000)/1000;
	Digit[5] = (batVolWarn - Digit[0]*10000000 - Digit[1]*1000000 - Digit[2]*100000 - Digit[3]*10000 - Digit[4]*1000)/100;
	Digit[6] = (batVolWarn - Digit[0]*10000000 - Digit[1]*1000000 - Digit[2]*100000 - Digit[3]*10000 - Digit[4]*1000 - Digit[5]*100)/10;
	Digit[7] = (batVolWarn - Digit[0]*10000000 - Digit[1]*1000000 - Digit[2]*100000 - Digit[3]*10000 - Digit[4]*1000 - Digit[5]*100 - Digit[6]*10);
	
	//准备添加到html中的数据
	for( i = 0; i < 8; i++) {
		 *(pcInsert + + i) = (char)(Digit[i]+0x30); 
	 }
	
}

//SSIHandler中需要用到的处理其他告警值的函数
void staWarn_Handler(char *pcInsert)
{
	int i;
	
	uint32_t TempVal = Temperature;//温度信息
	uint32_t CurrVal = ChargeCurrentVal;//电流信息
	uint8_t ConSVal = BSP_BAT_CAN_BAD_Num;//从机连接状态信息
	//24位数，对应十进制是8位，千万
	char Digit[15];
	
	
	Digit[0] = TempVal/10000;
	Digit[1] = (TempVal - Digit[0]*10000)/1000;
	Digit[2] = (TempVal - Digit[0]*10000 - Digit[1]*1000)/100;
	Digit[3] = (TempVal - Digit[0]*10000 - Digit[1]*1000 - Digit[2]*100)/10;
	Digit[4] = (TempVal - Digit[0]*10000 - Digit[1]*1000 - Digit[2]*100 - Digit[3]*10);
	
	Digit[5] = CurrVal/10000;
	Digit[6] = (CurrVal - Digit[5]*10000)/1000;
	Digit[7] = (CurrVal - Digit[5]*10000 - Digit[6]*1000)/100;
	Digit[8] = (CurrVal - Digit[5]*10000 - Digit[6]*1000 - Digit[7]*100)/10;
	Digit[9] = (CurrVal - Digit[5]*10000 - Digit[6]*1000 - Digit[7]*100 - Digit[8]*10);
	
	Digit[10] = ConSVal/10000;
	Digit[11] = (ConSVal - Digit[10]*10000)/1000;
	Digit[12] = (ConSVal - Digit[10]*10000 - Digit[11]*1000)/100;
	Digit[13] = (ConSVal - Digit[10]*10000 - Digit[11]*1000 - Digit[12]*100)/10;
	Digit[14] = (ConSVal - Digit[10]*10000 - Digit[11]*1000 - Digit[12]*100 - Digit[13]*10);

	//准备添加到html中的数据
	for( i = 0; i < 15; i++) {
		 *(pcInsert + + i) = (char)(Digit[i]+0x30); 
	 }
	
}
//SSIHandler中需要用到的处理其他告警值的函数
void confVal_Handler(char *pcInsert)
{
	int i;
	uint32_t addrVal = BKP_ReadBackupRegister(SlaveAddress);//Modbus地址信息
	uint32_t rateVal = BKP_ReadBackupRegister(TransBaudRate);//电流信息
	uint8_t ConSVal = BSP_BAT_CAN_BAD_Num;//从机连接状态信息
	//24位数，对应十进制是8位，千万
	char Digit[9];
	
	
	Digit[0] = addrVal/100;
	Digit[1] = (addrVal - Digit[0]*100)/10;
	Digit[2] = (addrVal - Digit[0]*100 - Digit[1]*10);
	
	Digit[3] = rateVal/100000;
	Digit[4] = (rateVal - Digit[3]*100000)/10000;	
	Digit[5] = (rateVal - Digit[3]*100000 - Digit[4]*10000)/1000;	
	Digit[6] = (rateVal - Digit[3]*100000 - Digit[4]*10000 - Digit[5]*1000)/100;	
	Digit[7] = (rateVal - Digit[3]*100000 - Digit[4]*10000 - Digit[5]*1000 - Digit[6]*100)/10;
	Digit[8] = (rateVal - Digit[3]*100000 - Digit[4]*10000 - Digit[5]*1000 - Digit[6]*100 - Digit[7]*10);

	//准备添加到html中的数据
	for( i = 0; i < 9; i++) {
		 *(pcInsert + + i) = (char)(Digit[i]+0x30); 
	 }
	
}
//SSI的Handler句柄
static u16_t SSIHandler(int iIndex,char *pcInsert,int iInsertLen)
{
	switch(iIndex)
	{
		case 0: 
				volVal_Handler(pcInsert);
				break;
		case 1:
				resVal_Handler(pcInsert);
				break;
		case 2:
				volWarn_Handler(pcInsert);
				break;
		case 3:
				resWarn_Handler(pcInsert);
				break;
		case 4:
				otrWarn_Handler(pcInsert);
				break;
		case 5:
				staWarn_Handler(pcInsert);
				break;
		case 6:
				confVal_Handler(pcInsert);
				break;
	}
	return strlen(pcInsert);
}

//CGI LED控制句柄
const char* LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  u8 i=0;  //注意根据自己的GET的参数的多少来选择i值范围
  
	iIndex = FindCGIParameter("LED1",pcParam,iNumParams);  //找到led的索引号
	//只有一个CGI句柄 iIndex=0
	if (iIndex != -1)
	{
			LED1=1;  //关闭所有的LED1灯
		for (i=0; i<iNumParams; i++) //检查CGI参数: example GET /leds.cgi?led=2&led=4 */
		{
		  if (strcmp(pcParam[i] , "LED1")==0)  //检查参数"led"
		  {
			if(strcmp(pcValue[i], "LED1ON") ==0)  //改变LED1状态
			  LED1=0; //打开LED1
					else if(strcmp(pcValue[i],"LED1OFF") == 0)
						LED1=1; //关闭LED1
		  }
		}
	 }
	if(LED1 == 0 && BEEP == 0) 		return "/STM32F407LED_ON_BEEP_OFF.shtml";  	//LED1开,BEEP关
	else if(LED1 == 0 && BEEP == 1) return "/STM32F407LED_ON_BEEP_ON.shtml";	//LED1开,BEEP开
	else if(LED1 == 1 && BEEP == 1) return "/STM32F407LED_OFF_BEEP_ON.shtml";	//LED1关,BEEP开
	else return "/STM32F407LED_OFF_BEEP_OFF.shtml";   							//LED1关,BEEP关					
}

//BEEP的CGI控制句柄
const char *BEEP_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[])
{
	u8 i=0;
	iIndex = FindCGIParameter("BEEP",pcParam,iNumParams);  //找到BEEP的索引号
	if(iIndex != -1) //找到BEEP索引号
	{
		BEEP=0;  //关闭
		for(i = 0;i < iNumParams;i++)
		{
			if(strcmp(pcParam[i],"BEEP") == 0)  //查找CGI参数
			{
				if(strcmp(pcValue[i],"BEEPON") == 0) //打开BEEP
					BEEP = 1;
				else if(strcmp(pcValue[i],"BEEPOFF") == 0) //关闭BEEP
					BEEP = 0;
			}
		}
	}
	if(LED1 == 0 && BEEP == 0) 		return "/STM32F407LED_ON_BEEP_OFF.shtml";  	//LED1开,BEEP关
	else if(LED1 == 0 && BEEP == 1) return "/STM32F407LED_ON_BEEP_ON.shtml";	//LED1开,BEEP开
	else if(LED1 == 1 && BEEP == 1) return "/STM32F407LED_OFF_BEEP_ON.shtml";	//LED1关,BEEP开
	else return "/STM32F407LED_OFF_BEEP_OFF.shtml";   //LED1关,BEEP关
	
}

const char *login_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[])
{
	//在此处，可以进行参数的验证，此处略。在前端页面处理验证。
	 return "/userinfo.shtml";  
	
}

const char *resTest_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[])
{
	//
	u8 i=0;
	iIndex = FindCGIParameter("RESTEST",pcParam,iNumParams);  //找到BEEP的索引号
	if(iIndex != -1) //找到BEEP索引号
	{
		BEEP=0;  //关闭
		for(i = 0;i < iNumParams;i++)
		{
			if(strcmp(pcParam[i],"RESTEST") == 0)  //查找CGI参数
			{
				if(strcmp(pcValue[i],"TESTOK") == 0) //打开BEEP
					BAT_Vol_Res_Dect_Flag = 1;
				else
					;
			}
			else ;
		}	
		
	}
	 return "/data.shtml";;  
	
}

const char *confChk_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[])
{
	//在此处，可以进行参数的验证，此处略。在前端页面处理验证。
	 return "/config.shtml";  
	
}

const char *confDate_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[])
{
	//。
	u8 i=0;
	uint8_t modbus_add_temp;
	uint8_t modbus_rate_temp;
	
	iIndex = FindCGIParameter("ModbusAdd",pcParam,iNumParams);  //找到ModbusAdd的索引号,file:///C:/confdate.cgi?ModbusAdd=15&ModbusSed=4800
	if(iIndex != -1) //找到BEEP索引号
	{
		for(i = 0;i < iNumParams;i++)
		{
			if(strcmp(pcParam[i],"ModbusAdd") == 0)  //查找CGI参数
			{
				modbus_add_temp = atoi(pcValue[i]);
				BKP_WriteBackupRegister(SlaveAddress, modbus_add_temp);
			}
		}
	}
//	
//	iIndex = FindCGIParameter("ModbusSed",pcParam,iNumParams);  //找到ModbusAdd的索引号,file:///C:/confdate.cgi?ModbusAdd=15&ModbusSed=4800
//	if(iIndex != -1) //找到BEEP索引号
//	{
//		for(i = 0;i < iNumParams;i++)
//		{
//			if(strcmp(pcParam[i],"ModbusSed") == 0)  //查找CGI参数
//			{
//				modbus_rate_temp = atoi(pcValue[i]);
//				BKP_WriteBackupRegister(TransBaudRate, modbus_rate_temp);
//			}
//		}
//	}
	
	 return "/config.shtml";  
	
}


//SSI句柄初始化
void httpd_ssi_init(void)
{  
	//配置内部温度传感器的SSI句柄
	http_set_ssi_handler(SSIHandler,ppcTAGs,NUM_CONFIG_SSI_TAGS);
}

//CGI句柄初始化
void httpd_cgi_init(void)
{ 
  //配置CGI句柄LEDs control CGI) */
  http_set_cgi_handlers(ppcURLs, NUM_CONFIG_CGI_URIS);
}


