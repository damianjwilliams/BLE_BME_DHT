# BLE_DHT_DME_RT

This repository contains the scripts required to measure temperature and humidity using DHT11, DHT22 and BME280 sensors hooked up to an ESP32 microcontroller acting as a server. Sensor data is streamed via Bluetooth Low Energy to a client computer running Python. The data is plotted in real-time using pyqtgraph. You can see it in action on this [video](https://youtu.be/ymS4U1X46Kg).

## Requirements

 - ESP32. Purchased from [Amazon](https://www.amazon.com/MELIFE-Development-Dual-Mode-Microcontroller-Integrated/dp/B07Q576VWZ/ref=cm_cr_arp_d_product_top?ie=UTF8). ESP32 installation instructions can be found [here](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/). It will work using the 'ESP32 Dev Module' board setting in the Arduino IDE. More information aboult ESP32 and Bluetooth Low Energy can be found in another great [Random Nerd Tutorial](https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/). The ESP32 BLE code was based on the 'BLE_server' code from the ESP32 BLE Arduino library. Automatically installed when you install the ESP32 on the Arduino IDE. 

 - [DHT11](https://www.mouser.com/ProductDetail/Adafruit/386?qs=GURawfaeGuDbeGFpZ2393w%3D%3D) (Mfr. #: 386)
 -  [DHT22](https://www.mouser.com/ProductDetail/Adafruit/385?qs=GURawfaeGuCJCHEuv7lcfg%3D%3D)(Mfr. #: 385) Both DHT sensors were obtained from Mouser Electronics and require the [Arduino DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library). 
 -  [BME280](https://www.sparkfun.com/products/13676) was from Sparkfun and requires the [BME280 Arduino Library](https://github.com/sparkfun/SparkFun_BME280_Arduino_Library).
 -  Python 3.9 and libraries described in python script. 

## Usage

**ESP32 (Server)**
 - Wire up sensors and ESP as shown below
 
![My image](https://github.com/damianjwilliams/BLE_DHT_BME_RT/blob/main/BLE_DHT_BME_RT.png)

Upload the ESP32 and sensor libraries to the ESP32

Configure the ESP32 BLE Arduino library to work with six characteristics. The library is initially configured so that the number of characteristics is limited to five per service and, as we are using a single service for all characteristics, is insufficient for this project. The maximum number of handles (i.e. characterisitics) can be changed https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/BLEServer.h#L67. I used 20. The ```BLEserver.h``` header file can be found in the ESP32 core library located (on my Mac): ```/Users/damianjwilliams/Library/Arduino15/packages/esp32/hardware/esp32/libraries/BLE/src/BLEserver.h```.


**Computer (Client; I used a Mac)**

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
a string as the first argument sender sent to notification callbacks. To determine the handle of each characteristics run  ```service_explorer.py``` from ```bleak/examples``` remembering to change the address on `line 59`. The output will look like
```
[Service] 4fafc201-1fb5-459e-8fcc-c5c9c331914b (Handle: 40): Unknown
	[Characteristic] beb5483e-36e1-4688-b7f5-ea07361b26a8 (Handle: 41): Unknown (read,write,notify,indicate), Value: b'2529'
		[Descriptor] 00002902-0000-1000-8000-00805f9b34fb (Handle: 43): Client Characteristic Configuration) | Value: b'\x00\x00'
	[Characteristic] 16d3930d-2523-4e25-bee8-86c84708b63c (Handle: 44): Unknown (read,write,notify,indicate), Value: b'3759'
		[Descriptor] 00002902-0000-1000-8000-00805f9b34fb (Handle: 46): Client Characteristic Configuration) | Value: b'\x00\x00'
	[Characteristic] a9fe148e-fdf2-431b-9ee8-af7e5ff43d07 (Handle: 47): Unknown (read,write,notify,indicate), Value: b'2690'
 ...
 ```
 The handle is found after the Characteristic UUID. It may well be different to those shown above. Keep a note of the handle number and make sure you know which characteristic is associated with which sensor reading. The handle number is important as it is used to identify the sensor measurement in the `BLE_DHT_BME_RT.py` script.
 
Change the address on `line 12` of `BLE_DHT_BME_RT.py` to that determined above. 
 
Change the ```sender``` to handle numbers on lines between 161 - 186. In this example, we know that handle 41 is associated with `beb5483e-36e1-4688-b7f5-ea07361b26a8` (from running `service_explorer.py`), and we know the characteristic is associated with the BME280 temperature measurement (defined in the ESP32 code) and hence the measurement will be plotted on `plot1`.

```
           if sender == 41:

                self.update_plot1(measurement)

```

Change the file path on `line 237` to the directory where you would like the log data to be stored.
 
 
 I found that with six characteristics some of the measurements were dropped or corrupted. I filtered the measurements to < 120 on `line 158` as a work around to stop stange numbers being recorded/plotted. I didn't see a similar problem using two characteristics. 
 
 You can plot the CSV file using the R script (changing the handle numbers as appropriate). 
 

