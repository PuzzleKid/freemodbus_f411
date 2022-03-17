/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "tim.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"


/* ----------------------- static functions ---------------------------------*/
//static
void prvvTIMERExpiredISR( void );

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
	log("%s\r\n",__FUNCTION__);
	HAL_TIM_Base_DeInit(&htim9);

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};

	htim9.Instance = TIM9;
	htim9.Init.Prescaler = 800 - 1;
	htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim9.Init.Period = usTim1Timerout50us - 1;
	htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim9) != HAL_OK)
	{
	Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig) != HAL_OK)
	{
	Error_Handler();
	}
	return TRUE;
}


inline void
vMBPortTimersEnable(  )
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
	log("%s\r\n",__FUNCTION__);
	__HAL_TIM_CLEAR_IT(&mbhtim,TIM_IT_UPDATE);
    __HAL_TIM_SET_COUNTER(&mbhtim, 0);
    //__HAL_TIM_ENABLE(&mbhtim);
    HAL_TIM_Base_Start_IT(&mbhtim);
}

inline void
vMBPortTimersDisable(  )
{
    /* Disable any pending timers. */
//	log("%s %08x\r\n",__FUNCTION__, mbhtim.Instance->SR);
	//__HAL_TIM_DISABLE(&mbhtim);
	HAL_TIM_Base_Stop_IT(&mbhtim);
	__HAL_TIM_SET_COUNTER(&mbhtim,0);
	__HAL_TIM_CLEAR_IT(&mbhtim,TIM_IT_UPDATE);
	log("%s\r\n",__FUNCTION__);
//	log("%s %08x\r\n",__FUNCTION__, mbhtim.Instance->SR);
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
//static
void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}

//void TIM2_IRQHandler(void)
//{
////	log("%08X\r\n",mbhtim.Instance->DIER);
////	log("%08X\r\n",mbhtim.Instance->SR);
////	log("%s\r\n",__FUNCTION__);
////	vMBPortTimersDisable(  );
//	HAL_TIM_IRQHandler(&mbhtim);
//
//
////	log("%08X\r\n",mbhtim.Instance->DIER);
////	log("%08X\r\n",mbhtim.Instance->SR);
////	log("%s\r\n",__FUNCTION__);
//
//	prvvTIMERExpiredISR();
////    if(__HAL_TIM_GET_FLAG(&mbhtim, TIM_FLAG_UPDATE))
////    {
////        __HAL_TIM_CLEAR_FLAG(&mbhtim, TIM_FLAG_UPDATE);
////        prvvTIMERExpiredISR();
////    }
//}


