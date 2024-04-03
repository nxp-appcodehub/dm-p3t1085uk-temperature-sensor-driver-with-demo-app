/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file p3t1085uk_interrupt.c
 * @brief The p3t1085uk_interrupt.c file implements the ISSDK P3T1085UK sensor
 *        driver example demonstration with interrupt mode.
 */

//-----------------------------------------------------------------------
// SDK Includes
//-----------------------------------------------------------------------
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "issdk_hal.h"
#include "gpio_driver.h"
#include "p3t1085uk_drv.h"
#include "systick_utils.h"

//-----------------------------------------------------------------------
// CMSIS Includes
//-----------------------------------------------------------------------
#include "Driver_I2C.h"

//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------

#define P3T1085_INT1_GPIO     GPIO0
#define P3T1085_INT1_PIN      28U
#define P3T1085_INT1_IRQ      GPIO00_IRQn
#define P3T1085_INT1_ISR      GPIO00_IRQHandler

// Constants
//-----------------------------------------------------------------------
/*! @brief Register settings for Interrupt (non buffered) mode. */
const registerwritelist_t cP3t1085ConfigNormal[] = {
    __END_WRITE_DATA__};


//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------

float temperature = 0;
ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER;
p3t1085_i2c_sensorhandle_t p3t1085Driver;
enum EThermoStatMode eThermBuffer;
enum EHysteresis  eHystBuffer;
enum EFunctionalMode eFuncModeBuffer;
enum ECRate eECRateBuffer;
volatile bool gP3t1085IntFlag = false;

gpio_pin_config_t int1_config = {
    kGPIO_DigitalInput,
    0,
};

//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------

void P3T1085_INT1_ISR(void)
{
    /* Clear external interrupt flag. */
    GPIO_GpioClearInterruptFlags(P3T1085_INT1_GPIO, 1U << P3T1085_INT1_PIN);
    gP3t1085IntFlag = true;
    SDK_ISR_EXIT_BARRIER;
}

/*! -----------------------------------------------------------------------
 *  @brief       Initialize P3T1085UK ALert Interrupt Pin and Enable IRQ
 *  @details     This function initializes P3T1085UK interrupt pin
 *  @return      void  There is no return value.
 *  -----------------------------------------------------------------------*/
void init_p3t1085_wakeup_int(void)
{
    CLOCK_EnableClock(kCLOCK_Gpio0);
    GPIO_SetPinInterruptConfig(P3T1085_INT1_GPIO, P3T1085_INT1_PIN, kGPIO_InterruptRisingEdge);

    EnableIRQ(P3T1085_INT1_IRQ);
    GPIO_PinInit(P3T1085_INT1_GPIO, P3T1085_INT1_PIN, &int1_config);
}

/*! -----------------------------------------------------------------------
 *  @brief       Take input from User to enter Thigh value in celcius
 *  @details     This static function sets THigh Value as provided by user and print it back
 *  @return      void  There is no return value.
 *  -----------------------------------------------------------------------*/
static void fModifyThighValue(int get)
{
    float fTemp, fTempLow;

    P3T1085_I2C_GetTHigh(&p3t1085Driver, &fTemp);
    if (get)
    {
        PRINTF("\r\nCurrent Thigh = %f\r\n", fTemp);
        return;
    }
    PRINTF("\r\nCurrent Thigh = %f, Enter Thigh value in Celcius, should be less than 127.9375\r\n", fTemp);
    SCANF("%f", &fTemp);

    if (fTemp > P3T1085UK_MAX_THIGH_VALUE_CEL || fTemp < P3T1085UK_MIN_TLOW_VALUE_CEL)
    {
        PRINTF("\r\nThigh should be in range from -75 to 127.9375\r\n");
        return;
    }
    P3T1085_I2C_GetTLow(&p3t1085Driver, &fTempLow);
    if (fTemp <= fTempLow)
    {
        PRINTF("\r\nThigh should be greater than Tlow\r\n");
        return;

    }
    P3T1085_I2C_SetTHigh(&p3t1085Driver, fTemp);
    P3T1085_I2C_GetTHigh(&p3t1085Driver, &fTemp);

    PRINTF("Set Thigh = %f\r\n", fTemp);
}

