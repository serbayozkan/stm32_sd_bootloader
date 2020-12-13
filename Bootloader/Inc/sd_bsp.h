/*
 * sd_bsp.h
 *
 *  Created on: 12 Ara 2020
 *      Author: serba
 */

#ifndef INC_SD_BSP_H_
#define INC_SD_BSP_H_

#include "gpio.h"

#define SD_CARD_IS_INSERTED() !HAL_GPIO_ReadPin(SD_DETECT_GPIO_PORT, SD_DETECT_PIN)

#endif /* INC_SD_BSP_H_ */
