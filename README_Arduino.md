## 🧑‍💻 Author

**Vaishnav Gophane**  
Embedded Firmware & IoT Developer
<br>
Pune, India.

📫 **Connect:** [Gmail](mr.vaishnavgophane@gmail.com) • [GitHub](https://github.com/vaishnavgophane) • [LinkedIn](https://www.linkedin.com/in/vaishnav-gophane-417686284/)

---

# MAX31865 PT100 – Arduino Implementation

This folder contains the Arduino implementation for interfacing a **PT100 RTD** with the **MAX31865** module using the Arduino UNO platforms.

---

## 🔧 Features

- SPI communication with MAX31865  
- Reads RTD raw values  
- Calculates PT100 resistance  
- Converts resistance → temperature (°C)  
- Uses Adafruit MAX31865 library or custom SPI  
- Fault detection (open, short, VCC/GND fault)

---

## 🛠 Hardware Connections (Arduino UNO)

| MAX31865 | Arduino UNO |
|----------|-------------|
| VCC      | 5V          |
| GND      | GND         |
| SCLK     | D13         |
| SDI (MOSI) | D11       |
| SDO (MISO) | D12       |
| CS       | D10         |

---

## 📦 Required Libraries

Install via Arduino IDE: https://github.com/adafruit/Adafruit_MAX31865

This code performs:

- MAX31865 setup  
- Reads RTD raw count  
- Converts to resistance  
- Applies Callendar–Van Dusen equation  
- Prints temperature over Serial Monitor  

---

## 📊 Example Serial Output
RTD Value : 11234
Resistance : 110.52 Ohms
Temperature : 28.13 °C
Fault : No Fault

---

## 🧪 Testing Notes

- Use **PT100 2-wire** by adding a jumper on the MAX31865 board  
- Ensure RTD wires are properly secured to avoid noise  
- Use **115200 baud rate** on Serial Monitor  
- If temperature is stuck → check `CS`, `MISO`, `MOSI`, and `CLK` wiring  

---

## 📄 Files in This Folder

MAX31865_Arduino.ino → Main Arduino sketch

---

