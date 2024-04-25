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
#include "RTE_Device.h"
#include "stdio.h"
#include "frdmmcxa153.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_LPI2C_DMA_BASEADDR DMA0
#define EXAMPLE_LPI2C_DMA_CLOCK    kCLOCK_GateDMA

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
#include "Driver_GPIO.h"

//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------

#define P3T1085_INT1_GPIO     GPIO0
#define P3T1085_INT1_PIN      28U
#define P3T1085_INT1_IRQ      GPIO0_IRQn
#define P3T1085_INT1_ISR      GPIO0_IRQHandler

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
GENERIC_DRIVER_GPIO *pGpioDriver = &Driver_GPIO_KSDK;

p3t1085_i2c_sensorhandle_t p3t1085Driver;
enum EThermoStatMode eThermBuffer;
enum EHysteresis  eHystBuffer;
enum EFunctionalMode eFuncModeBuffer;
enum EThermoStatMode eThermoStatBuffer;
enum EPolarity ePolarity;
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
/*     Clear external interrupt flag.
    GPIO_GpioClearInterruptFlags(P3T1085_INT1_GPIO, 1U << P3T1085_INT1_PIN);
    gP3t1085IntFlag = true;
    SDK_ISR_EXIT_BARRIER;*/

	/* Clear external interrupt flag. */
	GPIO_GpioClearInterruptFlags(ALERT_LED.base, 1U << ALERT_LED.pinNumber);
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
/*    CLOCK_EnableClock(kCLOCK_GateGPIO0);
    GPIO_SetPinInterruptConfig(P3T1085_INT1_GPIO, P3T1085_INT1_PIN, kGPIO_InterruptRisingEdge);

    EnableIRQ(P3T1085_INT1_IRQ);
    GPIO_PinInit(P3T1085_INT1_GPIO, P3T1085_INT1_PIN, &int1_config);*/

	pGpioDriver->pin_init(&ALERT_LED, GPIO_DIRECTION_IN, NULL, NULL, NULL);
	EnableIRQ(P3T1085_INT1_IRQ);
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
		PRINTF("\r\nCurrent T-High = %f°C\r\n", fTemp);
	}
	else{
		PRINTF("\r\nCurrent T-High = %f°C,\r\nEnter T-High value in Celcius (should be less than 127.9375°C)\r\n->", fTemp);
		SCANF("%f", &fTemp);

		if (fTemp > P3T1085UK_MAX_THIGH_VALUE_CEL || fTemp < P3T1085UK_MIN_TLOW_VALUE_CEL)
		{
			PRINTF("\r\nT-High should be in range from -75°C to 127.9375°C\r\n");
			return;
		}
		P3T1085_I2C_GetTLow(&p3t1085Driver, &fTempLow);
		if (fTemp <= fTempLow)
		{
			PRINTF("\r\nT-High should be greater than T-Low\r\n");
			return;

		}
		P3T1085_I2C_SetTHigh(&p3t1085Driver, fTemp);
		P3T1085_I2C_GetTHigh(&p3t1085Driver, &fTemp);

		PRINTF("Set T-High = %f°C\r\n", fTemp);
	}
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
		PRINTF("\r\nCurrent T-Low = %f°C\r\n", fTemp);
	}
	else
	{
		PRINTF("\r\nCurrent T-Low = %f°C,\r\nEnter T-Low value in Celcius (should not be less than -75°C)\r\n->", fTemp);
		SCANF("%f", &fTemp);
		if (fTemp > P3T1085UK_MAX_THIGH_VALUE_CEL || fTemp < P3T1085UK_MIN_TLOW_VALUE_CEL)
		{
			PRINTF("\r\nT-Low should be in range from -75°C to 127.9375°C\r\n");
			return;
		}
		P3T1085_I2C_GetTHigh(&p3t1085Driver, &fTempHigh);
		if (fTemp >= fTempHigh)
		{
			PRINTF("\r\nT-Low should be less than T-High\r\n");
			return;

		}
		P3T1085_I2C_SetTLow(&p3t1085Driver, fTemp);
		P3T1085_I2C_GetTLow(&p3t1085Driver, &fTemp);

		PRINTF("Set TLow = %f°C\r\n", fTemp);
	}
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
		PRINTF("\r\nCurrent Hyst = 0°C\r\n");
		break;
	case P3T1085UK_HYST_1C:
		PRINTF("\r\nCurrent Hyst = 1°C\r\n");
		break;
	case P3T1085UK_HYST_2C:
		PRINTF("\r\nCurrent Hyst = 2°C\r\n");
		break;
	case P3T1085UK_HYST_4C:
		PRINTF("\r\nCurrent Hyst = 4°C\r\n");
		break;
	default:
		PRINTF("\r\nError in getting Hyst\r\n");
	}
	if(get)
		return;

	PRINTF("Press 0 to set 0°C Hyst Value\r\n");
	PRINTF("Press 1 to set 1°C(default) Hyst Value\r\n");
	PRINTF("Press 2 to set 2°C Hyst Value\r\n");
	PRINTF("Press 3 to set 4°C Hyst Value\r\n");
	PRINTF("->");
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
		PRINTF("\r\nCurrent CRRate = 0.25Hz\r\n");
		break;
	case P3T1085UK_CR_1HZ:
		PRINTF("\r\nCurrent CRRate = 1Hz\r\n");
		break;
	case P3T1085UK_CR_4HZ:
		PRINTF("\r\nCurrent CRRate = 4Hz\r\n");
		break;
	case P3T1085UK_CR_16HZ:
		PRINTF("\r\nCurrent CRRate = 16Hz\r\n");
		break;
	default:
		PRINTF("\r\nError in getting Coversion Rate Value\r\n");
	}
	if (get)
		return;
	PRINTF("Press 0 to set 0.25Hz Conversion rate\r\n");
	PRINTF("Press 1 to set 1Hz Conversion rate\r\n");
	PRINTF("Press 2 to set 4Hz Conversion rate\r\n");
	PRINTF("Press 3 to set 16Hz Conversion rate\r\n");
	PRINTF("->");
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
	char dummy;

	P3T1085_I2C_GetFunctionalMode(&p3t1085Driver, &eFuncModeBuffer);
	switch(eFuncModeBuffer)
	{
	case P3T1085UK_SHUTDOWN_MODE:
		PRINTF("\r\nCurrently in Shutdown mode\r\n");
		break;
	case P3T1085UK_ONESHOT_MODE:
		PRINTF("\r\nCurrently in Oneshot mode\r\n");
		break;
	case P3T1085UK_CONT_CONV_MODE:
		PRINTF("\r\nCurrently in Continuous mode\r\n");
		break;
	default:
		PRINTF("\r\nError in getting Functional mode\r\n");
	}
	if (get)
		return;

	PRINTF("Press 0 to set Shutdown mode\r\n");
	PRINTF("Press 1 to set One-shot mode\r\n");
	PRINTF("Press 2 to set Continuous mode\r\n");
	PRINTF("->");
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
 *  @brief       Take input from User to enter Thermostat Mode value
 *  @details     This static function sets Thermostat Mode Value as provided by user and print it back
 *  @return      void  There is no return value.
 *  -----------------------------------------------------------------------*/
