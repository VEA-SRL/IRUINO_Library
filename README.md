# Arduino core for the IRUINO


## Contents
- [Development Status](#development-status)
- [Installation Instructions](#installation-instructions)
- [Decoding Exceptions](#decoding-exceptions)
- [Capabilities](#capabilities)

### Development Status

Latest Stable Release  [![Release Version](https://img.shields.io/badge/release-v1.2-33cc33?style=plastic)](https://github.com/VEA-SRL/arduino-hw/releases/tag/v1.2) 


### Installation Instructions
- Using Arduino IDE Boards Manager
  + insert the following links into the Additional Boards Manager URLs field: 
    - `https://raw.githubusercontent.com/VEA-SRL/IRUINO_Library/main/package_vea_index.json` 
    - `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
  + Open the Boards Manager from the Tools -> Board menu and install "iruino", by "VEA SRL"
  + Select the "IRUINO" board from the Tools -> Board menu
  + Select the product you need, for example "HCE-IOV-104"


### Decoding exceptions

You can use [EspExceptionDecoder](https://github.com/me-no-dev/EspExceptionDecoder) to get meaningful call trace.


### Capabilities
The Iruino is a board based on ESP32, with a lot of features.
#### I/O 
The Iruino has 
- 8 digital inputs, named `I0, I1, I2, I3, I4, I5, I6, I7`;
- 8 digital outputs, named `O0, O1, O2, O3, O4, O5, O6, O7`;
- 2 analog inputs, named `A0, A1`.
#### Connettivity
The Iruino has
- WiFi module, can be configured as AP or STA
- Bluetooth module, can be configured as BLE or SPP
#### Display
The Iruino has a 0.96" OLED display, with 128x64 pixels, connected in i2c to the `SDA, SCL` pins.
