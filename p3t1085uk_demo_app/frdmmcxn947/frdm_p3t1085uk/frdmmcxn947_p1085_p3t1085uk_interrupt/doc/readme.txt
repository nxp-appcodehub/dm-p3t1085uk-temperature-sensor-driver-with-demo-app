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
===============
When the demo runs successfully, you can see the logs printed on the terminal. 

LOGS:
===============
Temperature =29.000000

Press 0 to continue Read temperature without changing any config value
Press 1 to set Thigh Value
Press 2 to get Thigh Value
Press 3 to set Tlow value
Press 4 to get Tlow value
Press 5 to set Hysteresis value
Press 6 to get Hysteresis value
Press 7 to set Conversion rate
Press 8 to get Conversion rate
Press 9 to set Functional Mode
Press 10 to get Functional Mode
Press 11 to exit
===============


1) If user press 0, there will be no change in device configuarion settings. Application will read the temperature 
and printed on the console.

LOGS:
=============
Temperature =29.125000

Press 0 to continue Read temperature without changing any config value
Press 1 to set Thigh Value
Press 2 to get Thigh Value
Press 3 to set Tlow value
Press 4 to get Tlow value
Press 5 to set Hysteresis value
Press 6 to get Hysteresis value
Press 7 to set Conversion rate
Press 8 to get Conversion rate
Press 9 to set Functional Mode
Press 10 to get Functional Mode
Press 11 to exit
===============

2) If user press 1, then user can set the High threshold temperature value from -25C to 127C
LOGS:
=============
Current Thigh = 56.000000, Enter Thigh value in Celcius, should be less than 127.9375
Set Thigh = 37.000000

Temperature =29.000000

Press 0 to continue Read temperature without changing any config value
Press 1 to set Thigh Value
Press 2 to get Thigh Value
Press 3 to set Tlow value
Press 4 to get Tlow value
Press 5 to set Hysteresis value
Press 6 to get Hysteresis value
Press 7 to set Conversion rate
Press 8 to get Conversion rate
Press 9 to set Functional Mode
Press 10 to get Functional Mode
Press 11 to exit
===============

3) If user press 2, then user can get the High threshold temperature 
LOGS:
=============
Current Thigh = 37.000000

Temperature =29.187500

Press 0 to continue Read temperature without changing any config value
Press 1 to set Thigh Value
Press 2 to get Thigh Value
Press 3 to set Tlow value
Press 4 to get Tlow value
Press 5 to set Hysteresis value
Press 6 to get Hysteresis value
Press 7 to set Conversion rate
Press 8 to get Conversion rate
Press 9 to set Functional Mode
Press 10 to get Functional Mode
Press 11 to exit
===============

4) If user press 3, then user can set Low threshold temperature.
LOGS:
==============
Current TLow = 21.000000, Enter TLow value in Celcius, should not be less than -75
Set TLow = 20.000000

Temperature =29.312500

Press 0 to continue Read temperature without changing any config value
Press 1 to set Thigh Value
Press 2 to get Thigh Value
Press 3 to set Tlow value
Press 4 to get Tlow value
Press 5 to set Hysteresis value
Press 6 to get Hysteresis value
Press 7 to set Conversion rate
Press 8 to get Conversion rate
Press 9 to set Functional Mode
Press 10 to get Functional Mode
Press 11 to exit
==============

5) If user press 4, then user can get Low threshold temperature.
LOGS:
==============
Current TLow = 20.000000

Temperature =29.312500

Press 0 to continue Read temperature without changing any config value
Press 1 to set Thigh Value
Press 2 to get Thigh Value
Press 3 to set Tlow value
Press 4 to get Tlow value
Press 5 to set Hysteresis value
Press 6 to get Hysteresis value
Press 7 to set Conversion rate
Press 8 to get Conversion rate
Press 9 to set Functional Mode
Press 10 to get Functional Mode
Press 11 to exit
==============

6) If user press 5, then user can set Hysteresis value.
LOGS:
==============
Current Hyst = 2°C
Press 0 to set 0C Hyst Value
Press 1 to set 1C(default) Hyst Value
Press 2 to set 2C Hyst Value
Press 3 to set 4C Hyst Value
Set Hyst = 1°C