static void fModifyThermostatMode(int get)
{
	uint32_t uThermoStatMode;

	P3T1085_I2C_GetThermoStatMode(&p3t1085Driver, &eThermoStatBuffer);
	switch(eThermoStatBuffer)
	{
	case P3T1085UK_TS_COMP:
		PRINTF("\r\nCurrently in Comparator mode\r\n");
		break;
	case P3T1085UK_TS_INT:
		PRINTF("\r\nCurrently in Interrupt mode\r\n");
		break;
	default:
		PRINTF("\r\nError in getting Thermostat mode\r\n");
	}
	if (get)
		return;

	PRINTF("Press 0 to set Comparator mode\r\n");
	PRINTF("Press 1 to set Interrupt mode\r\n");
	PRINTF("->");
	SCANF("%u", &uThermoStatMode);

	if (uThermoStatMode >= 0 && uThermoStatMode <= 1)
	{
		P3T1085_I2C_SetThermoStatMode(&p3t1085Driver, uThermoStatMode);
		P3T1085_I2C_GetThermoStatMode(&p3t1085Driver, &eThermoStatBuffer);
		switch(eThermoStatBuffer)
		{
		case P3T1085UK_TS_COMP:
			PRINTF("\r\nCurrently in Comparator mode\r\n");
			break;
		case P3T1085UK_TS_INT:
			PRINTF("\r\nCurrently in Interrupt mode\r\n");
			break;
		default:
			PRINTF("\r\nError in getting Thermostat mode\r\n");
		}
	}
	else
		PRINTF("Invalid Input, try next time\r\n");
}

/*! -----------------------------------------------------------------------
 *  @brief       Take input from User to enter Polarity value
 *  @details     This static function sets Polarity Value as provided by user and print it back
 *  @return      void  There is no return value.
 *  -----------------------------------------------------------------------*/
