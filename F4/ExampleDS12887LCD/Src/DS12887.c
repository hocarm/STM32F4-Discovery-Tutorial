/**
  ******************************************************************************
  * @file		DS12887.h
  * @author	cuongabba
  * @date		6 February 2018
	* @note		Re-write form Peter Fleury DS12887 library
  ******************************************************************************
  */

/** Includes ---------------------------------------------------------------- */
#include "DS12887.h"


GPIO_InitTypeDef GPIO_InitStructDS;

void configpin_GPIO(void)
{
	// GPIO clock for control and data lines
	__HAL_RCC_GPIOB_CLK_ENABLE(); // enable port connect DS12887
	// Configure I/O for control lines as output
	GPIO_InitStructDS.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 |
	GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6| GPIO_PIN_7|GPIO_PIN_8|
	GPIO_PIN_9|GPIO_PIN_10| GPIO_PIN_11;
	GPIO_InitStructDS.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructDS.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructDS); // enable
}
// port write||
void Write_PortData(unsigned char dat)
{

  if(dat&0x80) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11,GPIO_PIN_SET); else HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11,GPIO_PIN_RESET); 
  if(dat&0x40) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_SET); else HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_RESET); 
  if(dat&0x20) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,GPIO_PIN_SET); else HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,GPIO_PIN_RESET); 
  if(dat&0x10) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,GPIO_PIN_SET); else HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,GPIO_PIN_RESET); 
  if(dat&0x08) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_SET); else HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_RESET); 
  if(dat&0x04) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,GPIO_PIN_SET); else HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,GPIO_PIN_RESET); 
  if(dat&0x02) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,GPIO_PIN_SET); else HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,GPIO_PIN_RESET); 
  if(dat&0x01) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,GPIO_PIN_SET); else HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,GPIO_PIN_RESET); 
  
}

// READ port

unsigned char Read_PortData(void)
{
  unsigned char dat;
  dat=0;
  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)) dat|=0x80; 
  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)) dat|=0x40;
  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9)) dat|=0x20;
  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8)) dat|=0x10;
  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)) dat|=0x08;
  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)) dat|=0x04;
  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5)) dat|=0x02;
  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)) dat|=0x01; 
  return(dat);
}

//================= Write ds12887==============
void DS12C887_Write(unsigned char Regaddr, unsigned char dat)   // wait 
{                                           
  //----- address in-----------------
  //CSO=1;  
  Write_PortData(0x00);   
  WR_HIGH();         //WR=1;
  RD_HIGH();         //RD=1;
  ALE_HIGH();        //ALE=1;
  DS12887_CS_LOW();  //CST=0;	
  Write_PortData(Regaddr);                         
  ALE_LOW();         //ALE=0;   
  WR_LOW();          //WR=0;	
  //----- data out-----------------------
  Write_PortData(dat);                                                                           
  WR_HIGH();         //WR=1;
  ALE_HIGH();        //ALE=1;      
  DS12887_CS_HIGH(); //CST=1;   
} 
//================= Init ds12887==============

void DS12C887_Init(void)
{
	DS12C887_Write(REGA,0xAF);
	DS12C887_Write(REGB,0x06); //Chon che do 24h va ma BIN
	DS12C887_Write(REGC,0x00);
}


//================= Read ds12887==============
unsigned char DS12C887_Read(unsigned char Regaddr)
{
  unsigned char dat;
  unsigned char i;
  //-----address close-----------------------
  //CSO=1; 
  Write_PortData(0x00);	
  
  WR_HIGH();         //WR=1;
  RD_HIGH();         //RD=1;
  ALE_HIGH();        //ALE=1;
	DS12887_CS_LOW();  //CST=0;	
  Write_PortData(Regaddr);           
  //for(i=0;i<10;i++);
  ALE_LOW();         //ALE=0;       
  Write_PortData(0xFF);
  //-----data in-----------
  RD_LOW();          //RD=0;
  for(i=0;i<10;i++);
  dat=Read_PortData();
  RD_HIGH();         //RD=1;
  DS12887_CS_HIGH(); //CST=1; 	
  ALE_HIGH();        //ALE=1;    
        
  return(dat);
}  

//------------------set time & date  -----------------------

void DS12C887_SetTime(unsigned char hour,unsigned char minute,unsigned char second)
{
	
	DS12C887_Write(DS12C887_HOUR,hour);
	DS12C887_Write(DS12C887_MINUTE,minute);
	DS12C887_Write(DS12C887_SECOND,second);
}
void DS12C887_SetDate(unsigned char day,unsigned char date,unsigned char month,unsigned char year)
{
	DS12C887_Write(DS12C887_DAY,day);
	DS12C887_Write(DS12C887_DATE,date);
	DS12C887_Write(DS12C887_MONTH,month);
	DS12C887_Write(DS12C887_YEAR,year);
}

//--------------------- Get time & date ---------------------------------

void DS12C887_GetDate(unsigned char* DayWeek,unsigned char* DayMonth,unsigned char* Month,unsigned char* Year)
{
#ifdef DO_WHILE
	unsigned char Temp;
	do
	{
	 Temp=DS12C887_Read(0x0A);
	}while((Temp&0x80)!=0x80);
	*DayWeek=DS12C887_Read(0x06);
	do
	{
	    Temp=DS12C887_Read(0x0A);
	}while((Temp&0x80)!=0x80);
	*DayMonth=DS12C887_Read(0x07);
	do
	{
	    Temp=DS12887_Read(0x0A);
	}while((Temp&0x80)!=0x80);
	*Month=DS12887_Read(0x08);
	do
	{
	    Temp=DS12C887_Read(0x0A);
	}while((Temp&0x80)!=0x80);
	*Year=DS12887_Read(0x09);
#else
	*DayWeek=DS12C887_Read(0x06);
	*DayMonth=DS12C887_Read(0x07);
	*Month=DS12C887_Read(0x08);
	*Year=DS12C887_Read(0x09);
#endif
}

void DS12C887_GetTime(unsigned char* Hour,unsigned char* Minute,unsigned char* Second)
{
#ifdef DO_WHILE
	unsigned char Temp;
	do
	{
	Temp=DS12C887_Read(0x0A);
	}while((Temp&0x80)!=0x80);
	*Hour=DS12C887_Read(0x04);
	do
	{
	    Temp=DS12C887_Read(0x0A);
	}while((Temp&0x80)!=0x80);
	*Minute=DS12887_Read(0x02);
	do
	{
	    Temp=DS12887_Read(0x0A);
	}while((Temp&0x80)!=0x80);
	*Second=DS12C887_Read(0x00);
#else
	*Hour=DS12C887_Read(0x04);
	*Minute=DS12C887_Read(0x02);
	*Second=DS12C887_Read(0x00);
#endif
}
	