/*! -----------------------------------------------------------------------
 *  @brief       Take input from User to enter TLow value in celcius
 *  @details     This static function sets TLow Value as provided by user and print it back
 *  @return      void  There is no return value.
 *  -----------------------------------------------------------------------*/
static void fModifyTlowValue(int get)
{
    float fTemp, fTempHigh;

    P3T1085_I2C_GetTLow(&p3t1085Driver, &fTemp);
    if (get)
    {
        PRINTF("\r\nCurrent TLow = %f\r\n", fTemp);
        return;
    }
    PRINTF("\r\nCurrent TLow = %f, Enter TLow value in Celcius, should not be less than -75\r\n", fTemp);
    SCANF("%f", &fTemp);
    if (fTemp > P3T1085UK_MAX_THIGH_VALUE_CEL || fTemp < P3T1085UK_MIN_TLOW_VALUE_CEL)
    {
        PRINTF("\r\nTlow should be in range from -75 to 127.9375\r\n");
        return;
    }
    P3T1085_I2C_GetTHigh(&p3t1085Driver, &fTempHigh);
    if (fTemp >= fTempHigh)
    {
        PRINTF("\r\nTlow should be less than Thigh\r\n");
        return;

    }
    P3T1085_I2C_SetTLow(&p3t1085Driver, fTemp);
    P3T1085_I2C_GetTLow(&p3t1085Driver, &fTemp);

    PRINTF("Set TLow = %f\r\n", fTemp);
}

/*! -----------------------------------------------------------------------
 *  @brief       Take input from User to enter Hysteresis value in celcius
 *  @details     This static function sets Hyst Value as provided by user and print it back
 *  @return      void  There is no return value.
 *  -----------------------------------------------------------------------*/
static void fModifyHystValue(int get)
{
    uint32_t uHyst;

    P3T1085_I2C_GetHysteresis(&p3t1085Driver, &eHystBuffer);
    switch(eHystBuffer)
    {
        case P3T1085UK_HYST_0C:
            PRINTF("\r\nCurrent Hyst = 0°C");
            break;
        case P3T1085UK_HYST_1C:
            PRINTF("\r\nCurrent Hyst = 1°C");
            break;
        case P3T1085UK_HYST_2C:
            PRINTF("\r\nCurrent Hyst = 2°C");
            break;
        case P3T1085UK_HYST_4C:
            PRINTF("\r\nCurrent Hyst = 4°C");
            break;
        default:
            PRINTF("\r\nError in getting Hyst");
    }
    if (get)
        return;
    PRINTF("\r\nPress 0 to set 0C Hyst Value\r\n");
    PRINTF("Press 1 to set 1C(default) Hyst Value\r\n");
    PRINTF("Press 2 to set 2C Hyst Value\r\n");
    PRINTF("Press 3 to set 4C Hyst Value\r\n");
    SCANF("%u", &uHyst);

    if (uHyst >= 0 && uHyst<= 3)
    {
        P3T1085_I2C_SetHysteresis(&p3t1085Driver, uHyst);
        P3T1085_I2C_GetHysteresis(&p3t1085Driver, &eHystBuffer);
        switch(eHystBuffer)
        {
            case P3T1085UK_HYST_0C:
                PRINTF("Set Hyst = 0°C\r\n");
                break;
            case P3T1085UK_HYST_1C:
                PRINTF("Set Hyst = 1°C\r\n");
                break;
            case P3T1085UK_HYST_2C:
                PRINTF("Set Hyst = 2°C\r\n");
                break;
            case P3T1085UK_HYST_4C:
                PRINTF("Set Hyst = 4°C\r\n");
                break;
            default:
                PRINTF("Error in Setting Hyst\r\n");
        }

    }
    else
        PRINTF("Invalid Input, try next time\r\n");
}

