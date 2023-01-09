/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "los_base.h"
#include "los_sys.h"
#include "los_typedef.h"
#include "los_task.h"
#include "stdio.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
UINT32 Main_Task_ID;
UINT32 On_Flashing_LED_ID;
UINT32 On_All_LED_ID;
UINT32 On_Get_Tempr_ID;
UINT32 On_Tim2_Task_ID;

uint16_t adcValue = 0;
float voltage = 0;
int isOpenTim2 = 1;
/* USER CODE END PV */

void SystemClock_Config(void);

/* USER CODE BEGIN 0 */

/*****************************************************************************
 Author : 郑人滏
 Function : Create_LOS_Task
 Description : 创建 LOS 任务
 Return      : 当任务创建成功时返回 LOS_OK，创建失败时返回 LOS_NOK
 *****************************************************************************/
UINT32 Create_LOS_Task(void (*Task_Func)(), char *Task_Name, UINT32 *Task_ID) {
	UINT32 uwRet = LOS_OK;
	
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.pcName = Task_Name;
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC) Task_Func;
	task_init_param.usTaskPrio = 0;
	task_init_param.uwStackSize = 0x200;
	
	uwRet = LOS_TaskCreate(Task_ID, &task_init_param);
	if (LOS_OK != uwRet)
	{
		printf("[Error: 任务 %s 创建失败！]\n", Task_Name);
		return LOS_NOK;
	}
	printf("[Success: 任务 %s 创建成功！]\n", Task_Name);
	return uwRet;
}

/*****************************************************************************
 Author : 郑人滏
 Function : Delete_LOS_Task
 Description : 删除 LOS 任务
 Return      : 当任务删除成功或失败时提示消息
 *****************************************************************************/
void Delete_LOS_Task(char *Task_Name, UINT32 Task_ID)
{
	UINT32 uwRet = LOS_TaskDelete(Task_ID);
	if (uwRet == LOS_OK)
	{
		printf("[Success: 任务 %s 删除成功！]\n", Task_Name);
	}
	else
	{
		printf("[Error: 任务 %s 删除失败！]\n", Task_Name);
	}
}

/*****************************************************************************
 Author : 郑人滏
 Function : On_Flashing_LED_Task
 Description : 绿灯每隔1.5秒闪烁1次，闪烁20次后，停止闪烁
 *****************************************************************************/
void On_Flashing_LED_Task()
{
	int count = 0;
	while (count < 20)
	{
		HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
		LOS_TaskDelay(1500);
		HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);
		LOS_TaskDelay(1500);
		printf("[Tips: 绿灯闪烁中...，第 %d 次]\n", count + 1);
		count++;
	}
	HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);
	Delete_LOS_Task("On_Flashing_LED_Task", On_Flashing_LED_ID);
}

/*****************************************************************************
 Author : 郑人滏
 Function : On_All_LED_Task
 Description : 所有灯全亮
 *****************************************************************************/
void On_All_LED_Task()
{
	HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
}

/*****************************************************************************
 Author : 郑人滏
 Function : On_Get_Tempr_Task
 Description : 获取热敏电阻值，并打印，蓝灯长亮
 *****************************************************************************/
void On_Get_Tempr_Task()
{
	HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_RESET);
	while (1)
	{
		voltage = adcValue * 3.3 / 4096;
		printf("adcValue: %d, voltage: %f \n", adcValue, voltage);
		LOS_TaskDelay(1000);
	}
}

/*****************************************************************************
 Author : 郑人滏
 Function : On_Tim2_Task
 Description : 定时器开启之后，执行该任务
 *****************************************************************************/
void On_Tim2_Task()
{
	printf("[Tips: Tim2 定时器执行中，红灯闪烁...]\n");
	HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
	LOS_TaskDelay(1000);
	HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
	LOS_TaskDelay(1000);
	isOpenTim2 = 0;
}

/*****************************************************************************
 Author : 郑人滏
 Function : HAL_TIM_PeriodElapsedCallback
 Description : TIM2 中断函数
 *****************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (isOpenTim2 == 0)
	{
		isOpenTim2 = 1;
		Create_LOS_Task(On_Tim2_Task, "On_Tim2_Task", &On_Tim2_Task_ID);
	}
}

/*****************************************************************************
 Author : 郑人滏
 Function : Main_Task
 Description : 主任务
 *****************************************************************************/
void Main_Task()
{
	// 指令字符
	char command[40];

	while(1)
	{
		printf("[Tips: 请输入控制命令：]\n");
		scanf("%s", command);
		printf("\n");

		if (strcmp(command, "timon") == 0)
		{
			printf("[Summary: 开启定时器 TIM2，通过定时器实现红灯周期闪烁]\n");
			isOpenTim2 = 0;
			HAL_TIM_Base_Start_IT(&htim2);
		}
		else if (strcmp(command, "timoff") == 0)
		{
			printf("[Summary: 关闭定时器 TIM2，红灯熄灭]\n");
			HAL_TIM_Base_Stop_IT(&htim2);
			HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
		}
		else if (strcmp(command, "ledflashingon") == 0)
		{
			printf("[Summary: 绿灯每隔1.5秒闪烁1次，闪烁20次后，停止闪烁]\n");
			Create_LOS_Task(On_Flashing_LED_Task, "On_Flashing_LED_Task", &On_Flashing_LED_ID);
		}
		else if (strcmp(command, "ledflashingoff") == 0)
		{
			printf("[Summary: 绿灯闪烁直接停止]\n");
			Delete_LOS_Task("On_Flashing_LED_Task", On_Flashing_LED_ID);
		}
		else if (strcmp(command, "gettempron") == 0)
		{
			printf("[Summary: 通过热敏电阻获取环境温度，同时蓝灯长亮，串口发送数据]\n");
			Create_LOS_Task(On_Get_Tempr_Task, "On_Get_Tempr_Task", &On_Get_Tempr_ID);
		}
		else if (strcmp(command, "gettemproff") == 0)
		{
			printf("[Summary: 关闭热敏电阻获取环境温度，蓝灯熄灭，串口不发送数据]\n");
			Delete_LOS_Task("On_Get_Tempr_Task", On_Get_Tempr_ID);
			HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_SET);
		}
		else if (strcmp(command, "ledallon") == 0)
		{
			printf("[Summary: 所有灯全亮]\n");
			Create_LOS_Task(On_All_LED_Task, "On_All_LED_Task", &On_All_LED_ID);
		}
		else if (strcmp(command, "ledalloff") == 0)
		{
			printf("[Summary: 所有灯全熄灭]\n");
			HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
		}
	}
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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	
	// 熄灭所有 LED 灯
  HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
	
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *) &adcValue, 1);
	
	// Lite OS 内核初始化
	UINT32 uwRet = LOS_KernelInit();
	if (uwRet != LOS_OK)
	  return LOS_NOK;

	// 创建主任务
	Create_LOS_Task(Main_Task, "Main_Task", &Main_Task_ID);
	
	// 启动 Lite OS
	LOS_Start();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV8;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
