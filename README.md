# Arduino core for the IRUINO


## Contents
- [Development Status](#development-status)
- [Installation Instructions](#installation-instructions)
- [Decoding Exceptions](#decoding-exceptions)


### Development Status

Latest Stable Release  [![Release Version](https://img.shields.io/badge/release-v1.0-33cc33?style=plastic)](https://github.com/VEA-SRL/arduino-hw/releases/tag/v1.0) 


### Installation Instructions
- Using Arduino IDE Boards Manager
  + insert the following links into the Additional Boards Manager URLs field: 
    - `https://raw.githubusercontent.com/VEA-SRL/IRUINO_Library/main/package_vea_index.json` 
    - `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
  + Open the Boards Manager from the Tools -> Board menu and install "iruino", by "VEA SRL"
  + Select the "IRUINO" board from the Tools -> Board menu


### Decoding exceptions

You can use [EspExceptionDecoder](https://github.com/me-no-dev/EspExceptionDecoder) to get meaningful call trace.