/*! -----------------------------------------------------------------------
 *  @brief       Take input from User to enter Conversion Rate value in Hz
 *  @details     This static function sets CRRate Value as provided by user and print it back
 *  @return      void  There is no return value.
 *  -----------------------------------------------------------------------*/
static void fModifyCRRate(int get)
{
    uint32_t uCRRate;

    P3T1085_I2C_GetConversionRate(&p3t1085Driver, &eECRateBuffer);
    switch (eECRateBuffer)
    {
        case P3T1085UK_CR_0_25HZ:
            PRINTF("\r\nCurrent CRRate = 0.25Hz");
            break;
        case P3T1085UK_CR_1HZ:
            PRINTF("\r\nCurrent CRRate = 1Hz");
            break;
        case P3T1085UK_CR_4HZ:
            PRINTF("\r\nCurrent CRRate = 4Hz");
            break;
        case P3T1085UK_CR_16HZ:
            PRINTF("\r\nCurrent CRRate = 16Hz");
            break;
        default:
            PRINTF("\r\nError in getting Coversion Rate Value");
    }
    if (get)
        return;
    PRINTF("\r\nPress 0 to set 0.25Hz Conversion rate\r\n");
    PRINTF("Press 1 to set 1Hz Conversion rate\r\n");
    PRINTF("Press 2 to set 4Hz Conversion rate\r\n");
    PRINTF("Press 3 to set 16Hz Conversion rate\r\n");
    SCANF("%u", &uCRRate);

    if (uCRRate >= 0 && uCRRate <= 3)
    {
        P3T1085_I2C_SetConversionRate(&p3t1085Driver, uCRRate);
        P3T1085_I2C_GetConversionRate(&p3t1085Driver, &eECRateBuffer);
        switch (eECRateBuffer)
        {
            case P3T1085UK_CR_0_25HZ:
                PRINTF("Set CRRate = 0.25Hz\r\n");
                break;
            case P3T1085UK_CR_1HZ:
                PRINTF("Set CRRate = 1Hz\r\n");
                break;
            case P3T1085UK_CR_4HZ:
                PRINTF("Set CRRate = 4Hz\r\n");
                break;
            case P3T1085UK_CR_16HZ:
                PRINTF("Set CRRate = 16Hz\r\n");
                break;
            default:
                PRINTF("Error in setting Coversion Rate Value\r\n");
        }
    }
    else
        PRINTF("Invalid Input, try next time\r\n");
}

/*! -----------------------------------------------------------------------
 *  @brief       Take input from User to enter Functional Mode value
 *  @details     This static function sets Functional Mode Value as provided by user and print it back
 *  @return      void  There is no return value.
 *  -----------------------------------------------------------------------*/
static void fModifyFunctionalMode(int get)
{
    uint32_t uFunctionalMode;

    P3T1085_I2C_GetFunctionalMode(&p3t1085Driver, &eFuncModeBuffer);
    switch(eFuncModeBuffer)
    {
        case P3T1085UK_SHUTDOWN_MODE:
            PRINTF("\r\nCurrently in Shutdown mode");
            break;
        case P3T1085UK_ONESHOT_MODE:
            PRINTF("\r\nCurrently in Oneshot mode");
            break;
        case P3T1085UK_CONT_CONV_MODE:
            PRINTF("\r\nCurrently in Continuous mode");
            break;
        default:
            PRINTF("\r\nError in getting Functional mode");
    }
    if (get)
        return;
    PRINTF("\r\nPress 0 to set Shutdown mode\r\n");
    PRINTF("Press 1 to set One-shot mode\r\n");
    PRINTF("Press 2 to set Continuous mode\r\n");
    SCANF("%u", &uFunctionalMode);

    if (uFunctionalMode >=0 && uFunctionalMode <= 2)
    {
        P3T1085_I2C_SetFunctionalMode(&p3t1085Driver, uFunctionalMode);
        P3T1085_I2C_GetFunctionalMode(&p3t1085Driver, &eFuncModeBuffer);
        switch(eFuncModeBuffer)
        {
            case P3T1085UK_SHUTDOWN_MODE:
                PRINTF("Set Shutdown mode\r\n");
                break;
            case P3T1085UK_ONESHOT_MODE:
                PRINTF("Set Oneshot mode\r\n");
                break;
            case P3T1085UK_CONT_CONV_MODE:
                PRINTF("Set Continuous mode\r\n");
                break;
            default:
                PRINTF("Error in setting Functional mode\r\n");
        }
    }
    else
        PRINTF("Invalid Input, try next time\r\n");
}

