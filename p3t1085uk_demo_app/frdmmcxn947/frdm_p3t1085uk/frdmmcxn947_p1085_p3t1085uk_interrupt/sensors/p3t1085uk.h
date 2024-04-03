/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**
 * @file p3t1085uk.h
 * @brief This file contains the P3T1085UK Temperature Sensor register definitions, access macros etc
 */
#ifndef P3T1085UK_H_
#define P3T1085UK_H_
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/**
 * @brief The P3T1085UK Register Definition types
 */

/**
 **
 **  @brief The P3T1085UK Sensor Internal Register Map.
 */
enum
{
    P3T1085UK_TEMP = 0x00,
    P3T1085UK_CONFIG = 0x01,
    P3T1085UK_TLOW = 0x02,
    P3T1085UK_THIGH = 0x03,
};

#define P3T1085UK_REG_SIZE_BYTES	2
#define P3T1085UK_CELCIUS_CONV_VAL	0.0625
#define P3T1085UK_TEMP_IGNORE_SHIFT	((uint16_t)4)
#define P3T1085UK_DEVICE_ADDRESS        (0x48) /*!< Device Address Value. */

#define P3T1085UK_CONFIG_CR_MASK        ((uint16_t)0x6000)
#define P3T1085UK_CONFIG_CR_SHIFT       ((uint16_t)13)
#define P3T1085UK_CONFIG_FH_MASK       ((uint16_t)0x1000)
#define P3T1085UK_CONFIG_FH_SHIFT       ((uint16_t)12)
#define P3T1085UK_CONFIG_FL_MASK       ((uint16_t)0x0800)
#define P3T1085UK_CONFIG_FL_SHIFT       ((uint16_t)11)
#define P3T1085UK_CONFIG_TM_MASK       ((uint16_t)0x0400)
#define P3T1085UK_CONFIG_TM_SHIFT       ((uint16_t)10)
#define P3T1085UK_CONFIG_MODE_MASK       ((uint16_t)0x0300)
#define P3T1085UK_CONFIG_MODE_SHIFT       ((uint16_t)8)
#define P3T1085UK_CONFIG_POL_MASK       ((uint16_t)0x0080)
#define P3T1085UK_CONFIG_POL_SHIFT       ((uint16_t)7)
#define P3T1085UK_CONFIG_HYST_MASK       ((uint16_t)0x0030)
#define P3T1085UK_CONFIG_HYST_SHIFT       ((uint16_t)4)

#define P3T1085UK_TEMP_NEGPOS_MASK       ((uint16_t)0x8000)
#define P3T1085UK_TEMP_NEGPOS_SHIFT       ((uint16_t)15)

#define P3T1085UK_TEMP_CALC_MASK       ((uint16_t)0xFFF0)
#define P3T1085UK_TEMP_CALC_SHIFT       ((uint16_t)4)

#define P3T1085UK_MAX_THIGH_VALUE_CEL	127.9375
#define P3T1085UK_MIN_TLOW_VALUE_CEL	-75.00
#endif
