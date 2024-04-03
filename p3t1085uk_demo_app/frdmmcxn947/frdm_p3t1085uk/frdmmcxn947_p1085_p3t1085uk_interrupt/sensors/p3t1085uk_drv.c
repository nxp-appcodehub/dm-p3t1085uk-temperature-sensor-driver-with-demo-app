/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file p3t1085uk_drv.c
 * @brief The p3t1085uk_drv.c file implements the P3T1085UK sensor driver interfaces.
 */

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "p3t1085uk_drv.h"
#include "systick_utils.h"
#include "stdio.h"

static int32_t P3T1085_I2C_GetTempRegValue(float fTemp, uint8_t * pBuffer)
{
    int32_t negative = 0;
    uint16_t value = 0;

    if (fTemp < 0){
    	 negative = 1;
    	 fTemp *= -1;
    }

    value = (uint16_t)(fTemp / P3T1085UK_CELCIUS_CONV_VAL);
    value = value & 0x0fff;
    if (negative)
        value = ~(value - 1);
    value = value << P3T1085UK_TEMP_IGNORE_SHIFT;

    pBuffer[1] = (uint8_t)value;
    pBuffer[0] = (uint8_t)(value >> 8);

    return SENSOR_ERROR_NONE;
}

static int32_t P3T1085_I2C_GetTempFloatValue(uint8_t * pBuffer, float * fTemp)
{
    int32_t negative = 0;
    uint16_t value = 0, temp = 0;

    temp = (uint16_t)((uint16_t)pBuffer[0] << 8) | (uint16_t)pBuffer[1];
    negative = (temp & P3T1085UK_TEMP_NEGPOS_MASK) != 0;

    temp >>= P3T1085UK_TEMP_IGNORE_SHIFT;
    if (negative) {
        value = ~(temp) + 1;
        value = (value & 0x0fff);
        *fTemp = -1 * P3T1085UK_CELCIUS_CONV_VAL * value;
    } else
        *fTemp = P3T1085UK_CELCIUS_CONV_VAL * temp;

    return SENSOR_ERROR_NONE;
}

static int32_t P3T1085_I2C_BlockedWrite_With_Mask(p3t1085_i2c_sensorhandle_t *pSensorHandle, uint16_t offset, uint16_t value, uint16_t mask)
{
    int32_t status;
    uint8_t reg[P3T1085UK_REG_SIZE_BYTES];
    uint16_t actValue = 0;

    status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, offset, P3T1085UK_REG_SIZE_BYTES, &reg[0]);
    if (ARM_DRIVER_OK != status)
    {
        pSensorHandle->isInitialized = false;
        return SENSOR_ERROR_INIT;
    }
    actValue = (int16_t)((int16_t)reg[0] << 8) | (int16_t)reg[1];

    actValue = (actValue & ~mask) | value;
    reg[1] = (uint8_t)actValue;
    reg[0] = (uint8_t)(actValue >> 8);

    status = Register_I2C_BlockWrite(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
                    offset, &reg[0], P3T1085UK_REG_SIZE_BYTES);

    if (ARM_DRIVER_OK != status)
    {
        return SENSOR_ERROR_WRITE;
    }

    return SENSOR_ERROR_NONE;
}

int32_t P3T1085_I2C_Initialize(
    p3t1085_i2c_sensorhandle_t *pSensorHandle, ARM_DRIVER_I2C *pBus, uint8_t index, uint16_t sAddress)
{
    int32_t status;
    uint8_t reg = 0;

    /*! Check the input parameters. */
    if ((pSensorHandle == NULL) || (pBus == NULL))
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    pSensorHandle->deviceInfo.deviceInstance = index;
    pSensorHandle->deviceInfo.functionParam = NULL;
    pSensorHandle->deviceInfo.idleFunction = NULL;

    /*! Initialize the sensor handle. */
    pSensorHandle->pCommDrv = pBus;
    pSensorHandle->slaveAddress = sAddress;
    pSensorHandle->isInitialized = true;

    return SENSOR_ERROR_NONE;
}