/*! -----------------------------------------------------------------------
 *  @brief       This is the The main function implementation.
 *  @details     This function invokes board initializes routines, then then brings up the sensor and
 *               finally enters an endless loop to continuously read available samples.
 *  @param[in]   void This is no input parameter.
 *  @return      void  There is no return value.
 *  @constraints None
 *  @reeentrant  No
 *  -----------------------------------------------------------------------*/
int main(void)
{
    int32_t status;
    uint32_t input;

    ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER; // Now using the shield.h value!!!

    /*! Initialize the MCU hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_SystickEnable();
    BOARD_InitDebugConsole();

    PRINTF("\r\nISSDK P3T1085UK sensor driver example demonstration with comparator mode.\r\n");

    /*! Initialize the I2C driver. */
    status = I2Cdrv->Initialize(I2C_S_SIGNAL_EVENT);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n I2C Initialization Failed\r\n");
        return -1;
    }

    /*! Set the I2C Power mode. */
    status = I2Cdrv->PowerControl(ARM_POWER_FULL);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n I2C Power Mode setting Failed\r\n");
        return -1;
    }

    /*! Set the I2C bus speed. */
    status = I2Cdrv->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n I2C Control Mode setting Failed\r\n");
        return -1;
    }

    /*! Initialize P3t1085uk sensor driver. */
    status = P3T1085_I2C_Initialize(&p3t1085Driver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, P3T1085_I2C_ADDR);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n Sensor Initialization Failed\r\n");
        return -1;
    }

    while (1)/* Forever loop */
    {
        status = P3T1085_I2C_GetTemp(&p3t1085Driver, &temperature);
        PRINTF("\r\nTemperature =%f\r\n", temperature);
        PRINTF("\r\n");

        if (ARM_DRIVER_OK != status)
        {
            PRINTF("\r\n Read Failed. \r\n");
            return -1;
        }
        PRINTF("Press 0 to continue Read temperature without changing any config value\r\n");
        PRINTF("Press 1 to set Thigh Value\r\n");
        PRINTF("Press 2 to get Thigh Value\r\n");
        PRINTF("Press 3 to set Tlow value\r\n");
        PRINTF("Press 4 to get Tlow value\r\n");
        PRINTF("Press 5 to set Hysteresis value\r\n");
        PRINTF("Press 6 to get Hysteresis value\r\n");
        PRINTF("Press 7 to set Conversion rate\r\n");
        PRINTF("Press 8 to get Conversion rate\r\n");
        PRINTF("Press 9 to set Functional Mode\r\n");
        PRINTF("Press 10 to get Functional Mode\r\n");
        PRINTF("Press 11 to exit\r\n");
        SCANF("%d", &input);
        switch (input)
        {
            case 0:
                break;
            case 1:
                fModifyThighValue(0);
                break;
            case 2:
                fModifyThighValue(1);
                break;
            case 3:
                fModifyTlowValue(0);
                break;
            case 4:
                fModifyTlowValue(1);
                break;
            case 5:
                fModifyHystValue(0);
                break;
            case 6:
                fModifyHystValue(1);
                break;
            case 7:
                fModifyCRRate(0);
                break;
            case 8:
                fModifyCRRate(1);
                break;
            case 9:
                fModifyFunctionalMode(0);
                break;
            case 10:
                fModifyFunctionalMode(1);
                break;
            case 11:
                return 0;
            default:
                PRINTF("Invalid input, Continuing reading temperature\r\n");
        }
    }
}
