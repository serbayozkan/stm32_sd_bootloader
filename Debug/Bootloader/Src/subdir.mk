################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bootloader/Src/bl.c \
../Bootloader/Src/crc.c \
../Bootloader/Src/flash.c \
../Bootloader/Src/sdcard.c 

OBJS += \
./Bootloader/Src/bl.o \
./Bootloader/Src/crc.o \
./Bootloader/Src/flash.o \
./Bootloader/Src/sdcard.o 

C_DEPS += \
./Bootloader/Src/bl.d \
./Bootloader/Src/crc.d \
./Bootloader/Src/flash.d \
./Bootloader/Src/sdcard.d 


# Each subdirectory must supply rules for building sources it contributes
Bootloader/Src/bl.o: ../Bootloader/Src/bl.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I../FATFS/App -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I"E:/GitRepo/stm32_sd_bootloader/Bootloader/Inc" -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../FATFS/Target -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Bootloader/Src/bl.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Bootloader/Src/crc.o: ../Bootloader/Src/crc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I../FATFS/App -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I"E:/GitRepo/stm32_sd_bootloader/Bootloader/Inc" -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../FATFS/Target -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Bootloader/Src/crc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Bootloader/Src/flash.o: ../Bootloader/Src/flash.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I../FATFS/App -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I"E:/GitRepo/stm32_sd_bootloader/Bootloader/Inc" -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../FATFS/Target -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Bootloader/Src/flash.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Bootloader/Src/sdcard.o: ../Bootloader/Src/sdcard.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I../FATFS/App -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I"E:/GitRepo/stm32_sd_bootloader/Bootloader/Inc" -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../FATFS/Target -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Bootloader/Src/sdcard.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