static void fModifyPolarity(int get)
{
	uint32_t uEPolarity;

	P3T1085_I2C_GetPolarity(&p3t1085Driver, &ePolarity);
	switch(ePolarity)
	{
	case P3T1085UK_POL_ACTIVE_LOW:
		PRINTF("\r\nALERT pin is active low\r\n");
		break;
	case P3T1085UK_POL_ACTIVE_HIGH:
		PRINTF("\r\nALERT pin is active high\r\n");
		break;
	default:
		PRINTF("\r\nError in getting Polarity value\r\n");
	}
	if (get)
		return;

	PRINTF("Press 0 to Active Low Polarity\r\n");
	PRINTF("Press 1 to Active High Polarity\r\n");
	PRINTF("->");
	SCANF("%u", &uEPolarity);

	if (uEPolarity >= 0 && uEPolarity <= 1)
	{
		P3T1085_I2C_SetPolarity(&p3t1085Driver, uEPolarity);
		P3T1085_I2C_GetPolarity(&p3t1085Driver, &ePolarity);
		switch(ePolarity)
		{
		case P3T1085UK_POL_ACTIVE_LOW:
			PRINTF("\r\nALERT pin is active low\r\n");
			GPIO_SetPinInterruptConfig(ALERT_LED.base, ALERT_LED.pinNumber, kGPIO_InterruptFallingEdge );
			break;
		case P3T1085UK_POL_ACTIVE_HIGH:
			PRINTF("\r\nALERT pin is active high\r\n");
			GPIO_SetPinInterruptConfig(ALERT_LED.base, ALERT_LED.pinNumber, kGPIO_InterruptRisingEdge);
			break;
		default:
			PRINTF("\r\nError in getting Polarity value\r\n");
		}
	}
	else
		PRINTF("Invalid Input, try next time\r\n");
}

/*! -----------------------------------------------------------------------
 *  @brief       Clear the Interrupt Alert Pin in P3T1085UK
 *  @details     This static function clear the alert signal in P3T1085UK
 *  @return      void  There is no return value.
 *  -----------------------------------------------------------------------*/
static void fModifyAlert()
{
	float fTemp, fTempHigh, fTempLow;

	P3T1085_I2C_GetTemp(&p3t1085Driver, &temperature);
	P3T1085_I2C_GetTLow(&p3t1085Driver, &fTempLow);
	P3T1085_I2C_GetTHigh(&p3t1085Driver, &fTempHigh);

	if(temperature >= fTempHigh)
	{
		PRINTF("\r\nEnter the valid High Temperature value to clear the alert pin\r\n");
		PRINTF("Current T-High = %f°C,\r\nEnter T-High value in Celcius (should be less than 127.9375°C)\r\n->", fTempHigh);
		SCANF("%f", &fTempHigh);

		if (fTempHigh > P3T1085UK_MAX_THIGH_VALUE_CEL || fTempHigh < P3T1085UK_MIN_TLOW_VALUE_CEL)
		{
			PRINTF("\r\nT-High should be in range from -75°C to 127.9375°C\r\n");
			return;
		}
		if (fTempHigh <= temperature)
		{
			PRINTF("\r\nT-High should be greater than current Temperature to clear the interrupt\r\n");
			return;
		}
		P3T1085_I2C_SetTHigh(&p3t1085Driver, fTempHigh);
		P3T1085_I2C_GetTHigh(&p3t1085Driver, &fTemp);
		PRINTF("Set T-High = %f°C\r\n", fTemp);
	}
	else if(temperature <= fTempLow)
	{
		PRINTF("\r\nEnter the valid Low Temperature value to clear the alert pin\r\n");
		PRINTF("\r\nCurrent T-Low = %f°C,\r\nEnter T-Low value in Celcius (should not be less than -75°C)\r\n->", fTempLow);
		SCANF("%f", &fTempLow);
		if (fTempLow > P3T1085UK_MAX_THIGH_VALUE_CEL || fTempLow < P3T1085UK_MIN_TLOW_VALUE_CEL)
		{
			PRINTF("\r\nT-High should be in range from -75°C to 127.9375°C\r\n");
			return;
		}
		if (fTempLow >= temperature)
		{
			PRINTF("\r\nT-Low should be less than current Temperature to clear the interrupt\r\n");
			return;
		}
		P3T1085_I2C_SetTLow(&p3t1085Driver, fTempLow);
		P3T1085_I2C_GetTLow(&p3t1085Driver, &fTemp);
		PRINTF("Set T-Low = %f°C\r\n", fTemp);
	}

	P3T1085_I2C_GetThermoStatMode(&p3t1085Driver, &eThermoStatBuffer);
	if(eThermoStatBuffer == P3T1085UK_TS_INT )
	{
		P3T1085_I2C_GetHysteresis(&p3t1085Driver, &eHystBuffer);
	}
}

