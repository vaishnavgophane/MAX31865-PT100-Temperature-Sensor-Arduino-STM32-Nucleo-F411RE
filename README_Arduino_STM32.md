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

# MAX31865 PT100 – STM32 Nucleo-F411RE Implementation

This folder contains the STM32 implementation of the **MAX31865 RTD-to-Digital Converter** interface using the **Nucleo-F411RE (SPI1 + UART2)**.

---

## 🔧 Features

- HAL-based SPI communication  
- Reads MAX31865 registers  
- Reads RTD raw count (15-bit value)  
- Converts RTD resistance → °C  
- Handles MAX31865 faults (open/short)  
- Debug output via UART2 (115200 baud)

---

## 🛠 Hardware Connections (SPI1)

| MAX31865 | STM32 F411RE Pin |
|----------|------------------|
| VCC      | 3.3V             |
| GND      | GND              |
| SCLK     | PA5 (SPI1_SCK)   |
| SDI/MOSI | PA7 (SPI1_MOSI)  |
| SDO/MISO | PA6 (SPI1_MISO)  |
| CS       | PA4              |

### UART2 Debug  
| Function | Pin |
|----------|-----|
| TX       | PA2 |
| RX       | PA3 |

Baud Rate: **115200**

---

## 🔢 Important Source Files

MAX31865.c → MAX31865 driver (SPI read/write, RTD read, faults)
MAX31865.h → Header file
main.c → STM32 app using the driver

---

## 📜 MAX31865 Driver Functions

### ✔ MAX31865_WriteRegister()  
Writes a byte to a specified register.

### ✔ MAX31865_ReadRegister()  
Reads a byte from a register.

### ✔ MAX31865_ReadRTD()  
Reads 15-bit RTD ADC value.

### ✔ MAX31865_Config()  
Enables bias, auto-conversion, 50/60Hz filtering.

### ✔ rtd_to_temperature()  
Converts resistance → temperature.

---

## 📊 Example UART Output(PuTTY)
RTD=8208 R=107.5 Ω T= 19.24 °C

---

## 🧪 Debugging Tips

- If RTD count = 0 → SPI wiring issue  
- If Temp = NAN → fault bit is set  
- If Fault = 0x80 → RTD open or disconnected  
- Ensure MAX31865 is powered with **3.3V** (not 5V!)  
- Keep RTD wires short to reduce noise  

---

## 📄 Files in This Folder

MAX31865.c → MAX31865 driver
MAX31865.h → Driver header
main.c → Main application