int32_t P3T1085_I2C_GetTemp(p3t1085_i2c_sensorhandle_t *pSensorHandle, float *pBuffer)
{
    uint8_t reg[P3T1085UK_REG_SIZE_BYTES];
    int32_t status;

    /*! Validate for the correct handle and register write list.*/
    if (pSensorHandle == NULL)
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before applying configuration.*/
    if (pSensorHandle->isInitialized != true)
    {
	return SENSOR_ERROR_INIT;
    }
    /*!  Read and store the device's Temp register.*/
    status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, P3T1085UK_TEMP, P3T1085UK_REG_SIZE_BYTES, &reg[0]);
    if (ARM_DRIVER_OK != status)
    {
	pSensorHandle->isInitialized = false;
	return SENSOR_ERROR_INIT;
    }
    P3T1085_I2C_GetTempFloatValue(&reg[0], pBuffer);

    return SENSOR_ERROR_NONE;
}

int32_t P3T1085_I2C_GetTHigh(p3t1085_i2c_sensorhandle_t *pSensorHandle, float *pBuffer)
{
    int32_t status;
    uint8_t reg[P3T1085UK_REG_SIZE_BYTES];

    /*! Validate for the correct handle and register write list.*/
    if (pSensorHandle == NULL)
    {
	return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before applying configuration.*/
    if (pSensorHandle->isInitialized != true)
    {
	return SENSOR_ERROR_INIT;
    }
    /*!  Read and store the device's Temp register.*/
    status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,  P3T1085UK_THIGH, P3T1085UK_REG_SIZE_BYTES, &reg[0]);
    if (ARM_DRIVER_OK != status)
    {
	pSensorHandle->isInitialized = false;
	return SENSOR_ERROR_INIT;
    }

    P3T1085_I2C_GetTempFloatValue(&reg[0], pBuffer);

    return SENSOR_ERROR_NONE;
}

int32_t P3T1085_I2C_GetTLow(p3t1085_i2c_sensorhandle_t *pSensorHandle, float *pBuffer)
{
    int32_t status;
    uint8_t reg[P3T1085UK_REG_SIZE_BYTES];

    /*! Validate for the correct handle and register write list.*/
    if (pSensorHandle == NULL)
    {
	return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before applying configuration.*/
    if (pSensorHandle->isInitialized != true)
    {
	return SENSOR_ERROR_INIT;
    }
    /*!  Read and store the device's Temp register.*/
    status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,  P3T1085UK_TLOW, P3T1085UK_REG_SIZE_BYTES, &reg[0]);
    if (ARM_DRIVER_OK != status)
    {
	pSensorHandle->isInitialized = false;
	return SENSOR_ERROR_INIT;
    }

    P3T1085_I2C_GetTempFloatValue(&reg[0], pBuffer);

    return SENSOR_ERROR_NONE;
}

int32_t P3T1085_I2C_WriteData(p3t1085_i2c_sensorhandle_t *pSensorHandle, const registerwritelist_t *pRegWriteList)
{
    int32_t status;

    /*! Validate for the correct handle and register write list.*/
    if ((pSensorHandle == NULL) || (pRegWriteList == NULL))
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before applying configuration.*/
    if (pSensorHandle->isInitialized != true)
    {
        return SENSOR_ERROR_INIT;
    }

    /*! Apply the Sensor Configuration based on the Register Write List */
    status = Sensor_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
                              pRegWriteList);
    if (ARM_DRIVER_OK != status)
    {
        return SENSOR_ERROR_WRITE;
    }

    return SENSOR_ERROR_NONE;
}

int32_t P3T1085_I2C_ReadData(p3t1085_i2c_sensorhandle_t *pSensorHandle,
                              const registerreadlist_t *pReadList,
                              uint8_t *pBuffer)
{
    int32_t status;

    /*! Validate for the correct handle and register read list.*/
    if ((pSensorHandle == NULL) || (pReadList == NULL) || (pBuffer == NULL))
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before reading sensor data.*/
    if (pSensorHandle->isInitialized != true)
    {
        return SENSOR_ERROR_INIT;
    }

    /*! Parse through the read list and read the data one by one. */
    status = Sensor_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
                             pReadList, pBuffer);
    if (ARM_DRIVER_OK != status)
    {
        return SENSOR_ERROR_READ;
    }

    return SENSOR_ERROR_NONE;
}

