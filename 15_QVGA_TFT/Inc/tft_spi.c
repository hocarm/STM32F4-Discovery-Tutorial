#include "tft_spi.h"
#include "tft_font.h"

uint16_t BACK_COLOR, POINT_COLOR;



/***************************************************** User Function ************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/

void tft_init(void)
{

	HAL_GPIO_WritePin(TFT_RESET_PORT,TFT_RESET_PIN,GPIO_PIN_SET);
  HAL_Delay(5);	
	HAL_GPIO_WritePin(TFT_RESET_PORT,TFT_RESET_PIN,GPIO_PIN_RESET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(TFT_RESET_PORT,TFT_RESET_PIN,GPIO_PIN_SET);

	SPI_ENABLE;
	
	tft_write_register(0xCB);  
  tft_write_data_8bit(0x39); 
  tft_write_data_8bit(0x2C); 
  tft_write_data_8bit(0x00); 
  tft_write_data_8bit(0x34); 
  tft_write_data_8bit(0x02); 

        tft_write_register(0xCF);  
        tft_write_data_8bit(0x00); 
        tft_write_data_8bit(0XC1); 
        tft_write_data_8bit(0X30); 
 
        tft_write_register(0xE8);  
        tft_write_data_8bit(0x85); 
        tft_write_data_8bit(0x00); 
        tft_write_data_8bit(0x78); 
 
        tft_write_register(0xEA);  
        tft_write_data_8bit(0x00); 
        tft_write_data_8bit(0x00); 
 
        tft_write_register(0xED);  
        tft_write_data_8bit(0x64); 
        tft_write_data_8bit(0x03); 
        tft_write_data_8bit(0X12); 
        tft_write_data_8bit(0X81); 

        tft_write_register(0xF7);  
        tft_write_data_8bit(0x20); 
  
        tft_write_register(0xC0);    
        tft_write_data_8bit(0x23);   
 
        tft_write_register(0xC1);   
        tft_write_data_8bit(0x10);   
 
        tft_write_register(0xC5);    
        tft_write_data_8bit(0x3e); 
        tft_write_data_8bit(0x28); 
 
        tft_write_register(0xC7);    
        tft_write_data_8bit(0x86);  
 
        tft_write_register(0x36);    
        tft_write_data_8bit(0x48); 

        tft_write_register(0x3A);    
        tft_write_data_8bit(0x55); 

        tft_write_register(0xB1);    
        tft_write_data_8bit(0x00);  
        tft_write_data_8bit(0x18); 
 
        tft_write_register(0xB6);    
        tft_write_data_8bit(0x08); 
        tft_write_data_8bit(0x82);
        tft_write_data_8bit(0x27);  
 
        tft_write_register(0xF2);    
        tft_write_data_8bit(0x00); 
 
        tft_write_register(0x26);    
        tft_write_data_8bit(0x01); 
 
        tft_write_register(0xE0);    
        tft_write_data_8bit(0x0F); 
        tft_write_data_8bit(0x31); 
        tft_write_data_8bit(0x2B); 
        tft_write_data_8bit(0x0C); 
        tft_write_data_8bit(0x0E); 
        tft_write_data_8bit(0x08); 
        tft_write_data_8bit(0x4E); 
        tft_write_data_8bit(0xF1); 
        tft_write_data_8bit(0x37); 
        tft_write_data_8bit(0x07); 
        tft_write_data_8bit(0x10); 
        tft_write_data_8bit(0x03); 
        tft_write_data_8bit(0x0E); 
        tft_write_data_8bit(0x09); 
        tft_write_data_8bit(0x00); 

        tft_write_register(0XE1);     
        tft_write_data_8bit(0x00); 
        tft_write_data_8bit(0x0E); 
        tft_write_data_8bit(0x14); 
        tft_write_data_8bit(0x03); 
        tft_write_data_8bit(0x11); 
        tft_write_data_8bit(0x07); 
        tft_write_data_8bit(0x31); 
        tft_write_data_8bit(0xC1); 
        tft_write_data_8bit(0x48); 
        tft_write_data_8bit(0x08); 
        tft_write_data_8bit(0x0F); 
        tft_write_data_8bit(0x0C); 
        tft_write_data_8bit(0x31); 
        tft_write_data_8bit(0x36); 
        tft_write_data_8bit(0x0F); 
 
        tft_write_register(0x11);    
        HAL_Delay(120); 
				
        tft_write_register(0x29);    
        tft_write_register(0x2c); 
				
				
				SPI_DISABLE;
}

void tft_puts8x16(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE)
{
	uint8_t i=0;
	SPI_ENABLE;
	  while(*(string+i)!='\0')
    {       
        if(column>LCD_W-16){column=0;row+=16;}
        if(row>LCD_H-16){row=column=0;}
        tft_putchar(column,row,*(string+i),TFT_STRING_MODE);
        column+=8;
        i++;
    }  
	SPI_DISABLE;
}

void tft_puts14x24(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE)
{
		uint8_t i=0;
	SPI_ENABLE;
	  while(*(string+i)!='\0')
    {       
				tft_putchar14x24(column,row,*(string+i),TFT_STRING_MODE);
        column+=14;
        i++;
    }  
	SPI_DISABLE;
}


void tft_puts18x32(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE)
{
			uint8_t i=0;
	SPI_ENABLE;
	  while(*(string+i)!='\0')
    {       
				tft_putchar18x32(column,row,*(string+i),TFT_STRING_MODE);
        column+=18;
        i++;
    }  
	SPI_DISABLE;
}
void tft_puts26x48(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE)
{
	uint8_t i=0;
	SPI_ENABLE;
	  while(*(string+i)!='\0')
    {       
				tft_putchar26x48(column,row,*(string+i),TFT_STRING_MODE);
        column+=26;
        i++;
    }  
	SPI_DISABLE;
}
void tft_clear(uint16_t Color)
{
	uint8_t VH,VL;
	uint16_t i,j;
	VH=Color >> 8;
	VL=Color & 0x00ff;
	
	SPI_ENABLE;
	
	tft_add_set(0,0,LCD_W-1,LCD_H-1);
  for(i=0;i<LCD_W;i++)
	{
	  for (j=0;j<LCD_H;j++)
	  {
       tft_write_data_8bit(VH);
			 tft_write_data_8bit(VL);	
	  }
	}
	
	SPI_DISABLE;
}

void tft_draw_line(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=column2-column1; 
	delta_y=row2-row1; 
	uRow=column1; 
	uCol=row1; 
	if(delta_x>0)incx=1; 
	else if(delta_x==0)incx=0;
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; 
	else distance=delta_y; 
	SPI_ENABLE;
	for(t=0;t<=distance+1;t++ )
	{  
		tft_draw_point(uRow,uCol);
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	} 
	SPI_DISABLE;	
}

void tft_draw_rectangle(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2)
{
	SPI_ENABLE;
	tft_draw_line(row1,column1,row1,column2);
	tft_draw_line(row1,column1,row2,column1);
	tft_draw_line(row2,column1,row2,column2);
	tft_draw_line(row2,column2,row1,column2);
	SPI_DISABLE;
}

void tft_draw_circle(uint16_t row,uint16_t column,uint8_t radian)
{
	int a,b;
	int di;
	a=0;b=radian;	  
	di=3-(radian<<1);
	SPI_ENABLE;
	while(a<=b)
	{
		tft_draw_point(column-b,row-a);                     
		tft_draw_point(column+b,row-a);                       
		tft_draw_point(column-a,row+b);                 
		tft_draw_point(column-b,row-a);                    
		tft_draw_point(column-a,row-b);                       
		tft_draw_point(column+b,row+a);                        
		tft_draw_point(column+a,row-b);             
		tft_draw_point(column+a,row+b);             
		tft_draw_point(column-b,row+a);             
		a++;
		    
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		tft_draw_point(column+a,row+b);
	}
	SPI_DISABLE;
}

void tft_fill(uint16_t row1,uint16_t column1,uint16_t row2,uint16_t column2,uint16_t color)
{
	uint16_t i,j; 
	SPI_ENABLE;
	tft_add_set(column1,row1,column2,row2);   
	for(i=row1;i<=row2;i++)
	{													   	 	
		for(j=column1;j<=column2;j++)
		{
			tft_write_data(color);
		}
	} 	
	SPI_DISABLE;
}

void tft_puts_image(const unsigned char* image_arr)
{
	uint32_t i;
	uint16_t temp;
	uint8_t high_byte,low_byte;
	
	SPI_ENABLE;
	tft_add_set(0,0,240-1,320-1);

	for(i=0;i<(240*320);i++)
	{ 
		low_byte=*(image_arr+i*2);
		high_byte=*(image_arr+i*2+1);
		temp=((high_byte)<<8|(low_byte));
		tft_write_data(temp);
	}	
	
	SPI_DISABLE;
}

/******************************************************** Private Function ******************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/

void tft_write_data_8bit(uint8_t data)
{
	uint8_t send_data =data;
	HAL_GPIO_WritePin(DC_PORT,DC_PIN,GPIO_PIN_SET);
	HAL_SPI_Transmit(&SPI_HANDLE_TYPE_DEF,&send_data,1,500);
}

void tft_write_data(uint16_t data)
{
	uint8_t send_data[2];
	send_data[0]=data >> 8;
	send_data[1]=data & 0x00ff;

	HAL_GPIO_WritePin(DC_PORT,DC_PIN,GPIO_PIN_SET);
	HAL_SPI_Transmit(&SPI_HANDLE_TYPE_DEF,send_data,2,500);
}

void tft_write_register(int8_t data)
{
	uint8_t register_value=data;
	HAL_GPIO_WritePin(DC_PORT,DC_PIN,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&SPI_HANDLE_TYPE_DEF,&register_value,1,500);
}


void tft_add_set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	 tft_write_register(0x2a);
   tft_write_data(x1);
   tft_write_data(x2);
  
   tft_write_register(0x2b);
   tft_write_data(y1);
   tft_write_data(y2);

   tft_write_register(0x2C);
}

void tft_draw_point(uint16_t x,uint16_t y)
{
	tft_add_set(x,y,x,y);
	tft_write_data(POINT_COLOR); 	
}

void tft_draw_point_big(uint16_t x,uint16_t y)
{
		tft_fill(x-1,y-1,x+1,y+1,POINT_COLOR);
}


void tft_putchar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode)
{
	uint8_t temp;
  uint8_t pos,t;
	//uint16_t x0=x;
	uint16_t colortemp=POINT_COLOR;      
  if(x>LCD_W-16||y>LCD_H-16)return;	    		   
	num=num-' ';
	
	tft_add_set(x,y,x+8-1,y+16-1);      
	if(!mode) 
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(uint16_t)num*16+pos];		 
			for(t=0;t<8;t++)
		  {                 
		        if(temp&0x01)POINT_COLOR=colortemp;
						else POINT_COLOR=BACK_COLOR;
						tft_write_data(POINT_COLOR);	
						temp>>=1; 
		  }
		}	
	}
	else
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(uint16_t)num*16+pos];		 
				for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)tft_draw_point(x+t,y+pos);
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	
}

//x is column
//y is row
void tft_putchar26x48(uint16_t x,uint16_t y,uint8_t data,uint8_t mode)
{
	if((x>LCD_H-48)||(y>LCD_W-26)) return;
	uint8_t i,j,k,temp;
	
	tft_add_set(x,y,x+26-1,y+48-1); 
	
	for(i=0;i<48/8;i++)
	{
		for(j=0;j<8;j++)
		{
			for(k=0;k<26;k++)
			{
				temp=Consolas26x48[(data-' ')*(48/8)*26+k*(48/8)+i];
				if(mode==TFT_STRING_MODE_BACKGROUND)
				{
					if(temp&(0x01<<j))
					{
						tft_write_data(POINT_COLOR);
					}
					else
					{
						tft_write_data(BACK_COLOR);
					}
				}
				else
				{
					if(temp&(0x01<<j))
					{
						tft_draw_point(x+k,y+(8*i+j));
					}
				}
			}
		}
	}
}



void tft_putchar14x24(uint16_t x,uint16_t y,uint8_t data,uint8_t mode)
{
	if((x>LCD_H-24)||(y>LCD_W-14)) return;
	uint8_t i,j,k,temp;

	tft_add_set(x,y,x+14-1,y+24-1); 
	
	for(i=0;i<24/8;i++)
	{
		for(j=0;j<8;j++)
		{
			for(k=0;k<14;k++)
			{
				temp=Consolas14x24[(data-' ')*(24/8)*14+k*(24/8)+i];
				if(mode==TFT_STRING_MODE_BACKGROUND)
				{
					if(temp&(0x01<<j))
					{
						tft_write_data(POINT_COLOR);
					}
					else
					{
						tft_write_data(BACK_COLOR);
					}
				}
				else
				{
					if(temp&(0x01<<j))
					{
						tft_draw_point(x+k,y+(8*i+j));
					}
				}
			}
		}
	}
}



void tft_putchar18x32(uint16_t x,uint16_t y,uint8_t data,uint8_t mode)
{
	if((x>LCD_H-32)||(y>LCD_W-18)) return;
	uint8_t i,j,k,temp;

	tft_add_set(x,y,x+18-1,y+32-1); 
	
	for(i=0;i<32/8;i++)
	{
		for(j=0;j<8;j++)
		{
			for(k=0;k<18;k++)
			{
				temp=Consolas18x32[(data-' ')*(32/8)*18+k*(32/8)+i];
				if(mode==TFT_STRING_MODE_BACKGROUND)
				{
					if(temp&(0x01<<j))
					{
						tft_write_data(POINT_COLOR);
					}
					else
					{
						tft_write_data(BACK_COLOR);
					}
				}
				else
				{
					if(temp&(0x01<<j))
					{
						tft_draw_point(x+k,y+(8*i+j));
					}
				}
			}
		}
	}
}
























