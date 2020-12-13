# STM32 SD Bootloader

## Description
Custom Bootloader project which enables firmware update over sd card with a lot of error handling mechanisms.
- Flash Integrity (Firmware Update Corruptions)
- Hardware SD Card Detection
- File System Initilization Handlers
- User Defined Firmware File Existance Control
- CRC Check of Firmware Binary File
- Flash Unlock/Erase/Read/Write/Lock Operation Controls
- Comparison of Firmware File Size and User Application Flash Area

Bootloader project uses FatFs file system. 

SD Card wrapper library (sdcard.c and sdcard.h) makes use of FatFs APIs easy.

## Usage

- This project is generated using STM32CubeIDE 
- Demo project is demonstrated on STM3240G-EVAL Board but bootloader project can be easily adapted to other microcontrollers and development boards

## Notes
- User Application binaries (.bin file) should include CRC32 at the end of firmware. CRC32 check is done by comparing the CRC32 value of firmware file and sd card input stream buffer.
- There are online tools that manage your binary files. User could use the below sites to calculate the CRC32 value of binary file and add CRC32 value to the end of firmware file

- https://hexed.it/
- https://emn178.github.io/online-tools/crc32_checksum.html

![sd_bootloader_flow_diagram](https://user-images.githubusercontent.com/43597729/102011482-86eb1580-3d55-11eb-9912-fcde0ddb91c7.png)

