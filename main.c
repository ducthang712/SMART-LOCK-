/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c-lcd.h"
#include "stdio.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


#define R1 GPIO_PIN_1
#define R2 GPIO_PIN_2
#define R3 GPIO_PIN_3
#define R4 GPIO_PIN_4
#define C1 GPIO_PIN_5
#define C2 GPIO_PIN_6
#define C3 GPIO_PIN_7

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//movement
	int move =0;

//set up 
	char save[11] = {'*',' ','*',' ','*',' ','*',' ',' ',' ',' '} ;	
	char pass[8] = {'1',' ','2',' ','3',' ','4',' '};
	
	int x=0;
	int count = 0;
	int check = 0;

//SETTINGS
int screen=0;

//DELAY
void delay_1ms(void){
	__HAL_TIM_SetCounter(&htim2,0);
	while(__HAL_TIM_GetCounter(&htim2) < 10);
}
void delay_ms(int time)
{
 int i = 0;
 for(i = 0; i < time; i++)
 {
   delay_1ms();
 }
}

//KEYPAD
char read_keypad (void)
{
	//ROW 1
	HAL_GPIO_WritePin(GPIOA, R1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, R2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, R3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, R4, GPIO_PIN_SET);
	
	if (!(HAL_GPIO_ReadPin (GPIOA, C1)))
		return '1';
	if (!(HAL_GPIO_ReadPin (GPIOA, C2)))
		return '2';
	if (!(HAL_GPIO_ReadPin (GPIOA, C3)))
		return '3';
	
	//ROW 2
	HAL_GPIO_WritePin(GPIOA, R1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, R2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, R3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, R4, GPIO_PIN_SET);
	
	if (!(HAL_GPIO_ReadPin (GPIOA, C1)))
		return '4';
	if (!(HAL_GPIO_ReadPin (GPIOA, C2)))
		return '5';
	if (!(HAL_GPIO_ReadPin (GPIOA, C3)))
		return '6';
	
	//ROW 3
	HAL_GPIO_WritePin(GPIOA, R1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, R2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, R3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, R4, GPIO_PIN_SET);
	
	if (!(HAL_GPIO_ReadPin (GPIOA, C1)))
		return '7';
	if (!(HAL_GPIO_ReadPin (GPIOA, C2)))
		return '8';
	if (!(HAL_GPIO_ReadPin (GPIOA, C3)))
		return '9';
	
	//ROW 4
	HAL_GPIO_WritePin(GPIOA, R1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, R2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, R3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, R4, GPIO_PIN_RESET);
	
	if (!(HAL_GPIO_ReadPin (GPIOA, C1)))
		return '*';
	if (!(HAL_GPIO_ReadPin (GPIOA, C2)))
		return '0';
	if (!(HAL_GPIO_ReadPin (GPIOA, C3)))
		return '#';
	
	
	return ' ';
}
uint8_t key;

//Wait for closing
uint8_t door_check(void)
{
	return HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11) == GPIO_PIN_SET;
	//while normal IR sensor == 1
}

//SCREEN DISPLAY
void display(void)
{
	screen++;
	if (screen == 4) screen =0;
}

//Door unlock
void unlock(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
	delay_ms(3000);
	CHECK: while (door_check()==1)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
	}
	delay_ms(2000);
	if (door_check()==0)
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
	else goto CHECK;
	
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start(&htim2);
	

	
//ADC BATTERY
	uint32_t adc_value =0;
	int nguyen =0;
	int thap_phan = 0;
	char battery_value[100];
	int check_batt ;

	lcd_init();
	
	//PROJECT PRESENTATION
	lcd_clear();
	delay_ms(1000);
	lcd_put_cur(0,4);
	lcd_send_string("PROJECT 1");
	lcd_put_cur(1,3);
	lcd_send_string("SMART LOCK");
	delay_ms(3000);
	lcd_clear();
	lcd_put_cur(0,1 );
	lcd_send_string("SAVING MODE...");
	delay_ms(1000);

	cursor();
	int count =0;
	
	//check password to weighting
	int correct =0;
	
	
	//PASSWORD
	lcd_clear();
	delay_ms(1000);
	lcd_put_cur(0,1);
	lcd_send_string("ENTER PASSWORD ");
	lcd_put_cur(1,5);
	lcd_send_string(save);
	
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		//wait for human detecting
		/*while (correct == 2)
		{
			if((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13) == GPIO_PIN_RESET))
			{
				delay_ms(2000);
				if ((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13) == GPIO_PIN_RESET))
				{
					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET);
					delay_ms(1000);
					lcd_init();
					
					//PASSWORD
					lcd_clear();
					delay_ms(1000);
					lcd_put_cur(0,1);
					lcd_send_string("ENTER PASSWORD ");
					lcd_put_cur(1,5);
					lcd_send_string(save);	
					
					correct =0;
				}
			}
		}*/
