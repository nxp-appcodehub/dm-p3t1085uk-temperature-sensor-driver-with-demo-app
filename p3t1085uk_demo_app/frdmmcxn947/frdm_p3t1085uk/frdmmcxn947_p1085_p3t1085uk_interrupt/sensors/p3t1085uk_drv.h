/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file p3t1085uk_drv.h
 * @brief The p3t1085uk_drv.h file describes the P3T1085UK (Chiron S&C) driver interface and structures.
 */

#ifndef P3T1085_UK_H_
#define P3T1085_UK_H_

/* Standard C Includes */
#include <stdint.h>

/* ISSDK Includes */
#include "p3t1085uk.h"
#include "sensor_io_i2c.h"
#include "register_io_i2c.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief This defines the sensor specific information for I2C.
 */
typedef struct
{
    registerDeviceInfo_t deviceInfo; /*!< I2C device context. */
    ARM_DRIVER_I2C *pCommDrv;        /*!< Pointer to the i2c driver. */
    bool isInitialized;              /*!< whether sensor is intialized or not.*/
    uint16_t slaveAddress;           /*!< slave address.*/
} p3t1085_i2c_sensorhandle_t;

enum EThermoStatMode
{
    P3T1085UK_TS_COMP = 0,            /*!< Comparator Mode. */
    P3T1085UK_TS_INT = 1,             /*!< Interrupt Mode. */
};

enum EFunctionalMode
{
    P3T1085UK_SHUTDOWN_MODE = 0,      /*!< Shutdown Mode. */
    P3T1085UK_ONESHOT_MODE = 1,		  /*!< Oneshot Mode. */
    P3T1085UK_CONT_CONV_MODE = 2,     /*!< Continuous Conversion Mode. */
};

enum EHysteresis
{
    P3T1085UK_HYST_0C = 0,
    P3T1085UK_HYST_1C = 1,
    P3T1085UK_HYST_2C = 2,
    P3T1085UK_HYST_4C = 3,
};

enum EPolarity
{
    P3T1085UK_POL_ACTIVE_LOW = 0,       /*!<  ALERT pin is active low */
    P3T1085UK_POL_ACTIVEH_HIGH = 1,     /*!<  ALERT pin is active high */
};

enum ECRate
{
    P3T1085UK_CR_0_25HZ = 0,
    P3T1085UK_CR_1HZ = 1,
    P3T1085UK_CR_4HZ = 2,
    P3T1085UK_CR_16HZ = 3,
};
/*******************************************************************************
 * APIs
 ******************************************************************************/

/*! @brief       The interface function to initialize the sensor.
 *  @details     This function initialize the sensor and sensor handle.
 *  @param[in]   pSensorHandle  handle to the sensor.
 *  @param[in]   pBus           pointer to the CMSIS API compatible I2C bus object.
 *  @param[in]   index          the I2C device number.
 *  @param[in]   sAddress       slave address of the device on the bus.
 *  @constraints This should be the first API to be called.
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::P3T1085_I2C_Initialize() returns the status .
 */
int32_t P3T1085_I2C_Initialize(p3t1085_i2c_sensorhandle_t *pSensorHandle, ARM_DRIVER_I2C *pBus, uint8_t index, uint16_t sAddress);

/*! @brief       The interface function to write data in the sensor.
 *  @details     This function write the sensor with data in the regsiter pair array.
 *  @param[in]   pSensorHandle handle to the sensor.
 *  @param[in]   pRegWriteList pointer to the register list.
 *  @constraints This can be called any number of times only after P3T1085_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::P3T1085_I2C_WriteData() returns the status .
 */
int32_t P3T1085_I2C_WriteData(p3t1085_i2c_sensorhandle_t *pSensorHandle, const registerwritelist_t *pRegWriteList);

/*! @brief       The interface function to read the sensor data.
 *  @details     This function read the sensor data out from the device and returns raw data in a byte stream.
 *  @param[in]   pSensorHandle handle to the sensor.
 *  @param[in]   pReadList     pointer to the list of device registers and values to read.
 *  @param[out]  pBuffer       buffer which holds raw sensor data.This buffer may be back to back databuffer based
 *                             command read in the list.
 *  @constraints This can be called any number of times only after P3T1085_I2C_Initialize().
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::P3T1085_I2C_ReadData() returns the status .
 */
int32_t P3T1085_I2C_ReadData(p3t1085_i2c_sensorhandle_t *pSensorHandle, const registerreadlist_t *pReadList, uint8_t *pBuffer);

/*! @brief       The interface function to get the temperature of the sensor.
 *  @details     This function read the temperature of sensor device and returns raw data in celsius.
 *  @param[in]   pSensorHandle handle to the sensor.
 *  @param[out]  pBuffer       handle to the output buffer
 *  @constraints None
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::P3T1085_I2C_GetTemp() returns the status .
 */
int32_t P3T1085_I2C_GetTemp(p3t1085_i2c_sensorhandle_t *pSensorHandle, float *pBuffer);

/*! @brief       The interface function to set the high limit Temperature of the sensor.
 *  @details     This function write the temperature into high limit temperature register of sensor device.
 *  @param[in]   pSensorHandle handle to the sensor.
 *  @param[in]   fTemp         Temperature in celsius
 *  @constraints None
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::P3T1085_I2C_SetTHigh() returns the status .
 */
int32_t P3T1085_I2C_SetTHigh(p3t1085_i2c_sensorhandle_t *pSensorHandle, float fTemp);

/*! @brief       The interface function to set the low limit Temperature of the sensor.
 *  @details     This function write the temperature into low limit temperature register of sensor device.
 *  @param[in]   pSensorHandle handle to the sensor.
 *  @param[in]   fTemp         Temperature in celsius
 *  @constraints None
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::P3T1085_I2C_SetTLow() returns the status .
 */
