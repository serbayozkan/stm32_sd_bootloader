# STM32 SD Bootloader

## Description
Bootloader project which enables firmware update over sd card with a lot of error handling mechanisms.
- Flash Integrity (Firmware Update Corruptions)
- Hardware SD Card Detection
- File System Initilization
- User Defined Firmware File Existance
- Firmware File CRC
- Flash Unlock/Erase/Read/Write/Lock Operations
- Comparison of Firmware File Size and User Application Flash Area

Bootloader project uses FatFs file system. 

Wrapper library (sdcard.c and sdcard.h) makes use of FatFs APIs easy.

## Usage

- This project is generated using STM32CubeIDE 
- Demo project is demonstrated on STM3240G-EVAL Board but bootloader project can be easily adapted to other microcontrollers and development boards

![sd_bootloader_flow_diagram](https://user-images.githubusercontent.com/43597729/102010994-9fa5fc00-3d52-11eb-9948-834e281aef31.jpg)

