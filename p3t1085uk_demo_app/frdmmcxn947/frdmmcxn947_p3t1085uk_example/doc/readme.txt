Hardware requirements
===================
- Mini/micro C USB cable
- FRDM-MCXN947 board
- P3T1085UK-ARD board (https://www.nxp.com/part/P3T1085UK-ARD#/)
- Personal Computer

Board settings
============
Since the examples use I2C, Pins 2-3 of JP2 and JP3 on P3T1085UK should be connected.
JP1 Pins 2-3 should be connected to select default operating voltage level i.e. "+3V3".
Pins 3-5 of J10 should be connected for A0 to be connected with GND for device I2C address.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
NOTE: By default I2C controller operates in Interrupt mode. For I2C controller to operate in EDMA mode, Define
#define RTE_I2C2_DMA_EN 1 in RTE_Device.h file under the board directory.

===============
When the demo runs successfully, you can see the logs printed on the terminal. 

LOGS:
===============
ISSDK P3T1085UK sensor driver example demonstration with comparator mode.

Temperature = 27.562500°C

Main Menu
**************************
1.  Continue Read temperature without changing any config value
2.  Set T-High
3.  Get T-High
4.  Set T-Low
5.  Get T-Low
6.  Set Hysteresis
7.  Get Hysteresis
8.  Set Conversion rate
9.  Get Conversion rate
10. Set Functional Mode
11. Get Functional Mode
12. Set Thermostat Mode
13. Get Thermostat Mode
14. Set Polarity
15. Get Polarity
16. Clear ALERT in Under-Tempearture and Over-Temperature Condition
17. Check ALERT LED
18. Exit
**************************
===============

1) If user press 1, there will be no change in device configuarion settings. Application will read the temperature 
and printed on the console.

LOGS:
=============
Temperature = 27.437500°C
===============

2) If user press 2, then user can set the High threshold temperature value from -25C to 127C
LOGS:
=============
Current T-High = 127.937500°C,
Enter T-High value in Celcius (should be less than 127.9375°C)
->Set T-High = 30.000000°C
Press Enter to goto Main Menu
===============

3) If user press 3, then user can get the High threshold temperature 
LOGS:
=============
Current T-High = 30.000000°C
Press Enter to goto Main Menu
===============

4) If user press 4, then user can set Low threshold temperature.
LOGS:
==============
Current T-Low = 20.000000°C,
Enter T-Low value in Celcius (should not be less than -75°C)
->Set TLow = 12.000000°C
Press Enter to goto Main Menu
==============

5) If user press 5, then user can get Low threshold temperature.
LOGS:
==============
Current T-Low = 12.000000°C
Press Enter to goto Main Menu
==============

6) If user press 6, then user can set Hysteresis value.
LOGS:
==============
Current Hyst = 1°C
Press 0 to set 0°C Hyst Value
Press 1 to set 1°C(default) Hyst Value
Press 2 to set 2°C Hyst Value
Press 3 to set 4°C Hyst Value
->Set Hyst = 0°C
Press Enter to goto Main Menu
==============

7) If user press 7, then user can get Hysteresis value.
LOGS:
==============
Current Hyst = 0°C
Press Enter to goto Main Menu
==============

8) If user press 8, then user can set the Conversion rate.
LOGS:
==============
Current CRRate = 1Hz
Press 0 to set 0.25Hz Conversion rate
Press 1 to set 1Hz Conversion rate
Press 2 to set 4Hz Conversion rate
Press 3 to set 16Hz Conversion rate
->Set CRRate = 0.25Hz
Press Enter to goto Main Menu
==============

9) If user press 9, then user can get the Conversion rate.
LOGS:
==============
Current CRRate = 0.25Hz
Press Enter to goto Main Menu
==============

10) If user press 10, then user can set the different functional mode.
LOGS:
==============
Currently in Continuous mode
Press 0 to set Shutdown mode
Press 1 to set One-shot mode
Press 2 to set Continuous mode
->Set Shutdown mode
Press Enter to goto Main Menu
==============

11) If user press 11, then user can get the different functional mode.
LOGS:
==============
Currently in Continuous mode
Press Enter to goto Main Menu
==============

12) If user press 12, then user can set the different thermostat mode.
LOGS:
==============
Currently in Comparator mode
Press 0 to set Comparator mode
Press 1 to set Interrupt mode
->
Currently in Interrupt mode
Press Enter to goto Main Menu
==============

13) If user press 13, then user can get different thermostat mode.
LOGS:
==============
Currently in Interrupt mode
Press Enter to goto Main Menu
==============

14) If user press 14, then user can set the polarity.
LOGS:
==============
ALERT pin is active low
Press 0 to Active Low Polarity
Press 1 to Active High Polarity
->
ALERT pin is active high
Press Enter to goto Main Menu
==============

15) If user press 15, then user can get the polarity.
LOGS:
==============
ALERT pin is active high
Press Enter to goto Main Menu
==============

16) If user press 16, then user can clear the alert status in case of Under-Temperature and Over-Temperature condition.
LOGS:
==============
Enter the valid High Temperature value to clear the alert pin
Current T-High = 22.000000°C,
Enter T-High value in Celcius (should be less than 127.9375°C)
->Set T-High = 40.000000°C
Press Enter to goto Main Menu
==============

17) If user press 17, then user can check the ALERT LED status.
LOGS:
==============
Temperature Alert Generated
Press Enter to goto Main Menu
**************************
==============


Note 1: This test application will monitor the temperature continuously.
Different options are available to change the configuration settings of sensor device and device to operate
in different mode. In case of under-temperature and overtemperature condition an ALERT LED D2 Glow on Arduino Board.

Note 2: P3T1085_I2C_Initialize() API should be called first in order to use other APIs for differnt device features.
User can refer p3t1085uk_drv.h header file for more information.
