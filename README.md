# BLE_DHT_DME_RT

This repository contains the scripts required to measure temperature and humidity using DHT11, DHT22 and BME280 sensors hooked up to an ESP32 microcontroller. Sensor data is streamed via Bluetooth Low Energy to a computer running Python. The data is plotted in real-time using pyqtgraph. 

The ESP32 was purchased from [Amazon](https://www.amazon.com/MELIFE-Development-Dual-Mode-Microcontroller-Integrated/dp/B07Q576VWZ/ref=cm_cr_arp_d_product_top?ie=UTF8). ESP32 installation instructions can be found [here](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/). It will work using the 'ESP32 Dev Module' board setting in the Arduino IDE. More information aboult ESP32 and Bluetooth Low Energy can be found in another great [Random Nerd Tutorial](https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/).

The Adafruit [DHT11](https://www.mouser.com/ProductDetail/Adafruit/386?qs=GURawfaeGuDbeGFpZ2393w%3D%3D) (Mfr. #: 386), and [DHT22](https://www.mouser.com/ProductDetail/adafruit/385/?qs=GURawfaeGuCJCHEuv7lcfg%3D%3Dsensors) (Mfr. #:
 385), were obtained from Mouser Electronics and requires the [Arduino DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library). The [BME280](https://www.sparkfun.com/products/13676) was from Sparkfun and requires the [BME280 Arduino Library](https://github.com/sparkfun/SparkFun_BME280_Arduino_Library).
