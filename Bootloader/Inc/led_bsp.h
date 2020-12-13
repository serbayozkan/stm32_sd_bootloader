/*
 * led_bsp.h
 *
 *  Created on: 12 Ara 2020
 *      Author: serbay
 */

#ifndef INC_LED_BSP_H_
#define INC_LED_BSP_H_

#include "gpio.h"

#define LED_ERROR_TOGGLE() HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin)
#define LED_RUN_TOGGLE() HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin)

#endif /* INC_LED_BSP_H_ */
