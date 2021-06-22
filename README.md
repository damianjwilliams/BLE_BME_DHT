# BLE_DHT_DME_RT

This repository contains the scripts required to measure temperature and humidity using DHT11, DHT22 and BME280 sensors hooked up to an ESP32 microcontroller acting as a server. Sensor data is streamed via Bluetooth Low Energy to a client computer running Python. The data is plotted in real-time using pyqtgraph. 

 The ESP32 was purchased from [Amazon](https://www.amazon.com/MELIFE-Development-Dual-Mode-Microcontroller-Integrated/dp/B07Q576VWZ/ref=cm_cr_arp_d_product_top?ie=UTF8). ESP32 installation instructions can be found [here](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/). It will work using the 'ESP32 Dev Module' board setting in the Arduino IDE. More information aboult ESP32 and Bluetooth Low Energy can be found in another great [Random Nerd Tutorial](https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/). The ESP32 BLE code was based on the 'BLE_server' code from the ESP32 BLE Arduino library. Automatically installed when you install the ESP32 on the Arduino IDE. 

 The Adafruit [DHT11](https://www.mouser.com/ProductDetail/Adafruit/386?qs=GURawfaeGuDbeGFpZ2393w%3D%3D) (Mfr. #: 386), and [DHT22](https://www.mouser.com/ProductDetail/adafruit/385/?qs=GURawfaeGuCJCHEuv7lcfg%3D%3Dsensors) (Mfr. #:
 385), were obtained from Mouser Electronics and requires the [Arduino DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library). The [BME280](https://www.sparkfun.com/products/13676) was from Sparkfun and requires the [BME280 Arduino Library](https://github.com/sparkfun/SparkFun_BME280_Arduino_Library).
 
 The ESP32 BLE Arduino library is initially configured so that the number of characteristics is limited to five, which for this project is insufficient. The maximum number of maximum number of handles (i.e. characterisitics) can be changed https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/BLEServer.h#L67. I just used 20. This header file can be found in the ESP32 core library located (on my Mac): /Users/damianjwilliams/Library/Arduino15/packages/esp32/hardware/esp32/libraries/BLE/src/BLEserver.h. 


The BLE client is the computer running Python using [bleak](https://github.com/hbldh/bleak) python package. 

#Determine ESP32 address
To run the code BLE_DHT_BME_RT python code, it is necessary to determine the address of the ESP32. It is a different process for Macs and PCs. For Macs, run ```discover.py``` from the bleak examples and the address will be listed next to the ESP32DHT (which is the names assigned in the ESP32 code). It will look similar to: ```6C9F597F-3452-4AAB-806B-D2558588D50D```. For the PC you need to run a script on the ESP32 shown [here](https://randomnerdtutorials.com/get-change-esp32-esp8266-mac-address-arduino/) and it will be something like ```21:71:86:CC:09:05```.

Determine the Handle number associated with each characterisitic.
 

