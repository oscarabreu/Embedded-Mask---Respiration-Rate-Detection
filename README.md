# STM32 - Respiration Rate Detection - Embedded Oxygen Mask

## About

This current iteration of the project has been modified to detect respiration rate by using measured deviations in temperature and pressure. This project uses an STM324F and a Bosch BME680 680 sensor embedded within a clinical oxygen mask that will poll for respiration rate using temperature deviations in a closed chamber. If a patient stops breathing for over 5 seconds, the microcontroller will signal an alarm. This device can be used to detect Sleep Apnea, Respiration Rate, and Sudden Infantile Death Syndrome (SUDS). BME680 sensor data is also included within this repository for personal use.

## What you'll need

### Software:

- [STM32 Cube IDE](https://www.st.com/en/development-tools/stm32cubeide.html)
- [STM32 Cube MX](https://www.st.com/en/development-tools/stm32cubemx.html)
- [Tera-Term](https://ttssh2.osdn.jp/index.html.en)
- [STLink](https://www.st.com/en/development-tools/stsw-link009.html)

### Hardware:

#### Bosch BME680:

The BME680 is the first gas sensor that integrates high-linearity and high-accuracy gas, pressure, humidity, and temperature sensors. It is especially developed for mobile applications and wearables where size and low power consumption are critical requirements. As an I2C device, SDA and SCL pins must be configured, as well as a 3.3V power and Ground pin. 
[Documentation about the BME680](https://www.bosch-sensortec.com/products/environmental-sensors/gas-sensors/bme680/)

#### STM32F429I-DISC1:

This project was created with an STM32429I Discovery Board and configured using the STM32 Cube MX initialization code. If you are using a different microcontroller, you will need to make appropriate adjustments on the STM32CubeMX Platform accordingly. 
[STM32F429I-DISC1 Details](https://www.st.com/en/evaluation-tools/32f429idiscovery.html)

## How to run

### A) Fork this repository

1. Navigate to the [Just-Breathe project](https://github.com/oscarabreu/Just-Breathe).
2. Click `Fork`.
   ![Fork Image](https://user-images.githubusercontent.com/99779654/221003438-fc9db10a-be4c-4c06-8894-8a4d55b6a7e1.png)
3. Select an owner for the forked repository.
   ![Owner Image](https://user-images.githubusercontent.com/99779654/221003470-fbe52398-ac40-4c9e-a3bf-ce302d8a1427.png)
4. Optionally, add a description of your fork.
   ![Description Image](https://user-images.githubusercontent.com/99779654/221003548-d5dab43d-f4ef-4bcc-a79d-0394f4207fd6.png)
5. Choose whether to copy only the default branch or all branches to the new fork. By default, only the default branch is copied.
   ![Branches Image](https://user-images.githubusercontent.com/99779654/221003673-a0b19064-21c0-4c3c-bfc4-b0a9a923ea31.png)
6. Click `Create fork`.
   ![Create Image](https://user-images.githubusercontent.com/99779654/221003708-57428320-59cf-4741-b180-72daa5ef30e9.png)

### B) Initialize your MCU

Use the STM32CubeMX platform linked above. Further details for newcomers are available [here](https://www.waveshare.com/wiki/STM32CubeMX_Tutorial_Series:_Overview).

### C) Set up your pins

Set up your corresponding SDA, SCL, RCC, and any GPIO pins you may use for your project. 
**Example**:
   ![Pins Image](https://user-images.githubusercontent.com/99779654/221005206-4aefe3cd-8dce-4fe7-a461-9ec9ee34284c.png)

### D) Connect your hardware

Connect your hardware to your corresponding pins.
**Example**:
   ![Hardware Image](https://user-images.githubusercontent.com/99779654/221005452-13c49023-5151-45f6-a4ec-d8057d12c45e.png)

### E) Build the project

Open the MX file as a workspace in STM32CubeIDE, copy the user-code and main contents and corresponding drivers of this repository, and hit "build". Make sure you have enabled hex/bin file.
**Instructions**:

- In CubeIDE go to Project Settings -> "C/C++ Build" group -> Settings -> "Tool Settings" tab -> MCU Post build outputs -> "Convert to Intel Hex file" check box & "Convert to Binary File" check box.
   ![Build Image](https://user-images.githubusercontent.com/99779654/221006198-ff21c063-1099-4df5-8931-2e15e52d81bf.png)

### F) Program the STM32

1. Open STLink Utility.
2. While your STM32 is connected, click on connect.
3. Once connected, click on Open file.
4. Upload your .bin/.hex file from where it was generated when you last built your project.
5. Next, click on "Program".
   ![STLink Image](https://user-images.githubusercontent.com/99779654/221007401

### G) View the results

To view the polling temperature and humidity values, install Tera Term, connect to COM3, Hit Setup -> Serial Port -> Speed #115200, and exit. To properly show inline values in Tera Term go to Setup->Terminal->New-Line->Receive:LF, Transmit: LF.

**Expected Output**:

   ![Output Image](https://user-images.githubusercontent.com/99779654/221008001-c5fbdf26-e28b-4a90-9e35-07d78068423e.png)

> **Note**: Pressure values are not being outputted due to the equilibrium of the closed chamber and the corresponding polling rate of this device.


