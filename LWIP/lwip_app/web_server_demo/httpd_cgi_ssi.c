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
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//NETCONN API��̷�ʽ��WebServer���Դ���	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/8/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   

extern volatile uint32_t	  GE_BAT_Vol[192];
extern volatile	uint32_t	BatIntRes[192];
extern __IO	uint8_t	 BSP_BAT_CAN_BAD_Num;
//extern 



uint16_t connStatus;

//

#define NUM_CONFIG_CGI_URIS	(sizeof(ppcURLs) / sizeof(tCGI))
#define NUM_CONFIG_SSI_TAGS	(sizeof(ppcTAGs) / sizeof(char *))
	
extern short Get_Temprate(void);  //����Get_Temperate()����
void RTC_Get_Time(u8 *hour,u8 *min,u8 *sec,u8 *ampm){;}; //����RTC_Get_Timer()����
void RTC_Get_Date(u8 *year,u8 *month,u8 *date,u8 *week){;}; //����RTC_Get_Date()����

//����������Ե�CGI handler
const char* LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
//���������ļ��޸ĵ�CGI hander
const char* BEEP_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[]);

static const char *ppcTAGs[]=  //SSI��Tag
{
	"v", //��ѹֵ
	"r", //����ֵ
	"h", //ʱ��
	"y", //����
};


static const tCGI ppcURLs[]= //cgi����
{
	{"/leds.cgi",LEDS_CGI_Handler},
	{"/beep.cgi",BEEP_CGI_Handler},
};


//��web�ͻ��������������ʱ��,ʹ�ô˺�����CGI handler����
static int FindCGIParameter(const char *pcToFind,char *pcParam[],int iNumParams)
{
	int iLoop;
	for(iLoop = 0;iLoop < iNumParams;iLoop ++ )
	{
		if(strcmp(pcToFind,pcParam[iLoop]) == 0)
		{
			return (iLoop); //����iLOOP
		}
	}
	return (-1);
}


