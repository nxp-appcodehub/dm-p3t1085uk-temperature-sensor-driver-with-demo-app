/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! File: frdm_stbi_p1085_shield.h
* @brief The frdm_stbi_p1085_shield.h file declares arduino pin mapping for frdm_stbi_p1085_shield expansion board.
*/

#ifndef _FRDM_STBI_P1085_SHIELD_H_
#define _FRDM_STBI_P1085_SHIELD_H_

/* The shield name */
#define SHIELD_NAME "FRDM-STBI-P1085"

// P3T1085 Sensor Information
#define P3T1085_I2C_ADDR 0x48
//#define P3T1085_INT1     D2
#define P3T1085_INT2     A0

// FRDM-STBI-P1085 Shield Reset
#define RESET_GPIO A3

#endif /* _FRDM_STBI_P1085_SHIELD_H_ */
