#
#  UNPUBLISHED PROPRIETARY SOURCE CODE
#  Copyright (c) 2016 MXCHIP Inc.
#
#  The contents of this file may not be disclosed to third parties, copied or
#  duplicated in any form, in whole or in part, without the prior written
#  permission of MXCHIP Corporation.
#


NAME = STM32F4xx

# Host architecture is ARM Cortex M4
HOST_ARCH := Cortex-M4

# Host MCU alias for OpenOCD
HOST_OPENOCD := stm32f4x

GLOBAL_INCLUDES := . \
                   .. \
                   ../.. \
                   ../../include \
                   ../../$(HOST_ARCH) \
                   ../../$(HOST_ARCH)/CMSIS \
                   peripherals

# Global defines
GLOBAL_DEFINES  := USE_STDPERIPH_DRIVER \
                   _STM3x_ \
                   _STM32x_

# Convert the MCU variant into the required STM peripheral library constant
ifneq (,$(filter $(HOST_MCU_VARIANT), STM32F405 STM32F415 STM32F407 STM32F417))
GLOBAL_DEFINES += STM32F40_41xxx
endif
ifneq (,$(filter $(HOST_MCU_VARIANT), STM32F427 STM32F437))
GLOBAL_DEFINES += STM32F427_437xx
endif
ifneq (,$(filter $(HOST_MCU_VARIANT), STM32F429 STM32F439))
GLOBAL_DEFINES += STM32F429_439xx
endif
ifneq (,$(filter $(HOST_MCU_VARIANT), STM32F401))
GLOBAL_DEFINES += STM32F401xx
endif
ifneq (,$(filter $(HOST_MCU_VARIANT), STM32F411))
GLOBAL_DEFINES += STM32F411xE
endif
ifneq (,$(filter $(HOST_MCU_VARIANT), STM32F446))
GLOBAL_DEFINES += STM32F446xx
endif

# Global flags
GLOBAL_CFLAGS   += $$(CPU_CFLAGS)    $$(ENDIAN_CFLAGS_LITTLE)
GLOBAL_CXXFLAGS += $$(CPU_CXXFLAGS)  $$(ENDIAN_CXXFLAGS_LITTLE)
GLOBAL_ASMFLAGS += $$(CPU_ASMFLAGS)  $$(ENDIAN_ASMFLAGS_LITTLE)
GLOBAL_LDFLAGS  += $$(CPU_LDFLAGS)   $$(ENDIAN_LDFLAGS_LITTLE)


ifeq ($(TOOLCHAIN_NAME),GCC)
GLOBAL_LDFLAGS  += -nostartfiles
GLOBAL_LDFLAGS  += -Wl,--defsym,__STACKSIZE__=$$($(RTOS)_START_STACK)
GLOBAL_LDFLAGS  += -L ./WICED/platform/MCU/$(NAME)/$(TOOLCHAIN_NAME) \
                   -L ./WICED/platform/MCU/$(NAME)/$(TOOLCHAIN_NAME)/$(HOST_MCU_VARIANT)
else
ifeq ($(TOOLCHAIN_NAME),IAR)
GLOBAL_LDFLAGS  += --config_def __STACKSIZE__=$$($(RTOS)_START_STACK)
endif
endif

# Components
$(NAME)_COMPONENTS += $(TOOLCHAIN_NAME)
$(NAME)_COMPONENTS += MCU/STM32F4xx/peripherals
$(NAME)_COMPONENTS += utilities

# Source files
$(NAME)_SOURCES := ../../$(HOST_ARCH)/crt0_$(TOOLCHAIN_NAME).c \
                   ../../$(HOST_ARCH)/hardfault.c \
                   ../mico_platform_common.c \
                   ../wlan_platform_common.c \
                   platform_init.c \
                   GCC/platform_unhandled_isr.c

#for DCT with crc checking
# $(NAME)_COMPONENTS  += utilities/crc


ifndef NO_WIFI
$(NAME)_SOURCES += wlan_bus_driver/wlan_platform.c \
                   wlan_bus_driver/wlan_bus_$(BUS).c
endif

# These need to be forced into the final ELF since they are not referenced otherwise
$(NAME)_LINK_FILES := ../../$(HOST_ARCH)/crt0_$(TOOLCHAIN_NAME).o \
                      ../../$(HOST_ARCH)/hardfault.o 

# $(NAME)_CFLAGS = $(COMPILER_SPECIFIC_PEDANTIC_CFLAGS)

# Add maximum and default watchdog timeouts to definitions. Warning: Do not change MAX_WATCHDOG_TIMEOUT_SECONDS
MAX_WATCHDOG_TIMEOUT_SECONDS = 22
GLOBAL_DEFINES += MAX_WATCHDOG_TIMEOUT_SECONDS=$(MAX_WATCHDOG_TIMEOUT_SECONDS)

# DCT linker script
DCT_LINK_SCRIPT += $(TOOLCHAIN_NAME)/$(HOST_MCU_VARIANT)/dct$(LINK_SCRIPT_SUFFIX)

ifeq ($(APP),bootloader)
####################################################################################
# Building bootloader
####################################################################################

DEFAULT_LINK_SCRIPT += $(TOOLCHAIN_NAME)/bootloader$(LINK_SCRIPT_SUFFIX)
#$(NAME)_SOURCES     += WAF/waf_platform.c
#$(NAME)_LINK_FILES  += WAF/waf_platform.o
GLOBAL_INCLUDES     += 
$(NAME)_SOURCES     += GCC/platform_vector_table_bootloader.c
$(NAME)_LINK_FILES  += GCC/platform_vector_table_bootloader.o

else
#ifneq ($(filter sflash_write, $(APP)),)
ifneq ($(filter sflash_write, $(APP)),)
####################################################################################
# Building sflash_write
####################################################################################

PRE_APP_BUILDS      += bootloader
DEFAULT_LINK_SCRIPT := $(TOOLCHAIN_NAME)/app_ram$(LINK_SCRIPT_SUFFIX)
GLOBAL_INCLUDES     += WAF ../../../../../apps/waf/bootloader/
GLOBAL_DEFINES      += __JTAG_FLASH_WRITER_DATA_BUFFER_SIZE__=16384
ifeq ($(TOOLCHAIN_NAME),IAR)
GLOBAL_LDFLAGS      += --config_def __JTAG_FLASH_WRITER_DATA_BUFFER_SIZE__=16384
endif

else
ifeq ($(USES_BOOTLOADER_OTA),1)
####################################################################################
# Building standard application to run with bootloader
####################################################################################

PRE_APP_BUILDS      += bootloader
DEFAULT_LINK_SCRIPT := $(TOOLCHAIN_NAME)/app_with_bootloader$(LINK_SCRIPT_SUFFIX)
GLOBAL_INCLUDES     += 
$(NAME)_SOURCES     += GCC/platform_vector_table.c
$(NAME)_LINK_FILES  += GCC/platform_vector_table.o

else
####################################################################################
# Building a WWD application (standalone app without bootloader and DCT)
####################################################################################

DEFAULT_LINK_SCRIPT := $(TOOLCHAIN_NAME)/app_no_bootloader$(LINK_SCRIPT_SUFFIX)
GLOBAL_DEFINES      += WICED_DISABLE_BOOTLOADER
GLOBAL_INCLUDES     += 
$(NAME)_SOURCES     += GCC/platform_vector_table.c
$(NAME)_LINK_FILES  += GCC/platform_vector_table.o

endif # USES_BOOTLOADER_OTA = 1
endif # APP= sflash_write
endif # APP=bootloader

