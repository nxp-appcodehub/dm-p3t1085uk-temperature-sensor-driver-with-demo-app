# NXP Application Code Hub
[<img src="https://mcuxpresso.nxp.com/static/icon/nxp-logo-color.svg" width="100"/>](https://www.nxp.com)

## P3T1085UK: ±0.5 °C accurate temperature-to-digital converter with a -40 °C to +125 °C range.


- This example demo describes how to configure the accurate temperature sensor P3T1085UK for temperature measurement. This sensor can be used in many industrial and medical applications.


- NXP’s next-generation sensors feature a strong balance of intelligent integration, logic and customizable platform software to enable smarter, more differentiated applications.


- Easy enablement is of utmost importance to reduce evaluation, development & time to market. NXP’s Sensor development ecosystem is making it easier for customers to accelerate their sensor product development.

- The image below shows Sensors Development Ecosystem Offering:

    [<img src="./images/Sensors_Development_Ecosystem.png" width="700"/>](Sensors_Development_Ecosystem.png)

 
#### Boards: FRDM-MCXN947 , FRDM-MXCA153
#### Accessories: nxp_p3t1085uk_ard
#### Categories: Sensor
#### Peripherals: I2C
#### Toolchains: MCUXpresso IDE

## Table of Contents
1. [Software](#step1)
2. [Hardware](#step2)
3. [Setup](#step3)
4. [Run Example Projects](#step4)
5. [Run P3T1085UK Demo Example](#step5)
5. [FAQs](#step6) 
6. [Support](#step7)
7. [Release Notes](#step8)

## 1. Software<a name="step1"></a>
- [IoT Sensing SDK (ISSDK) v1.8](https://nxp.com/iot-sensing-sdk) offered as middleware in MCUXpresso SDK for supported platforms
- [MCUXpresso SDK v2.14.0 for FRDM-MCXN947](https://mcuxpresso.nxp.com/en/select)
- [MCUXpresso SDK v2.14.2 for FRDM-MCXA153](https://mcuxpresso.nxp.com/en/select)
- [MCUXpresso IDE v11.9.0](https://www.nxp.com/design/design-center/software/development-software/mcuxpresso-software-and-tools-/mcuxpresso-integrated-development-environment-ide:MCUXpresso-IDE)
- Git v2.39.0

## 2. Hardware<a name="step2"></a>
- FRDM-MCXN947 MCU board
- FRDM-MCXA153 MCU board
- [P3T1085UK-ARD] 
- Personal Computer
- Mini/micro C USB cable

## 3. Setup<a name="step3"></a>
### 3.1 Step 1: Download and Install required Software(s)
- Install MCUXpresso IDE 11.9.0
- Download and Install [MCUXpresso SDK v2.14.0 for FRDM-MCXN947](https://mcuxpresso.nxp.com/en/builder?hw=FRDM-MCXN947). Make sure to select ISSDK  middleware while building SDK.
- Download and Install [MCUXpresso SDK v2.14.2 for FRDM-MCXA153](https://mcuxpresso.nxp.com/en/builder?hw=FRDM-MCXA153). Make sure to select ISSDK  middleware while building SDK.
- Install Git v2.39.0 (for cloning and running west commands).
- Install Putty/Teraterm for UART.

### 3.2 Step 2: Clone the APP-CODE-HUB/dm-p3t1085uk-temperature-sensor-driver-with-demo-app
- Clone this repository to get the example projects:
- Change directory to cloned project folder:<br>
    cd *dm-p3t1085uk-temperature-sensor-driver-with-demo-app*

**Note:** If you are using Windows to clone the project, then please configure filename length limit using below command
 
**git config --system core.longpaths true**

### 3.3 Step 3: Build example projects
- Open MCUXpresso IDE and select a directory to create your workspace.
- Install MCXUpresso SDK 2.14.0 for FRDM-MCX947 (drag and drop SDK zip into "Installed SDK" view) into MCUXpresso IDE.
- Go to "Quickstart Panel" and click on "Import Project(s) from file system",
- Select "Project directory (unpacked)" and browse to the cloned project folder.
- Select example projects that you want to open and run.
- Right click on project and select build to start building the project.

## 4. Run Example Projects<a name="step4"></a>
- Connect the chosen example project HW: FRDM-MCXN947 with P3T1085UK-ARD.
- Right click on project and select "Debug As". Select MCUXpresso IDE LinkServer (inc. CMSIS DAP) probes.
- Connect a USB cable between the host PC and the MCU-Link USB port on the target board.
- Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
- Note : Refer internal readme for detailed information based upon prefered base board(MCXN947/MCXA153).

      Example path : "dm-p3t1085uk-temperature-sensor-driver-with-demo-app\p3t1085uk_demo_app\frdmmcxn947\frdmmcxn947_p3t1085uk_example\doc>"

## 5. Run P3T1085UK Demo Example<a name="step5"></a>

- User need to check COM port after connecting USB cable between Host PC and Target Board via device manager.

[<img src="./images/device_manager.png" width="700"/>](device_manager.png)

- Open PUTTY/Teraterm application installed on your Windows PC with Baudrate 115200 and assigned COM port as mentioned in above step.

[<img src="./images/Putty_Serial_Terminal.PNG" width="700"/>](device_manager.png)

Note 1: By default I2C controller operates in Interrupt mode. For I2C controller to operate in EDMA mode, Define
RTE_I2C2_DMA_EN 1 in RTE_Device.h file under the board directory.


- After right click on project and select "Debug As", Demo application will run in interactive mode. When the demo runs successfully, you can see the logs printed on the terminal.

Main menu will look like:

[<img src="./images/LOG1.PNG" width="700"/>](image1.png)


1) Press #1 to read the temperature and print on the console.

    -Temperature will be printed in format:

[<img src="./images/LOG2.PNG" width="700"/>](image2.png)


2) Use option 2 to 5, to change the low threshold and high threshold temperature value.

   -Steps to set T-High value:

      a) Press #2 on main menu, below option will appear: 

[<img src="./images/LOG3.PNG" width="700"/>](image3.png)
    
       b) Enter value and press enter set T-High.

[<img src="./images/LOG4.PNG" width="700"/>](image3.png)  

3) User can choose option from 6 to 15, to change the device configuration settings.

For example : Use Option #10 (Set Functional Mode)
  
   -Steps to set Functional Mode :

      a) Press #10 on main menu, below option will appear: 

[<img src="./images/LOG6.PNG" width="400"/>](LOG6.png)

      b) Press option (0,1,2) to set required functional mode.

  -Steps to Clear ALERT Signal

       a) Enable ALERT using #2 or #4 options.

 [<img src="./images/LOG7.PNG" width="700"/>](LOG7.png)

       b) To Check that Alert Status use option #17

[<img src="./images/LOG8.PNG" width="400"/>](LOG8.png)

       c) Press #16 to clear ALERT Signal. 
       d) Set valid value and Press enter. 
     
[<img src="./images/LOG9.PNG" width="700"/>](LOG9.png)

       e) Again check #17 to confirm Alert status. 
       
[<img src="./images/LOG10.PNG" width="700"/>](LOG10.png)


Note 2: This test application will monitor the temperature continuously.
Different options are available to change the configuration settings of sensor device and device to operate
in different mode. In case of under-temperature and overtemperature condition an ALERT LED D2 Glow on Arduino Board.

Note 3: P3T1085_I2C_Initialize() API should be called first in order to use other APIs for different device features.
User can refer p3t1085uk_drv.h header file for more information.



## 6. FAQs<a name="step5"></a>
*No FAQs have been identified for this project.*

## 7. Support<a name="step6"></a>
*Provide URLs for help here.*

#### Project Metadata
<!----- Boards ----->
[![Board badge](https://img.shields.io/badge/Board-FRDM&ndash;MCXN947-blue)](https://github.com/search?q=org%3Anxp-appcodehub+FRDM-MCXN947+in%3Areadme&type=Repositories) [![Board badge](https://img.shields.io/badge/Board-FRDM&ndash;MCXA153-blue)](https://github.com/search?q=org%3Anxp-appcodehub+FRDM-MCXA153+in%3Areadme&type=Repositories)

<!----- Categories ----->
[![Category badge](https://img.shields.io/badge/Category-SENSOR-yellowgreen)](https://github.com/search?q=org%3Anxp-appcodehub+sensor+in%3Areadme&type=Repositories)

<!----- Peripherals ----->
[![Peripheral badge](https://img.shields.io/badge/Peripheral-I2C-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+i2c+in%3Areadme&type=Repositories)

<!----- Toolchains ----->
[![Toolchain badge](https://img.shields.io/badge/Toolchain-MCUXPRESSO%20IDE-orange)](https://github.com/search?q=org%3Anxp-appcodehub+mcux+in%3Areadme&type=Repositories)

Questions regarding the content/correctness of this example can be entered as Issues within this GitHub repository.

>**Warning**: For more general technical questions regarding NXP Microcontrollers and the difference in expected funcionality, enter your questions on the [NXP Community Forum](https://community.nxp.com/)

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/@NXP_Semiconductors)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/nxp-semiconductors)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/nxpsemi/)
[![Follow us on Twitter](https://img.shields.io/badge/Twitter-Follow%20us%20on%20Twitter-white.svg)](https://twitter.com/NXP)

## 8. Release Notes<a name="step7"></a>
| Version | Description / Update                           | Date                        |
|:-------:|------------------------------------------------|----------------------------:|
 1.0     |  Initial release on Application Code Hub      | March 28<sup>th</sup> 2024 |  
 2.0     |  Additional features added for P3T1085UK on FRDM-MCXN947 and Complete features added for P3T1085UK on FRDM-MCXA153      | April 30<sup>th</sup> 2024 |

