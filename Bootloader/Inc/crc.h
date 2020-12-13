/*
 * crc.h
 *
 *  Created on: 12 Ara 2020
 *      Author: serba
 */

#ifndef INC_CRC_H_
#define INC_CRC_H_

#include <stdint.h>

uint32_t rc_crc32(uint32_t crc, const char *buf, uint32_t len);

#endif /* INC_CRC_H_ */