//SSIHandler����Ҫ�õ��Ĵ���ADC�ĺ���
void ADC_Handler(char *pcInsert)
{ 
		int i;
	
    char Digit[24]; 
		char Digit1[24]; 
		char Digit2[24]; 
		char Digit3[24]; 
		uint32_t ADCVal[24];
		//uint32_t ADCVal[24] = {20,30,45,23,54,45,232,233,1,142,34,5,23,91,34,234,132,43,123,35,34,12,54,12};    
			for ( i = 0; i < 24; i++ )
		{
			ADCVal[i] = GE_BAT_Vol[i];
		}  
        

     //��ȡADC��ֵ
     //ADCVal = Get_Adc_Average(5,10); //��ȡADC1_CH5�ĵ�ѹֵ
		
     
     //ת��Ϊ��ѹ ADCVval * 0.8mv
     //ADCVal = (uint32_t)(ADCVal * 0.8); 
			
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


     //׼����ӵ�html�е�����
     
		 *pcInsert       = (char)(Digit[0]+0x30);//���ɷŵ�ѭ�����֮��
		
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

//SSIHandler����Ҫ�õ��Ĵ����ڲ��¶ȴ������ĺ���
void Temperate_Handler(char *pcInsert)
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
        
		//��ȡ�ڲ��¶�ֵ
		//Temperate = Get_Temprate(); //��ȡ�¶�ֵ �˴�������100��
	 
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
			
			
			
		 //׼����ӵ�html�е�����
     
		 *pcInsert       = (char)(Digit[0]+0x30);//���ɷŵ�ѭ�����֮��
		
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

//SSIHandler����Ҫ�õ��Ĵ���RTCʱ��ĺ���
void RTCTime_Handler(char *pcInsert)
{
	u8 hour,min,sec,ampm;
	
	RTC_Get_Time(&hour,&min,&sec,&ampm);
//	connStatus = BSP_BAT_CAN_BAD_Num;
	
	*pcInsert = 		(char)((hour/10) + 0x30);
	*(pcInsert+1) = (char)((hour%10) + 0x30);
	*(pcInsert+2) = ':';
	*(pcInsert+3) = (char)((min/10) + 0x30);
	*(pcInsert+4) = (char)((min%10) + 0x30);
	*(pcInsert+5) = ':';
	*(pcInsert+6) = (char)((sec/10) + 0x30);
	*(pcInsert+7) = (char)((sec%10) + 0x30);
}

//SSIHandler����Ҫ�õ��Ĵ���RTC���ڵĺ���
void RTCdate_Handler(char *pcInsert)
{
	u8 year,month,date,week;
	RTC_Get_Date(&year,&month,&date,&week);
	
	*pcInsert = '2';
	*(pcInsert+1) = '0';
	*(pcInsert+2) = (char)((year/10) + 0x30);
	*(pcInsert+3) = (char)((year%10) + 0x30);
	*(pcInsert+4) = '-';
	*(pcInsert+5) = (char)((month/10) + 0x30);
	*(pcInsert+6) = (char)((month%10) + 0x30);
	*(pcInsert+7) = '-';
	*(pcInsert+8) = (char)((date/10) + 0x30);
	*(pcInsert+9) = (char)((date%10) + 0x30);
	*(pcInsert+10) = ' ';
	*(pcInsert+11) = 'w';
	*(pcInsert+12) = 'e';
	*(pcInsert+13) = 'e';
	*(pcInsert+14) = 'k';
	*(pcInsert+15) = ':';
	*(pcInsert+16) = (char)(week + 0x30);
	
}
//SSI��Handler���
static u16_t SSIHandler(int iIndex,char *pcInsert,int iInsertLen)
{
	switch(iIndex)
	{
		case 0: 
				ADC_Handler(pcInsert);
				break;
		case 1:
				Temperate_Handler(pcInsert);
				break;
		case 2:
				RTCTime_Handler(pcInsert);
				break;
		case 3:
				RTCdate_Handler(pcInsert);
				break;
	}
	return strlen(pcInsert);
}

//CGI LED���ƾ��
const char* LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  u8 i=0;  //ע������Լ���GET�Ĳ����Ķ�����ѡ��iֵ��Χ
  
	iIndex = FindCGIParameter("LED1",pcParam,iNumParams);  //�ҵ�led��������
	//ֻ��һ��CGI��� iIndex=0
	if (iIndex != -1)
	{
			LED1=1;  //�ر����е�LED1��
		for (i=0; i<iNumParams; i++) //���CGI����: example GET /leds.cgi?led=2&led=4 */
		{
		  if (strcmp(pcParam[i] , "LED1")==0)  //������"led"
		  {
			if(strcmp(pcValue[i], "LED1ON") ==0)  //�ı�LED1״̬
			  LED1=0; //��LED1
					else if(strcmp(pcValue[i],"LED1OFF") == 0)
						LED1=1; //�ر�LED1
		  }
		}
	 }
	if(LED1 == 0 && BEEP == 0) 		return "/STM32F407LED_ON_BEEP_OFF.shtml";  	//LED1��,BEEP��
	else if(LED1 == 0 && BEEP == 1) return "/STM32F407LED_ON_BEEP_ON.shtml";	//LED1��,BEEP��
	else if(LED1 == 1 && BEEP == 1) return "/STM32F407LED_OFF_BEEP_ON.shtml";	//LED1��,BEEP��
	else return "/STM32F407LED_OFF_BEEP_OFF.shtml";   							//LED1��,BEEP��					
}

//BEEP��CGI���ƾ��
const char *BEEP_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[])
{
	u8 i=0;
	iIndex = FindCGIParameter("BEEP",pcParam,iNumParams);  //�ҵ�BEEP��������
	if(iIndex != -1) //�ҵ�BEEP������
	{
		BEEP=0;  //�ر�
		for(i = 0;i < iNumParams;i++)
		{
			if(strcmp(pcParam[i],"BEEP") == 0)  //����CGI����
			{
				if(strcmp(pcValue[i],"BEEPON") == 0) //��BEEP
					BEEP = 1;
				else if(strcmp(pcValue[i],"BEEPOFF") == 0) //�ر�BEEP
					BEEP = 0;
			}
		}
	}
	if(LED1 == 0 && BEEP == 0) 		return "/STM32F407LED_ON_BEEP_OFF.shtml";  	//LED1��,BEEP��
	else if(LED1 == 0 && BEEP == 1) return "/STM32F407LED_ON_BEEP_ON.shtml";	//LED1��,BEEP��
	else if(LED1 == 1 && BEEP == 1) return "/STM32F407LED_OFF_BEEP_ON.shtml";	//LED1��,BEEP��
	else return "/STM32F407LED_OFF_BEEP_OFF.shtml";   //LED1��,BEEP��
	
}

//SSI�����ʼ��
void httpd_ssi_init(void)
{  
	//�����ڲ��¶ȴ�������SSI���
	http_set_ssi_handler(SSIHandler,ppcTAGs,NUM_CONFIG_SSI_TAGS);
}

//CGI�����ʼ��
void httpd_cgi_init(void)
{ 
  //����CGI���LEDs control CGI) */
  http_set_cgi_handlers(ppcURLs, NUM_CONFIG_CGI_URIS);
}