int32_t P3T1085_I2C_SetTHigh(p3t1085_i2c_sensorhandle_t *pSensorHandle, float fTemp)
{
    int32_t status, negative = 0;
    uint8_t reg[P3T1085UK_REG_SIZE_BYTES];

    /*! Validate for the correct handle and register write list.*/
    if (pSensorHandle == NULL)
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before applying configuration.*/
    if (pSensorHandle->isInitialized != true)
    {
        return SENSOR_ERROR_INIT;
    }

    P3T1085_I2C_GetTempRegValue(fTemp, &reg[0]);
    status = Register_I2C_BlockWrite(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
                    P3T1085UK_THIGH, &reg[0], P3T1085UK_REG_SIZE_BYTES);
    if (ARM_DRIVER_OK != status)
    {
        return SENSOR_ERROR_WRITE;
    }

    return SENSOR_ERROR_NONE;
}

int32_t P3T1085_I2C_SetTLow(p3t1085_i2c_sensorhandle_t *pSensorHandle, float fTemp)
{
    int32_t status, negative = 0;
    uint8_t reg[P3T1085UK_REG_SIZE_BYTES];

    /*! Validate for the correct handle and register write list.*/
    if (pSensorHandle == NULL)
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before applying configuration.*/
    if (pSensorHandle->isInitialized != true)
    {
        return SENSOR_ERROR_INIT;
    }

    P3T1085_I2C_GetTempRegValue(fTemp, &reg[0]);
    status = Register_I2C_BlockWrite(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
                    P3T1085UK_TLOW, &reg[0], P3T1085UK_REG_SIZE_BYTES);
    if (ARM_DRIVER_OK != status)
    {
        return SENSOR_ERROR_WRITE;
    }

    return SENSOR_ERROR_NONE;
}


int32_t P3T1085_I2C_GetThermoStatMode(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum EThermoStatMode *pBuffer)
{
    uint16_t value = 0;
    uint8_t reg[P3T1085UK_REG_SIZE_BYTES];
    int32_t status;

    /*! Validate for the correct handle and register write list.*/
    if (pSensorHandle == NULL)
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before applying configuration.*/
    if (pSensorHandle->isInitialized != true)
    {
        return SENSOR_ERROR_INIT;
    }
    status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, P3T1085UK_CONFIG, P3T1085UK_REG_SIZE_BYTES, &reg[0]);
    if (ARM_DRIVER_OK != status)
    {
        pSensorHandle->isInitialized = false;
        return SENSOR_ERROR_INIT;
    }
    value = (uint16_t)((uint16_t)reg[0] << 8) | (uint16_t)(reg[1]);
    value = (value & P3T1085UK_CONFIG_TM_MASK) >> P3T1085UK_CONFIG_TM_SHIFT;

    switch(value)
    {
        case P3T1085UK_TS_COMP:
            *pBuffer = P3T1085UK_TS_COMP;
            break;
        case P3T1085UK_TS_INT:
            *pBuffer = P3T1085UK_TS_INT;
            break;
        default:
            *pBuffer = P3T1085UK_TS_COMP;
    }

    return SENSOR_ERROR_NONE;
}

int32_t P3T1085_I2C_SetThermoStatMode(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum EThermoStatMode eMode)
{
    int32_t status;

    /*! Validate for the correct handle and register write list.*/
    if (pSensorHandle == NULL)
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before applying configuration.*/
    if (pSensorHandle->isInitialized != true)
    {
        return SENSOR_ERROR_INIT;
    }

    status = P3T1085_I2C_BlockedWrite_With_Mask(pSensorHandle, P3T1085UK_CONFIG,
                         (uint16_t)(eMode << P3T1085UK_CONFIG_TM_SHIFT), P3T1085UK_CONFIG_TM_MASK);
    if (ARM_DRIVER_OK != status)
    {
        return SENSOR_ERROR_WRITE;
    }

    return SENSOR_ERROR_NONE;
}

int32_t P3T1085_I2C_SetPolarity(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum EPolarity ePol)
{
    int32_t status;

    /*! Validate for the correct handle and register write list.*/
    if (pSensorHandle == NULL)
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before applying configuration.*/
    if (pSensorHandle->isInitialized != true)
    {
        return SENSOR_ERROR_INIT;
    }

    status = P3T1085_I2C_BlockedWrite_With_Mask(pSensorHandle, P3T1085UK_CONFIG,
    (uint16_t)(ePol << P3T1085UK_CONFIG_POL_SHIFT), P3T1085UK_CONFIG_POL_MASK );
    if (ARM_DRIVER_OK != status)
    {
        return SENSOR_ERROR_WRITE;
    }

    return SENSOR_ERROR_NONE;
}

