/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */
#define BACKUP_REGISTER_MAGIC 0x55AA // ����������Ч�Ա��??
#define BACKUP_REGISTER_YEAR RTC_BKP_DR1
#define BACKUP_REGISTER_MONTH RTC_BKP_DR2
#define BACKUP_REGISTER_DATE RTC_BKP_DR3
#define BACKUP_REGISTER_HOUR RTC_BKP_DR4
#define BACKUP_REGISTER_MIN RTC_BKP_DR5
#define BACKUP_REGISTER_SEC RTC_BKP_DR6
#define BACKUP_REGISTER_FLAG RTC_BKP_DR7
/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
  RTC_DateTypeDef sDate = {0};

  __HAL_RCC_PWR_CLK_ENABLE(); // 1. �ȿ�����Դʱ��
  HAL_PWR_EnableBkUpAccess(); // 2. Ȼ��������������
  __HAL_RCC_BKP_CLK_ENABLE(); // 3. �����������ʱ��??

  if (HAL_RTCEx_BKUPRead(&hrtc, BACKUP_REGISTER_FLAG) == BACKUP_REGISTER_MAGIC)
  {
    // �ӱ��ݼĴ�����ȡʱ�������??
    sTime.Seconds = HAL_RTCEx_BKUPRead(&hrtc, BACKUP_REGISTER_SEC);
    sTime.Minutes = HAL_RTCEx_BKUPRead(&hrtc, BACKUP_REGISTER_MIN);
    sTime.Hours = HAL_RTCEx_BKUPRead(&hrtc, BACKUP_REGISTER_HOUR);

    sDate.Date = HAL_RTCEx_BKUPRead(&hrtc, BACKUP_REGISTER_DATE);
    sDate.Month = HAL_RTCEx_BKUPRead(&hrtc, BACKUP_REGISTER_MONTH);
    sDate.Year = HAL_RTCEx_BKUPRead(&hrtc, BACKUP_REGISTER_YEAR);

    // ����������
    HAL_PWR_DisableBkUpAccess();

    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
    HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD);

    return;
  }

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x19;
  sTime.Minutes = 0x59;
  sTime.Seconds = 0x50;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
  DateToUpdate.Month = RTC_MONTH_JANUARY;
  DateToUpdate.Date = 0x1;
  DateToUpdate.Year = 0x25;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    HAL_PWR_EnableBkUpAccess();
    /* Enable BKP CLK enable for backup registers */
    __HAL_RCC_BKP_CLK_ENABLE();
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(RTC_IRQn);
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(RTC_IRQn);
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void SaveTimeToBackup(void)
{
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;

  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
  HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);

  // ���и��ϸ��ʱ����Ч�Լ��
  if (sTime.Hours <= 0x23 && sTime.Minutes <= 0x59 && sTime.Seconds <= 0x59 &&
      sDate.Month <= 0x12 && sDate.Date <= 0x31 && sDate.Year <= 0x99 &&
      !(sTime.Hours == 0 && sTime.Minutes == 0 && sTime.Seconds == 0)) // ȷ��ʱ�䲻Ϊȫ0
  {
    HAL_PWR_DisableBkUpAccess();

    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_BKP_CLK_ENABLE();

    HAL_RTCEx_BKUPWrite(&hrtc, BACKUP_REGISTER_FLAG, 0); // ��д��0��ȷ����������д����ɺ���д��ħ����?

    // д��ʱ�����������?
    HAL_RTCEx_BKUPWrite(&hrtc, BACKUP_REGISTER_SEC, sTime.Seconds);
    HAL_RTCEx_BKUPWrite(&hrtc, BACKUP_REGISTER_MIN, sTime.Minutes);
    HAL_RTCEx_BKUPWrite(&hrtc, BACKUP_REGISTER_HOUR, sTime.Hours);
    HAL_RTCEx_BKUPWrite(&hrtc, BACKUP_REGISTER_DATE, sDate.Date);
    HAL_RTCEx_BKUPWrite(&hrtc, BACKUP_REGISTER_MONTH, sDate.Month);
    HAL_RTCEx_BKUPWrite(&hrtc, BACKUP_REGISTER_YEAR, sDate.Year);

    // ���д��ħ���֣���ʾ���������������Ч
    HAL_RTCEx_BKUPWrite(&hrtc, BACKUP_REGISTER_FLAG, BACKUP_REGISTER_MAGIC);

    // ���ñ��ݷ���
    HAL_PWR_DisableBkUpAccess();
    __HAL_RCC_BKP_CLK_DISABLE();
  }
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