static void checkAlert()
{
	uint32_t flag = pGpioDriver->read_pin(&ALERT_LED);
	P3T1085_I2C_GetPolarity(&p3t1085Driver, &ePolarity);
	if(ePolarity==P3T1085UK_POL_ACTIVE_LOW)
	{
		if(flag)
			PRINTF("\r\nTemperature Alert Not Generated\r\n");
		else
			PRINTF("\r\nTemperature Alert Generated\r\n");
	}
	else
	{
		if(flag)
			PRINTF("\r\nTemperature Alert Generated\r\n");
		else
			PRINTF("\r\nTemperature Alert Not Generated\r\n");
	}
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
    char dummy;

#if RTE_I2C0_DMA_EN
/*  Enable DMA clock. */
    CLOCK_EnableClock(EXAMPLE_LPI2C_DMA_CLOCK);
    edma_config_t edmaConfig = {0};
    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(EXAMPLE_LPI2C_DMA_BASEADDR, &edmaConfig);
#endif

    RESET_PeripheralReset(kDMA_RST_SHIFT_RSTn);
    ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER; // Now using the shield.h value!!!

    /*! Initialize the MCU hardware. */
    BOARD_InitPins();
    BOARD_InitBootClocks();
    //BOARD_SystickEnable();
    BOARD_InitDebugConsole();
    init_p3t1085_wakeup_int();

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
		PRINTF("\r\nTemperature = %f°C\r\n", temperature);
		PRINTF("\r\n");

		if (ARM_DRIVER_OK != status)
		{
			PRINTF("\r\n Read Failed. \r\n");
			return -1;
		}
		PRINTF("Main Menu\r\n");
		PRINTF("**************************\r\n");
		PRINTF("1.  Continue Read temperature without changing any config value\r\n");
		PRINTF("2.  Set T-High\r\n");
		PRINTF("3.  Get T-High\r\n");
		PRINTF("4.  Set T-Low\r\n");
		PRINTF("5.  Get T-Low\r\n");
		PRINTF("6.  Set Hysteresis\r\n");
		PRINTF("7.  Get Hysteresis\r\n");
		PRINTF("8.  Set Conversion rate\r\n");
		PRINTF("9.  Get Conversion rate\r\n");
		PRINTF("10. Set Functional Mode\r\n");
		PRINTF("11. Get Functional Mode\r\n");
		PRINTF("12. Set Thermostat Mode\r\n");
		PRINTF("13. Get Thermostat Mode\r\n");
		PRINTF("14. Set Polarity\r\n");
		PRINTF("15. Get Polarity\r\n");
		PRINTF("16. Clear ALERT in Under-Tempearture and Over-Temperature Condition\r\n");
		PRINTF("17. Check ALERT LED\r\n");
		PRINTF("18. Exit\r\n");
		PRINTF("**************************\r\n");
		SCANF("%d", &input);
		switch (input)
		{
		case 1:
			status = P3T1085_I2C_GetTemp(&p3t1085Driver, &temperature);
			PRINTF("\r\nCurrent Temperature = %f°C\r\n", temperature);
			break;
		case 2:
			fModifyThighValue(0);
			break;
		case 3:
			fModifyThighValue(1);
			break;
		case 4:
			fModifyTlowValue(0);
			break;
		case 5:
			fModifyTlowValue(1);
			break;
		case 6:
			fModifyHystValue(0);
			break;
		case 7:
			fModifyHystValue(1);
			break;
		case 8:
			fModifyCRRate(0);
			break;
		case 9:
			fModifyCRRate(1);
			break;
		case 10:
			fModifyFunctionalMode(0);
			break;
		case 11:
			fModifyFunctionalMode(1);
			break;
		case 12:
			fModifyThermostatMode(0);
			break;
		case 13:
			fModifyThermostatMode(1);
			break;
		case 14:
			fModifyPolarity(0);
			break;
		case 15:
			fModifyPolarity(1);
			break;
		case 16:
			fModifyAlert();
			break;
		case 17:
			checkAlert();
			break;
		case 18:
			return 0;
		default:
			PRINTF("Invalid input, Continuing reading temperature\r\n");
		}
		PRINTF("\r\nPress Enter to goto Main Menu\r\n");
		do
		{
			dummy = GETCHAR();
		} while(dummy != 13);
	}
}

