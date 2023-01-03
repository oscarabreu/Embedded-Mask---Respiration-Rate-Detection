# Just-Breathe

An STM32F4 Embedded Device with a Bosch BME 680 Temperature and Humidity sensor to be integrated with an oxygen mask, that will detect when a patient stops breathing within 5 seconds through a controlled humidity/temperature chamber. This program was written in C, and using STM32CubeIDE. To open this project, install STM32Cube here:

Install STM32 Cube IDE:
https://www.st.com/en/development-tools/stm32cubeide.html
 
The BOSCH BME680 sensor is an I2C device and will need the corresponding SDA and SCL pins to be properly programmed, as well as requiring a ground and 3.3V pin to be properly connected. Install STM32 Cube MX to properly configure your SDA and SCL I2C pins on your particular STM32 board including the GPIO pins that will be responsible for alerting. 

Install STM32 Cube MX:
https://www.st.com/en/development-tools/stm32cubemx.html
 
Then, open the MX file as a workspace in your IDE, copy the main contents as well as the corresponding drivers, build the file and copy the corresponding .bin or .hex file in the Debug folder to any where that is accessible. Then, install STM32 STLink-Utility, and connect your STM32. Click on connect, and then click on Open file, and upload your .bin/.hex file from where it was last saved. Click on program, and you're set. To view the polling temperature and humidity values, install Tera Term, connect to COM3, Hit Setup -> Serial Port -> Speed #115200, and exit. To properly show inline values in Tera Term go to Setup->Terminal->New-Line->Receive:LF, Transmit: LF.

Install Tera-Term
https://ttssh2.osdn.jp/index.html.en

Install STLink
https://www.st.com/en/development-tools/stsw-link009.html