Temperature =29.312500

Press 0 to continue Read temperature without changing any config value
Press 1 to set Thigh Value
Press 2 to get Thigh Value
Press 3 to set Tlow value
Press 4 to get Tlow value
Press 5 to set Hysteresis value
Press 6 to get Hysteresis value
Press 7 to set Conversion rate
Press 8 to get Conversion rate
Press 9 to set Functional Mode
Press 10 to get Functional Mode
Press 11 to exit
==============

7) If user press 6, then user can get Hysteresis value.
LOGS:
==============
Current Hyst = 1°C
Temperature =29.187500

Press 0 to continue Read temperature without changing any config value
Press 1 to set Thigh Value
Press 2 to get Thigh Value
Press 3 to set Tlow value
Press 4 to get Tlow value
Press 5 to set Hysteresis value
Press 6 to get Hysteresis value
Press 7 to set Conversion rate
Press 8 to get Conversion rate
Press 9 to set Functional Mode
Press 10 to get Functional Mode
Press 11 to exit
==============

8) If user press 7, then user can set the Conversion rate.
LOGS:
==============
Current CRRate = 1Hz
Press 0 to set 0.25Hz Conversion rate
Press 1 to set 1Hz Conversion rate
Press 2 to set 4Hz Conversion rate
Press 3 to set 16Hz Conversion rate
Set CRRate = 0.25Hz

Temperature =29.000000

Press 0 to continue Read temperature without changing any config value
Press 1 to set Thigh Value
Press 2 to get Thigh Value
Press 3 to set Tlow value
Press 4 to get Tlow value
Press 5 to set Hysteresis value
Press 6 to get Hysteresis value
Press 7 to set Conversion rate
Press 8 to get Conversion rate
Press 9 to set Functional Mode
Press 10 to get Functional Mode
Press 11 to exit
==============

9) If user press 8, then user can get the Conversion rate.
LOGS:
==============
Current CRRate = 0.25Hz
Temperature =28.875000

Press 0 to continue Read temperature without changing any config value
Press 1 to set Thigh Value
Press 2 to get Thigh Value
Press 3 to set Tlow value
Press 4 to get Tlow value
Press 5 to set Hysteresis value
Press 6 to get Hysteresis value
Press 7 to set Conversion rate
Press 8 to get Conversion rate
Press 9 to set Functional Mode
Press 10 to get Functional Mode
Press 11 to exit
==============

10) If user press 9, then user can set the different functional mode.
LOGS:
==============
Currently in Shutdown mode
Press 0 to set Shutdown mode
Press 1 to set One-shot mode
Press 2 to set Continuous mode
Set Continuous mode

Temperature =28.812500

Press 0 to continue Read temperature without changing any config value
Press 1 to set Thigh Value
Press 2 to get Thigh Value
Press 3 to set Tlow value
Press 4 to get Tlow value
Press 5 to set Hysteresis value
Press 6 to get Hysteresis value
Press 7 to set Conversion rate
Press 8 to get Conversion rate 
Press 9 to set Functional Mode
Press 10 to get Functional Mode
Press 11 to exit
==============

11) If user press 10, then user can get the different functional mode.
LOGS:
==============
Currently in Continuous mode
Temperature =28.562500

Press 0 to continue Read temperature without changing any config value
Press 1 to set Thigh Value
Press 2 to get Thigh Value
Press 3 to set Tlow value
Press 4 to get Tlow value
Press 5 to set Hysteresis value
Press 6 to get Hysteresis value
Press 7 to set Conversion rate
Press 8 to get Conversion rate
Press 9 to set Functional Mode
Press 10 to get Functional Mode
Press 11 to exit
==============

Note 1: This test application will monitor the temperature continuously.
Different options areavailable to change the configuration settings of sensor device and device to operate
in different mode. In case of under-temperature and overtemperature condition an ALERT LED D2 Glow on Arduino Board.

Note 2: P3T1085_I2C_Initialize() API should be called first in order to use other APIs for differnt device features.
User can refer p3t1085uk_drv.h header file for more information.
