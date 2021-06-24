# BLE_DHT_DME_RT

This repository contains the scripts required to measure temperature and humidity using DHT11, DHT22 and BME280 sensors hooked up to an ESP32 microcontroller acting as a server. Sensor data is streamed via Bluetooth Low Energy to a client computer running Python. The data is plotted in real-time using pyqtgraph. You can see it in action on this [video](https://youtu.be/ymS4U1X46Kg).

## Requirements

 - ESP32. Purchased from [Amazon](https://www.amazon.com/MELIFE-Development-Dual-Mode-Microcontroller-Integrated/dp/B07Q576VWZ/ref=cm_cr_arp_d_product_top?ie=UTF8). ESP32 installation instructions can be found [here](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/). It will work using the 'ESP32 Dev Module' board setting in the Arduino IDE. More information aboult ESP32 and Bluetooth Low Energy can be found in another great [Random Nerd Tutorial](https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/). The ESP32 BLE code was based on the 'BLE_server' code from the ESP32 BLE Arduino library. Automatically installed when you install the ESP32 on the Arduino IDE. 

 - [DHT11](https://www.mouser.com/ProductDetail/Adafruit/386?qs=GURawfaeGuDbeGFpZ2393w%3D%3D) (Mfr. #: 386)
 -  [DHT22](https://www.mouser.com/ProductDetail/Adafruit/385?qs=GURawfaeGuCJCHEuv7lcfg%3D%3D)(Mfr. #: 385) Both DHT sensors were obtained from Mouser Electronics and require the [Arduino DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library). 
 -  [BME280](https://www.sparkfun.com/products/13676) was from Sparkfun and requires the [BME280 Arduino Library](https://github.com/sparkfun/SparkFun_BME280_Arduino_Library).
 -  Python 3.9 and libraries described in python script. 

## Usage

ESP32 (Server)
 - Wire up sensors and ESP as shown below
 
![My image](https://github.com/damianjwilliams/BLE_DHT_BME_RT/blob/main/BLE_DHT_BME_RT.png)

Upload the ESP32 and sensor libraries to the ESP32

Configure the ESP32 BLE Arduino library to work with six characteristics. The library is initially configured so that the number of characteristics is limited to five per service and, as we are using a single service for all characteristics, is insufficient for this project. The maximum number of handles (i.e. characterisitics) can be changed https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/BLEServer.h#L67. I used 20. The ```BLEserver.h``` header file can be found in the ESP32 core library located (on my Mac): ```/Users/damianjwilliams/Library/Arduino15/packages/esp32/hardware/esp32/libraries/BLE/src/BLEserver.h```.


Computer (Client; I used a Mac)

Install the BLE client [bleak](https://github.com/hbldh/bleak) and other required Python packages. 

Determine ESP32 address.  It is a different process for Macs and PCs. For Macs, run ```discover.py``` from ```bleak/examples``` and the address will be listed in the terminal window next to the ESP32DHT (which is the names assigned in the ESP32 code). In this case : ```6C9F597F-3452-4AAB-806B-D2558588D50D```. 

```
5FFE1058-C77D-4E41-B0B9-D6D48A43D1BA: Unknown
91E60FCF-A821-4E97-A1F1-16FC602D8C3F: Unknown
6C9F597F-3452-4AAB-806B-D2558588D50D: ESP32DHT
1474C2A5-DBA3-400A-8236-8397D4AB73AE: Unknown
```

For the PC you need to run a script on the ESP32 shown [here](https://randomnerdtutorials.com/get-change-esp32-esp8266-mac-address-arduino/) and it will be something like ```21:71:86:CC:09:05```.

Determine the Handle number associated with each characteristic. In bleak all notifications have the characteristic’s integer handle instead of its UUID as
a string as the first argument sender sent to notification callbacks. To determine the handle of each characteristics run run ```service_explorer.py``` from ```bleak/examples``` remembering to change the address on line `59`. 
 

