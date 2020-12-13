/*
 * flash.h
 *
 *  Created on: Dec 1, 2020
 *      Author: serba
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include <stdbool.h>
#include <stdint.h>

#define FLASH_DEBUG              1

int printf(const char *p, ...);

#if FLASH_DEBUG
#define FLASH_PRINT(ch, ...) printf("FLASH_DEBUG: "), printf((ch), ##__VA_ARGS__)
#else
#define FLASH_DEBUG(ch, ...)
#endif

#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_5
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_5  +  GetSectorSize(ADDR_FLASH_SECTOR_5) - 1
#define FLASH_APP_SECTOR_SIZE (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR)

#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

uint32_t GetSectorSize(uint32_t Sector);

bool flash_unlock_and_erase(void);
bool flash_lock();
bool flash_write(uint32_t *address, uint32_t data);
uint32_t flash_read(uint32_t *address);

#endif /* INC_FLASH_H_ */
