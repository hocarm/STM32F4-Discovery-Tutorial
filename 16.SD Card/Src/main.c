#include "stm32f4xx_hal.h"
#include "fatfs.h"
#include "string.h"

FATFS   fatfs; 		/* FAT File System */
FRESULT fresult;  /* FAT File Result */
FIL			myfile;		/* FILE Instance */

uint8_t write_data[]="Hello!\nThis is my friend.",byte_written=0;
uint8_t read_data[100],byte_read;


SD_HandleTypeDef hsd;
HAL_SD_CardInfoTypedef SDCardInfo;






void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_SDIO_SD_Init(void);

float a=0.001567;

uint8_t buffer[20]="hjhjkl";


int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_SDIO_SD_Init();
	MX_FATFS_Init();

	if(BSP_SD_Init()==MSD_OK)
	{
		fresult=f_mount(&fatfs,"",1);
		
		/* Create File with name "stm32.txt" and file access is write */
//		fresult=f_open(&myfile,"stm32.txt",FA_CREATE_ALWAYS|FA_WRITE);
		
//		byte_written=f_printf(&myfile,"%s",write_data);
//		fresult=f_close(&myfile);
		
		
		/* Open file with file name "stm32.txt" and file access is write*/
//		fresult=f_open(&myfile,"stm32.txt",FA_WRITE);
		
		/* Go to the end of file */
//		f_lseek(&myfile,f_size(&myfile));
//    f_printf(&myfile,"Osiris");
		
		/* NOTE: f_printf lacks floating point support */
		/* If want to print float number use sprintf, then use f_printf */
//		sprintf((char*)buffer,"%0.6f",a);
//		f_printf(&myfile,"%s",buffer);
//		f_close(&myfile);
		
		/* Open file with file name "stm32.txt" and file access is read */
//			fresult=f_open(&myfile,"stm32.txt",FA_READ);
		/* Read all of file */
//		  fresult=f_read(&myfile,read_data,f_size(&myfile),(UINT*)&byte_read);
//			fresult=f_close(&myfile);
		
		/* Delete the file */
//		fresult=f_unlink("stm32.txt");
	}
	
	
	
	
	
	
	
	
	while (1)
	{

	}
}


void SystemClock_Config(void)
{

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	__HAL_RCC_PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
															|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* SDIO init function */
static void MX_SDIO_SD_Init(void)
{

	hsd.Instance = SDIO;
	hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
	hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
	hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
	hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
	hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
	hsd.Init.ClockDiv = 0;

}

/** Configure pins as 
				* Analog 
				* Input 
				* Output
				* EVENT_OUT
				* EXTI
*/
static void MX_GPIO_Init(void)
{

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
	* @brief  This function is executed in case of error occurrence.
	* @param  None
	* @retval None
	*/
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler */
	/* User can add his own implementation to report the HAL error return state */
	while(1) 
	{
	}
	/* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
	 * @brief Reports the name of the source file and the source line number
	 * where the assert_param error has occurred.
	 * @param file: pointer to the source file name
	 * @param line: assert_param error line source number
	 * @retval None
	 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
		ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */

}

#endif

/**
	* @}
	*/ 

/**
	* @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
