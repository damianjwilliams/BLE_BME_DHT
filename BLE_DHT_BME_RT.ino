#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>
#include "SparkFunBME280.h"
#include "DHT.h"


#define DHTPIN22 18     // Digital pin connected to the DHT sensor
#define DHTPIN11 19   // Digital pin connected to the DHT sensor
#define DHTTYPE11 DHT11   // DHT 11
#define DHTTYPE22 DHT22   // DHT 22 

DHT dht22(DHTPIN22, DHTTYPE22);
DHT dht11(DHTPIN11, DHTTYPE11);

BLEServer* pServer = NULL;

BLECharacteristic* pTempBMECharacteristic = NULL;
BLECharacteristic* pHumiBMECharacteristic = NULL;

BLECharacteristic* pTempDHT11Characteristic = NULL;
BLECharacteristic* pHumiDHT11Characteristic = NULL;

BLECharacteristic* pTempDHT22Characteristic = NULL;
BLECharacteristic* pHumiDHT22Characteristic = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;



#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

#define TEMPBME_CHAR_UUID    "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define HUMIBME_CHAR_UUID    "16d3930d-2523-4e25-bee8-86c84708b63c"

#define TEMPDHT11_CHAR_UUID   "a9fe148e-fdf2-431b-9ee8-af7e5ff43d07"
#define HUMIDHT11_CHAR_UUID   "d3888281-c1be-4bcd-b55c-b2f9d2c05b47"

#define TEMPDHT22_CHAR_UUID    "6e3139b5-b244-4202-a3b7-7a87b4036d8b"
#define HUMIDHT22_CHAR_UUID    "b4b278d2-1f4a-4d9a-8816-800074a16211"




//DHT dht(DHTPIN, DHTTYPE);


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

BME280 mySensor;

void setup() {
  Serial.begin(115200);

Wire.begin();
dht22.begin();
dht11.begin();


  if (mySensor.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println("The sensor did not respond. Please check wiring.");
    while(1); //Freeze
  }

  //dht.begin();

  // Create the BLE Device
  BLEDevice::init("ESP32DHT");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

// Create the BLE Service
BLEService *pService = pServer->createService(SERVICE_UUID);

//------------------------------------------------------------------- bme temp

 // Create a BLE Characteristic for temperature
pTempBMECharacteristic = pService->createCharacteristic(
                      TEMPBME_CHAR_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

// Create a BLE Descriptor
pTempBMECharacteristic->addDescriptor(new BLE2902());

//------------------------------------------------------------------- bme humi

// Create a BLE Characteristic for humidity
pHumiBMECharacteristic = pService->createCharacteristic(
                      HUMIBME_CHAR_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  // Create a BLE Descriptor
pHumiBMECharacteristic->addDescriptor(new BLE2902());

 //------------------------------------------------------------------ dht11 temp

// Create a BLE Characteristic for pressure
pTempDHT11Characteristic = pService->createCharacteristic(
                      TEMPDHT11_CHAR_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

// Create a BLE Descriptor
pTempDHT11Characteristic->addDescriptor(new BLE2902());

//-------------------------------------------------------------------- dht11 humi
  
// Create a BLE Characteristic for altitude
pHumiDHT11Characteristic = pService->createCharacteristic(
                      HUMIDHT11_CHAR_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

// Create a BLE Descriptor
pHumiDHT11Characteristic->addDescriptor(new BLE2902());

//--------------------------------------------------------------------- dht22 temp
 
 // Create a BLE Characteristic for pressure
pTempDHT22Characteristic = pService->createCharacteristic(
                      TEMPDHT22_CHAR_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

// Create a BLE Descriptor
pTempDHT22Characteristic->addDescriptor(new BLE2902());

//------------------------------------------------------------------------ dht22 humi
 
 // Create a BLE Characteristic for altitude
pHumiDHT22Characteristic = pService->createCharacteristic(
                      HUMIDHT22_CHAR_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

// Create a BLE Descriptor
pHumiDHT22Characteristic->addDescriptor(new BLE2902());

//--------------------------------------------------------------------------

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
    // notify changed value
    if (deviceConnected) {

float tbme = mySensor.readTempC();
float hbme = mySensor.readFloatHumidity();
   
float t11 = dht11.readTemperature();
float h11 = dht11.readHumidity();  
  
float t22 = dht22.readTemperature();
float h22 = dht22.readHumidity();    
  
  
int itbme = int(tbme*100);
int ihbme = int(hbme*100);

int it11 = int(t11*100);
int ih11 =  int(h11*100);

int it22 =  int(t22*100);
int ih22 = int(h22*100);


//Serial.print(F("BME280 Temperature: "));
//Serial.print(tbme);
//Serial.print(F("C\n"));

//Serial.print(F("BME280 Humidity: "));
//Serial.print(hbme);
//Serial.print(F("%\n"));


//Serial.print(F("DHT 11 Temperature: "));
Serial.print(t11);
//Serial.print(F("C\n"));
Serial.print(F(" "));

//Serial.print(F("DHT 11 Humidity: "));
Serial.print(h11);
//Serial.print(F("%\n"));
Serial.print(F("\n"));

// Serial.print(F("DHT 22 Temperature: "));
// Serial.print(t22);
// Serial.print(F("C\n"));

// Serial.print(F("DHT 22 Humidity: "));
// Serial.print(h22);
// Serial.print(F("%\n"));


char buffer[200];
        
      sprintf(buffer, "%i", itbme);
      pTempBMECharacteristic->setValue(buffer);
      pTempBMECharacteristic->notify();

      sprintf(buffer, "%i", ihbme);
      pHumiBMECharacteristic->setValue(buffer);
      pHumiBMECharacteristic->notify();

      sprintf(buffer, "%i", it11);
      pTempDHT11Characteristic->setValue(buffer);
      pTempDHT11Characteristic->notify();

      sprintf(buffer, "%i", ih11);
      pHumiDHT11Characteristic->setValue(buffer);
      pHumiDHT11Characteristic->notify();

      sprintf(buffer, "%i", it22);
      pTempDHT22Characteristic->setValue(buffer);
      pTempDHT22Characteristic->notify();

      sprintf(buffer, "%i", ih22);
      pHumiDHT22Characteristic->setValue(buffer);
      pHumiDHT22Characteristic->notify();

    
        delay(2000); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}
