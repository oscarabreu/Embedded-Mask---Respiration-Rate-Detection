# Just-Breathe

# ABOUT THIS PROJECT

This current iteration of "Just-Breathe" has been modified to detect respiration rate by using measured deviations in temperature and pressure. This project uses an STM324F and a Bosch BME680 680 sensor embedded within a clinical oxygen mask that will poll for respiration rate using temperature deviations in a closed chamber. If a patient stops breathing for over 5 seconds, the microcontroller will signal an alarm. This device can be used to detect Sleep Apnea, Respiration Rate, and Suddent Infantile Death Syndrome (SUDS). BME680 sensor data is also included within this repository for personal use. 

# WHAT YOU'LL NEED

Install STM32 Cube IDE:
https://www.st.com/en/development-tools/stm32cubeide.html

Install STM32 Cube MX:
https://www.st.com/en/development-tools/stm32cubemx.html

Install Tera-Term
https://ttssh2.osdn.jp/index.html.en

Install STLink
https://www.st.com/en/development-tools/stsw-link009.html

Bosch BME680:
The BME680 is the first gas sensor that integrates high-linearity and high-accuracy gas, pressure, humidity and temperature sensors. It is especially developed for mobile applications and wearables where size and low power consumption are critical requirements. As an I2C device, SDA and SCL pins must be configured, as well as a 3.3V power and Ground pin. Documentation about the BME680 can be visible here:

https://www.bosch-sensortec.com/products/environmental-sensors/gas-sensors/bme680/

STM32F429I-DISC1:
This project was created with an STM32429I Discovery Board and configured usng the STM32 Cube MX initialization code. If you are using a different micrcontroller, you will need to make appropriate adjustments on the STM32CubeMX Platform accordingly. 
https://www.st.com/en/evaluation-tools/32f429idiscovery.html

#HOW TO RUN?

A) Fork this repository: 
1) Navigate to the Just-Breathe project at https://github.com/oscarabreu/Just-Breathe.

2) Click Fork.
![image](https://user-images.githubusercontent.com/99779654/221003438-fc9db10a-be4c-4c06-8894-8a4d55b6a7e1.png)

3) Select an owner for the forked repository.
![image](https://user-images.githubusercontent.com/99779654/221003470-fbe52398-ac40-4c9e-a3bf-ce302d8a1427.png)

4)Optionally, add a description of your fork.
![image](https://user-images.githubusercontent.com/99779654/221003548-d5dab43d-f4ef-4bcc-a79d-0394f4207fd6.png)

5) Choose whether to copy only the default branch or all branches to the new fork. By default, only the default branch is copied.
![image](https://user-images.githubusercontent.com/99779654/221003673-a0b19064-21c0-4c3c-bfc4-b0a9a923ea31.png)

6) Click Create fork
![image](https://user-images.githubusercontent.com/99779654/221003708-57428320-59cf-4741-b180-72daa5ef30e9.png)


B) Initialize your MCU using the STM32CubeMX platform linked above. Further details for newcomers are posted below:
https://www.waveshare.com/wiki/STM32CubeMX_Tutorial_Series:_Overview

C)
Set up your corresponding SDA, SCL, RCC and any GPIO pins you may use for your project.
Example:
![Screen Shot 2023-02-23 at 2 00 51 PM](https://user-images.githubusercontent.com/99779654/221005206-4aefe3cd-8dce-4fe7-a461-9ec9ee34284c.png)

D) Connect your hardware to your corresponding pins. 
Example:
![Screen Shot 2023-02-23 at 2 01 12 PM](https://user-images.githubusercontent.com/99779654/221005452-13c49023-5151-45f6-a4ec-d8057d12c45e.png)

E) Then, open the MX file as a workspace in STM32CubeIDE, and copy the user-code and main contents and corresponding drivers of this current repository, and hit "build". Make sure you have enabled hex/bin file: Instructions on how to do that are posted below:
- In CubeIDE go to Project Settings -> "C/C++ Build" group -> Settings -> "Tool Settings" tab -> MCU Post build outputs -> "Convert to Intel Hex file" check box & "Convert to Binary File" check box
![image](https://user-images.githubusercontent.com/99779654/221006198-ff21c063-1099-4df5-8931-2e15e52d81bf.png)

F) Then, open STLink Utility, and while your STM32 is connected, click on connect. The green bar should quickly reach the end when fully connected and registers may have binary written in the body section of STLink. Once connected, click on Open file, and upload your .bin/.hex file from where it was generated when you last built your project. Next, click on "Program". 
![image](https://user-images.githubusercontent.com/99779654/221007401-94d697c0-c1d7-400f-b850-b0724ace6be1.png)


G) To view the polling temperature and humidity values, install Tera Term, connect to COM3, Hit Setup -> Serial Port -> Speed #115200, and exit. To properly show inline values in Tera Term go to Setup->Terminal->New-Line->Receive:LF, Transmit: LF.
![image](https://user-images.githubusercontent.com/99779654/221007548-30489d12-da2a-4f45-b269-ed5bc2530546.png)

From here, you should be seeing the Temperature, and Humidity values of your BME680! 

Note: Pressure values are not being outputted due to equilibrium of the closed chamber and the corresponding polling rate of this device.
If the project is working you should be seeing this!
[Screen Shot 2023-02-23 at 2 16 08 PM](https://user-images.githubusercontent.com/99779654/221008001-c5fbdf26-e28b-4a90-9e35-07d78068423e.png)


