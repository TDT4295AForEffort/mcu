################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../autogen/sl_board_default_init.c \
../autogen/sl_device_init_clocks.c \
../autogen/sl_event_handler.c 

OBJS += \
./autogen/sl_board_default_init.o \
./autogen/sl_device_init_clocks.o \
./autogen/sl_event_handler.o 

C_DEPS += \
./autogen/sl_board_default_init.d \
./autogen/sl_device_init_clocks.d \
./autogen/sl_event_handler.d 


# Each subdirectory must supply rules for building sources it contributes
autogen/sl_board_default_init.o: ../autogen/sl_board_default_init.c autogen/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DDEBUG_EFM=1' '-DEFM32GG990F1024=1' '-DSL_BOARD_NAME="BRD2200A"' '-DSL_BOARD_REV="B05"' '-DSL_COMPONENT_CATALOG_PRESENT=1' -I"/home/kasper/SimplicityStudio/v5_workspace/mcu/config" -I"/home/kasper/SimplicityStudio/v5_workspace/mcu/autogen" -I"/home/kasper/SimplicityStudio/v5_workspace/mcu" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/Device/SiliconLabs/EFM32GG/Include" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//hardware/board/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/CMSIS/Core/Include" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/service/device_init/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/emlib/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/common/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/common/toolchain/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/service/system/inc" -Os -Wall -Wextra -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -c -fmessage-length=0 -MMD -MP -MF"autogen/sl_board_default_init.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

autogen/sl_device_init_clocks.o: ../autogen/sl_device_init_clocks.c autogen/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DDEBUG_EFM=1' '-DEFM32GG990F1024=1' '-DSL_BOARD_NAME="BRD2200A"' '-DSL_BOARD_REV="B05"' '-DSL_COMPONENT_CATALOG_PRESENT=1' -I"/home/kasper/SimplicityStudio/v5_workspace/mcu/config" -I"/home/kasper/SimplicityStudio/v5_workspace/mcu/autogen" -I"/home/kasper/SimplicityStudio/v5_workspace/mcu" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/Device/SiliconLabs/EFM32GG/Include" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//hardware/board/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/CMSIS/Core/Include" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/service/device_init/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/emlib/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/common/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/common/toolchain/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/service/system/inc" -Os -Wall -Wextra -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -c -fmessage-length=0 -MMD -MP -MF"autogen/sl_device_init_clocks.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

autogen/sl_event_handler.o: ../autogen/sl_event_handler.c autogen/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DDEBUG_EFM=1' '-DEFM32GG990F1024=1' '-DSL_BOARD_NAME="BRD2200A"' '-DSL_BOARD_REV="B05"' '-DSL_COMPONENT_CATALOG_PRESENT=1' -I"/home/kasper/SimplicityStudio/v5_workspace/mcu/config" -I"/home/kasper/SimplicityStudio/v5_workspace/mcu/autogen" -I"/home/kasper/SimplicityStudio/v5_workspace/mcu" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/Device/SiliconLabs/EFM32GG/Include" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//hardware/board/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/CMSIS/Core/Include" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/service/device_init/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/emlib/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/common/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/common/toolchain/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/service/system/inc" -Os -Wall -Wextra -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -c -fmessage-length=0 -MMD -MP -MF"autogen/sl_event_handler.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