int32_t P3T1085_I2C_GetPolarity(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum EPolarity * pBuffer)
{
    uint16_t value = 0;
    uint8_t reg[P3T1085UK_REG_SIZE_BYTES];
    int32_t status;

    /*! Validate for the correct handle and register write list.*/
    if (pSensorHandle == NULL)
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before applying configuration.*/
    if (pSensorHandle->isInitialized != true)
    {
        return SENSOR_ERROR_INIT;
    }
    status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, P3T1085UK_CONFIG, P3T1085UK_REG_SIZE_BYTES, &reg[0]);
    if (ARM_DRIVER_OK != status)
    {
        pSensorHandle->isInitialized = false;
        return SENSOR_ERROR_INIT;
    }
    value = (uint16_t)((uint16_t)reg[0] << 8) | (uint16_t)(reg[1]);
    value = (value & P3T1085UK_CONFIG_POL_MASK ) >> P3T1085UK_CONFIG_POL_SHIFT;

    switch(value)
    {
        case  P3T1085UK_POL_ACTIVE_LOW:
            *pBuffer =  P3T1085UK_POL_ACTIVE_LOW;
            break;
        case P3T1085UK_POL_ACTIVEH_HIGH:
            *pBuffer = P3T1085UK_POL_ACTIVEH_HIGH;
            break;
        default:
            *pBuffer = P3T1085UK_POL_ACTIVE_LOW;
    }

    return SENSOR_ERROR_NONE;
}

int32_t P3T1085_I2C_SetConversionRate(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum ECRate eECRate)
{
    int32_t status;

    /*! Validate for the correct handle and register write list.*/
    if (pSensorHandle == NULL)
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before applying configuration.*/
    if (pSensorHandle->isInitialized != true)
    {
        return SENSOR_ERROR_INIT;
    }

    status = P3T1085_I2C_BlockedWrite_With_Mask(pSensorHandle, P3T1085UK_CONFIG,
                         (uint16_t)(eECRate << P3T1085UK_CONFIG_CR_SHIFT), P3T1085UK_CONFIG_CR_MASK);
    if (ARM_DRIVER_OK != status)
    {
        return SENSOR_ERROR_WRITE;
    }

    return SENSOR_ERROR_NONE;
}

int32_t P3T1085_I2C_GetConversionRate(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum ECRate * pBuffer)
{
    uint16_t value = 0;
    uint8_t reg[P3T1085UK_REG_SIZE_BYTES];
    int32_t status;

    /*! Validate for the correct handle and register write list.*/
    if (pSensorHandle == NULL)
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before applying configuration.*/
    if (pSensorHandle->isInitialized != true)
    {
        return SENSOR_ERROR_INIT;
    }
    status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, P3T1085UK_CONFIG, P3T1085UK_REG_SIZE_BYTES, &reg[0]);
    if (ARM_DRIVER_OK != status)
    {
        pSensorHandle->isInitialized = false;
        return SENSOR_ERROR_INIT;
    }
    value = (uint16_t)((uint16_t)reg[0] << 8) | (uint16_t)(reg[1]);
    value = (value & P3T1085UK_CONFIG_CR_MASK) >> P3T1085UK_CONFIG_CR_SHIFT;

    switch(value)
    {
        case P3T1085UK_CR_0_25HZ:
            *pBuffer = P3T1085UK_CR_0_25HZ;
            break;
        case P3T1085UK_CR_1HZ:
            *pBuffer = P3T1085UK_CR_1HZ;
            break;
        case P3T1085UK_CR_4HZ:
            *pBuffer = P3T1085UK_CR_4HZ;
            break;
        case P3T1085UK_CR_16HZ:
            *pBuffer = P3T1085UK_CR_16HZ;
            break;
        default:
            *pBuffer = P3T1085UK_CR_1HZ;
    }

    return SENSOR_ERROR_NONE;
}

int32_t P3T1085_I2C_SetHysteresis(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum EHysteresis eHyst)
{
    int32_t status;

    /*! Validate for the correct handle and register write list.*/
    if (pSensorHandle == NULL)
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before applying configuration.*/
    if (pSensorHandle->isInitialized != true)
    {
        return SENSOR_ERROR_INIT;
    }

    status = P3T1085_I2C_BlockedWrite_With_Mask(pSensorHandle, P3T1085UK_CONFIG,
                         (uint16_t)(eHyst << P3T1085UK_CONFIG_HYST_SHIFT), P3T1085UK_CONFIG_HYST_MASK);
    if (ARM_DRIVER_OK != status)
    {
        return SENSOR_ERROR_WRITE;
    }

    return SENSOR_ERROR_NONE;
}

