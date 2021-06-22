import time
import csv
import asyncio
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore, QtGui
from bleak import BleakClient
from qasync import QEventLoop
from collections import deque

# ESP32 ID format for Mac
# Different format for PC
address = "6C9F597F-7085-4AAB-806B-D2558588D50D" # Change to your ESP32

TEMPBME_CHAR_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"  # 41
HUMIBME_CHAR_UUID = "16d3930d-2523-4e25-bee8-86c84708b63c"  # 44

TEMPDHT11_CHAR_UUID = "a9fe148e-fdf2-431b-9ee8-af7e5ff43d07"  # 47
HUMIDHT11_CHAR_UUID = "d3888281-c1be-4bcd-b55c-b2f9d2c05b47"  # 50

TEMPDHT22_CHAR_UUID = "6e3139b5-b244-4202-a3b7-7a87b4036d8b"  # 53
HUMIDHT22_CHAR_UUID = "b4b278d2-1f4a-4d9a-8816-800074a16211"  # 56

length_window = 200


class Window(pg.GraphicsLayoutWidget):
    def __init__(self, loop=None, parent=None):
        super().__init__(parent)
        self._loop = loop

        self.setWindowTitle("pyqtgraph example: Scrolling Plots")

        plot1 = self.addPlot(title="Temperature (C)")
        plot2 = self.addPlot(title="Humidity (%)")
        plot1.addLegend()
        plot1.addLegend()

        plot1.setYRange(15, 50, padding=0)
        plot2.setYRange(20, 100, padding=0)


        self._save_data = []
        self._check_input = []

        self._bme_humi_data = deque([0] * length_window)
        self._curve1 = plot1.plot(self.bme_humi_data, pen='r', name='BME')        #

        self._bme_temp_data = deque([0] * length_window)
        self._curve2 = plot2.plot(self.bme_temp_data, pen='r', name='BME')

        self._dht11_humi_data = deque([0] * length_window)
        self._curve3 = plot1.plot(self.dht11_humi_data, pen='g', name='DHT11')

        self._dht11_temp_data = deque([0] * length_window)
        self._curve4 = plot2.plot(self.dht11_temp_data, pen='g', name='DHT11')

        self._dht22_humi_data = deque([0] * length_window)
        self._curve5 = plot1.plot(self.dht22_humi_data, pen='y', name='DHT22')

        self._dht22_temp_data = deque([0] * length_window)
        self._curve6 = plot2.plot(self.dht22_temp_data, pen='y', name='DHT22')

        self._client = BleakClient(address, loop=self._loop)

    @property
    def client(self):
        return self._client

    async def start(self):
        await self.client.connect()
        self.start_read()

    async def stop(self):
        await self.self.disable_notif()

    @property
    def bme_humi_data(self):
        return self._bme_humi_data

    @property
    def bme_temp_data(self):
        return self._bme_temp_data

    @property
    def dht11_humi_data(self):
        return self._dht11_humi_data

    @property
    def dht11_temp_data(self):
        return self._dht11_temp_data

    @property
    def dht22_humi_data(self):
        return self._dht22_humi_data

    @property
    def dht22_temp_data(self):
        return self._dht22_temp_data

    @property
    def curve1(self):
        return self._curve1

    @property
    def curve2(self):
        return self._curve2

    @property
    def curve3(self):
        return self._curve3

    @property
    def curve4(self):
        return self._curve4

    @property
    def curve5(self):
        return self._curve5

    @property
    def curve6(self):
        return self._curve6



    async def read(self):

        await self.client.start_notify(HUMIBME_CHAR_UUID, self.notification_handler)
        await self.client.start_notify(TEMPBME_CHAR_UUID, self.notification_handler)
        await self.client.start_notify(TEMPDHT11_CHAR_UUID, self.notification_handler)
        await self.client.start_notify(HUMIDHT11_CHAR_UUID, self.notification_handler)
        await self.client.start_notify(TEMPDHT22_CHAR_UUID, self.notification_handler)
        await self.client.start_notify(HUMIDHT22_CHAR_UUID, self.notification_handler)

        await asyncio.sleep(460.0)

        await self.client.stop_notify(TEMPBME_CHAR_UUID)
        await self.client.stop_notify(HUMIBME_CHAR_UUID)
        await self.client.stop_notify(TEMPDHT11_CHAR_UUID)
        await self.client.stop_notify(HUMIDHT11_CHAR_UUID)
        await self.client.stop_notify(TEMPDHT22_CHAR_UUID)
        await self.client.stop_notify(HUMIDHT22_CHAR_UUID)

        QtCore.QTimer.singleShot(1000, self.start_read)

    def notification_handler(self, sender, data):



        measurement = int(data) / 100
        current_time = time.time()
        for_log = [current_time, sender, measurement]
        print(for_log)
        self.log_data(for_log)


#
        if measurement < 120:


            if sender == 41:

                self.update_plot1(measurement)


            elif sender == 44:

                self.update_plot2(measurement)


            elif sender == 47:

                self.update_plot3(measurement)


            elif sender == 50:

                self.update_plot4(measurement)


            elif sender == 53:

                self.update_plot5(measurement)


            elif sender == 56:

                self.update_plot6(measurement)





    def start_read(self):
        asyncio.ensure_future(self.read(), loop=self._loop)

    def update_plot1(self, humibme):

        self.bme_humi_data.append(humibme)
        self.bme_humi_data.popleft()
        self.curve1.setData(self.bme_humi_data)


    def update_plot2(self, tempbme):
        self.bme_temp_data.append(tempbme)
        self.bme_temp_data.popleft()
        self.curve2.setData(self.bme_temp_data)

    def update_plot3(self, humidht11):

        self._dht11_humi_data.append(humidht11)
        self.dht11_humi_data.popleft()
        self.curve3.setData(self.dht11_humi_data)


    def update_plot4(self, tempdht11):
        self.dht11_temp_data.append(tempdht11)
        self.dht11_temp_data.popleft()
        self.curve4.setData(self.dht11_temp_data)


    def update_plot5(self, humidht22):

        self.dht22_humi_data.append(humidht22)
        self.dht22_humi_data.popleft()
        self.curve5.setData(self.dht22_humi_data)


    def update_plot6(self, tempdht22):
        self.dht22_temp_data.append(tempdht22)
        self.dht22_temp_data.popleft()
        self.curve6.setData(self.dht22_temp_data)


    def log_data(self,for_log):
        #change to file path where you want save the log data
        with open("/Users/damianwilliams/Desktop/test_data.csv", "a") as f:
            writer = csv.writer(f, delimiter=",")
            writer.writerow(for_log)


def main(args):
    app = QtGui.QApplication(args)
    loop = QEventLoop(app)
    asyncio.set_event_loop(loop)

    window = Window()
    window.show()

    with loop:
        asyncio.ensure_future(window.start(), loop=loop)
        loop.run_forever()


if __name__ == "__main__":
    import sys

    main(sys.argv)