//START CHECKING PASSWORD
		while(correct == 0)
			{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
			
				
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);		
		if (read_keypad() != ' ')
			{
				//save

				
				key = read_keypad();
				if ((key != ' ' )&&(key != '*'))
					if (x <8)
						{
							save[x] = key;
							x += 2;
						}
					//CHECK CORRECT
					if (x >= 8)
						{
							for (int i=0; i<7; i+=2)
								{
									switch (i)
									{
										case 0: if(save[i] == pass[0]) count++;
										case 2: if(save[i] == pass[2]) count++;
										case 4: if(save[i] == pass[4]) count++;
										case 6: if(save[i] == pass[6]) count++;
										break;
									}
								}
							if (count == 4)
							{
								HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
								check = 1;
							}
							else 
								{
									count =0;
								}
						}
					
							
					if (key == '*')
						{
							for (int i =0;i<7;i+=2)
							{
								save[i] = '*';
								save[i+1] = ' ';
							}
							count = 0;
							x = 0;
						}
				//LCD
					//PASSWORD CORRECT
					if (check == 1)
						{
							lcd_put_cur(1,5);
							lcd_send_string(save);
							delay_ms(300);
							
							for (int i=0; i<2;i++)
							{
								lcd_clear();
								delay_ms(200);
								lcd_put_cur(0,3);
								lcd_send_string("PASSWORD");
								lcd_put_cur(1,6);
								lcd_send_string("CORRECT");
								delay_ms(200);
							}
							
							unlock();
						
							
							delay_ms(2000);
							lcd_clear();
							delay_ms(100);
							lcd_put_cur(0,3);
							lcd_send_string("LOADING...");
							delay_ms(2000);
							lcd_clear();
							delay_ms(200);
				
							correct = 1;
		

							//reset pass
/*							count = 0;
							check = 0;
							x = 0;
							
							
							for (int i =0;i<7;i+=2)
							{
								save[i] = '*';
								save[i+1] = ' ';
							}
							
							
							lcd_put_cur(0,1);
							lcd_send_string("ENTER PASSWORD");
							lcd_put_cur(1,5);
							lcd_send_string(save);
							delay_ms(200);
*/					
						}
						
					//PASSWORD WRONG

					//FILLING PASSWORD
					else 
						{
								lcd_put_cur(1,5);
								lcd_send_string(save);
								delay_ms(200);
						}	
			}
		}
			
while (correct ==1)
{
	//FUNCTION 1
	lcd_put_cur(0,1);
	lcd_send_string("BATTERY POWER");
	
	//FUNTION 2
	lcd_put_cur(1,1);
	lcd_send_string("CHANGE PASS");
	
	//CURSOR
	lcd_put_cur(move,0);
	lcd_send_data(0);
	
	//MOVE UP
	if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3) == GPIO_PIN_RESET)
	{
		lcd_put_cur(1,0);
		lcd_send_string(" ");
		move=0;
	}
	//MOVE DOWN
	if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4) == GPIO_PIN_RESET)
	{
		lcd_put_cur(0,0);
		lcd_send_string(" ");
		move =1;
	}
	//CHOOSE 
	if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5) == GPIO_PIN_RESET)
	{
		//FUNC 1: BATTERY POWER
		switch(move)
		{
			case 0: 
				{
					lcd_clear();
					while (move ==0)
						{
							//ADC - BATTERY
							HAL_ADC_Start(&hadc1);
							delay_ms(50);
							adc_value = HAL_ADC_GetValue(&hadc1);
							HAL_ADC_Stop(&hadc1);
							
							check_batt = adc_value;
							adc_value= 0.77*(adc_value);
							nguyen = adc_value /1000;
							thap_phan = adc_value % 1000;
							thap_phan = thap_phan /10;
							sprintf( battery_value, "%d %d",nguyen, thap_phan);
					//  sprintf( battery_value,"%d" ,check_batt);
							
							//DISPLAY BATTERY
							lcd_put_cur(0,0);
							lcd_send_string("BATTERY: ");
							lcd_put_cur(1,0);
							lcd_send_string("HEALTH: ");
							lcd_put_cur(0,10);
							lcd_send_string(battery_value);
							lcd_put_cur(0,11);
							lcd_send_string(".");
							lcd_put_cur(0,14);
							lcd_send_string("V");
							
							delay_ms(200);
						}
				}
			case 1:
				{
					lcd_clear();
					lcd_put_cur(0,0);
					lcd_send_string(" INTO CHANGING");
					delay_ms(3000);
					lcd_clear();
					break;
				}
			}
		
		//FUNC 2: CHANGE PASSWORD
//		if(move==0)
	}

}
/*		
	while (move ==1)
		{
			//ADC - BATTERY
			HAL_ADC_Start(&hadc1);
			delay_ms(50);
			adc_value = HAL_ADC_GetValue(&hadc1);
			HAL_ADC_Stop(&hadc1);
			
			check_batt = adc_value;
			adc_value= 0.77*(adc_value);
			nguyen = adc_value /1000;
			thap_phan = adc_value % 1000;
			thap_phan = thap_phan /10;
			sprintf( battery_value, "%d %d",nguyen, thap_phan);
	//      		sprintf( battery_value,"%d" ,check_batt);
			
			//DISPLAY BATTERY
			lcd_put_cur(0,0);
			lcd_send_string("BATTERY: ");
			lcd_put_cur(1,0);
			lcd_send_string("HEALTH: ");
			lcd_put_cur(0,10);
			lcd_send_string(battery_value);
			lcd_put_cur(0,11);
			lcd_send_string(".");
			lcd_put_cur(0,14);
			lcd_send_string("V");
			
			delay_ms(200);
		}
*/
	}
}
  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */



// BUTTON INTERUPT INSIDE
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == (GPIO_PIN_0))
	{
		unlock();
	}	
	
	if (GPIO_Pin == (GPIO_PIN_1))
	{
		display();
	}
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
	HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn);
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
	HAL_NVIC_ClearPendingIRQ(EXTI1_IRQn);
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