int32_t P3T1085_I2C_GetHysteresis(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum EHysteresis *pBuffer)
{
    uint16_t value = 0;
    uint8_t reg[P3T1085UK_REG_SIZE_BYTES];
    int32_t status;

    /*! Validate for the correct handle and register write list.*/
    if (pSensorHandle == NULL)
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before applying configuration.*/
    if (pSensorHandle->isInitialized != true)
    {
        return SENSOR_ERROR_INIT;
    }
    status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, P3T1085UK_CONFIG, P3T1085UK_REG_SIZE_BYTES, &reg[0]);
    if (ARM_DRIVER_OK != status)
    {
        pSensorHandle->isInitialized = false;
        return SENSOR_ERROR_INIT;
    }
    value = (uint16_t)((uint16_t)reg[0] << 8) | (uint16_t)(reg[1]);
    value = (value & P3T1085UK_CONFIG_HYST_MASK) >> P3T1085UK_CONFIG_HYST_SHIFT;

    switch(value)
    {
        case P3T1085UK_HYST_0C:
            *pBuffer = P3T1085UK_HYST_0C;
            break;
        case P3T1085UK_HYST_1C:
            *pBuffer = P3T1085UK_HYST_1C;
            break;
        case P3T1085UK_HYST_2C:
            *pBuffer = P3T1085UK_HYST_2C;
            break;
        case P3T1085UK_HYST_4C:
            *pBuffer = P3T1085UK_HYST_4C;
            break;
        default:
            *pBuffer = P3T1085UK_HYST_1C;
    }

    return SENSOR_ERROR_NONE;
}

int32_t P3T1085_I2C_SetFunctionalMode(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum EFunctionalMode eMode)
{
    int32_t status;

    /*! Validate for the correct handle and register write list.*/
    if (pSensorHandle == NULL)
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before applying configuration.*/
    if (pSensorHandle->isInitialized != true)
    {
        return SENSOR_ERROR_INIT;
    }

    status = P3T1085_I2C_BlockedWrite_With_Mask(pSensorHandle, P3T1085UK_CONFIG,
    (uint16_t)(eMode << P3T1085UK_CONFIG_MODE_SHIFT), P3T1085UK_CONFIG_MODE_MASK );
    if (ARM_DRIVER_OK != status)
    {
        return SENSOR_ERROR_WRITE;
    }

    return SENSOR_ERROR_NONE;
}

int32_t P3T1085_I2C_GetFunctionalMode(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum EFunctionalMode *pBuffer)
{
    uint16_t value = 0;
    uint8_t reg[P3T1085UK_REG_SIZE_BYTES];
    int32_t status;

    /*! Validate for the correct handle and register write list.*/
    if (pSensorHandle == NULL)
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before applying configuration.*/
    if (pSensorHandle->isInitialized != true)
    {
        return SENSOR_ERROR_INIT;
    }
    status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress, P3T1085UK_CONFIG, P3T1085UK_REG_SIZE_BYTES, &reg[0]);
    if (ARM_DRIVER_OK != status)
    {
        pSensorHandle->isInitialized = false;
        return SENSOR_ERROR_INIT;
    }
    value = (uint16_t)((uint16_t)reg[0] << 8) | (uint16_t)(reg[1]);
    value = (value & P3T1085UK_CONFIG_MODE_MASK) >> P3T1085UK_CONFIG_MODE_SHIFT;

    switch(value)
    {
        case P3T1085UK_SHUTDOWN_MODE:
            *pBuffer = P3T1085UK_SHUTDOWN_MODE;
            break;
        case P3T1085UK_ONESHOT_MODE:
            *pBuffer = P3T1085UK_ONESHOT_MODE;
            break;
        case P3T1085UK_CONT_CONV_MODE:
            *pBuffer = P3T1085UK_CONT_CONV_MODE;
            break;
        default:
            *pBuffer = P3T1085UK_CONT_CONV_MODE;
    }

    return SENSOR_ERROR_NONE;
}