int32_t P3T1085_I2C_SetTLow(p3t1085_i2c_sensorhandle_t *pSensorHandle, float fTemp);

/*! @brief       The interface function to get the high limit Temperature of the sensor.
 *  @details     This function read high limit temperature register of sensor device.
 *  @param[in]   pSensorHandle handle to the sensor.
 *  @param[out]  pBuffer       handle to the output buffer
 *  @constraints None
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::P3T1085_I2C_GetTHigh() returns the status .
 */
int32_t P3T1085_I2C_GetTHigh(p3t1085_i2c_sensorhandle_t *pSensorHandle, float *pBuffer);

/*! @brief       The interface function to get the low limit Temperature of the sensor.
 *  @details     This function read low limit temperature register of sensor device.
 *  @param[in]   pSensorHandle handle to the sensor.
 *  @param[out]  pBuffer       handle to the output buffer
 *  @constraints None
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::P3T1085_I2C_GetTLow() returns the status .
 */
int32_t P3T1085_I2C_GetTLow(p3t1085_i2c_sensorhandle_t *pSensorHandle, float *pBuffer);

/*! @brief       The interface function to set the thermostat mode of the sensor.
 *  @details     This function set the Comparator mode or Interrupt mode of sensor device.
 *  @param[in]   pSensorHandle handle to the sensor.
 *  @param[in]   eMode         enum of thermostat modes
 *  @constraints None
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::P3T1085_I2C_SetThermoStatMode() returns the status .
 */
int32_t P3T1085_I2C_SetThermoStatMode(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum EThermoStatMode eMode);

/*! @brief       The interface function to set the functional mode of the sensor.
 *  @details     This function set the Oneshot, shutdown and continuous conversion mode of sensor device.
 *  @param[in]   pSensorHandle handle to the sensor.
 *  @param[in]   eMode         enum of functional modes
 *  @constraints None
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::P3T1085_I2C_SetFunctionalMode() returns the status .
 */
int32_t P3T1085_I2C_SetFunctionalMode(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum EFunctionalMode eMode);

/*! @brief       The interface function to set the Hysteresis value of the sensor.
 *  @details     This function set the Hysteresis value of sensor device.
 *  @param[in]   pSensorHandle handle to the sensor.
 *  @param[in]   eHyst        enum of Hysteresis values
 *  @constraints None
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::P3T1085_I2C_SetHysteresis() returns the status .
 */
int32_t P3T1085_I2C_SetHysteresis(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum EHysteresis eHyst);

/*! @brief       The interface function to set the Polarity of the sensor.
 *  @details     This function set the alert pin to high or low of sensor device.
 *  @param[in]   pSensorHandle handle to the sensor.
 *  @param[in]   ePol        enum of Polarity values
 *  @constraints None
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::P3T1085_I2C_SetPolarity() returns the status .
 */
int32_t P3T1085_I2C_SetPolarity(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum EPolarity ePol);

/*! @brief       The interface function to set the conversion rate of the sensor.
 *  @details     This function set conversion time or conversion frequency of sensor device.
 *  @param[in]   pSensorHandle handle to the sensor.
 *  @param[in]   ePol        enum of conversion rate values
 *  @constraints None
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::P3T1085_I2C_SetConversionRate() returns the status .
 */
int32_t P3T1085_I2C_SetConversionRate(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum ECRate eECRate);

/*! @brief       The interface function to check the thermostat mode of the sensor.
 *  @details     This function to check the thermostat mode of sensor device.
 *  @param[in]   pSensorHandle handle to the sensor.
 *  @param[out]  pBuffer       handle to the output buffer
 *  @constraints None
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::P3T1085_I2C_ThermoStatMode() returns the status .
 */
int32_t P3T1085_I2C_GetThermoStatMode(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum EThermoStatMode *pBuffer);

/*! @brief       The interface function to check the functional mode of the sensor.
 *  @details     This function to check the functional mode of sensor device.
 *  @param[in]   pSensorHandle handle to the sensor.
 *  @param[out]  pBuffer       handle to the output buffer
 *  @constraints None
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::P3T1085_I2C_GetFunctionalMode() returns the status .
 */
int32_t P3T1085_I2C_GetFunctionalMode(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum EFunctionalMode *pBuffer);

/*! @brief       The interface function to check hysteresis value of the sensor.
 *  @details     This function to check the hysteresis value of sensor device.
 *  @param[in]   pSensorHandle handle to the sensor.
 *  @param[out]  pBuffer       handle to the output buffer
 *  @constraints None
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::P3T1085_I2C_GetHysteresis() returns the status .
 */
int32_t P3T1085_I2C_GetHysteresis(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum EHysteresis *pBuffer);

/*! @brief       The interface function to check polarity of the sensor.
 *  @details     This function to check the polarity of sensor device.
 *  @param[in]   pSensorHandle handle to the sensor.
 *  @param[out]  pBuffer       handle to the output buffer
 *  @constraints None
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::P3T1085_I2C_GetPolarity() returns the status .
 */
int32_t P3T1085_I2C_GetPolarity(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum EPolarity * pBuffer);

/*! @brief       The interface function to check conversion rate of the sensor.
 *  @details     This function to check the conversion rate values of sensor device.
 *  @param[in]   pSensorHandle handle to the sensor.
 *  @param[out]  pBuffer       handle to the output buffer
 *  @constraints None
 *               Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reeentrant  No
 *  @return      ::P3T1085_I2C_GetConversionRate() returns the status .
 */
int32_t P3T1085_I2C_GetConversionRate(p3t1085_i2c_sensorhandle_t *pSensorHandle, enum ECRate * pBuffer);
#endif // P3T1085_UK_H_
