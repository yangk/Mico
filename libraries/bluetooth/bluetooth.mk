#
# Copyright 2016, MXCHIP Corporation
# All Rights Reserved.
#

NAME := Lib_MiCO_Bluetooth_Driver_for_$(BT_CHIP)$(BT_CHIP_REVISION)

BTE_PLATFORM_DIR := BTE/mico_bt_platform

GLOBAL_INCLUDES += include

# Include BTE stack as component
$(NAME)_COMPONENTS := libraries/bluetooth/BTE

ifeq ($(BT_CHIP_XTAL_FREQUENCY),)
$(NAME)_SOURCES := firmware/$(BT_CHIP)$(BT_CHIP_REVISION)/bt_firmware_image.c
else
$(NAME)_SOURCES := firmware/$(BT_CHIP)$(BT_CHIP_REVISION)/$(BT_CHIP_XTAL_FREQUENCY)/bt_firmware_image.c
endif

$(NAME)_SOURCES += $(BTE_PLATFORM_DIR)/mico_bt_bus.c \
                   $(BTE_PLATFORM_DIR)/mico_bt_hcd.c \
                   $(BTE_PLATFORM_DIR)/mico_bt_logmsg.c \
                   $(BTE_PLATFORM_DIR)/mico_bt_nvram_access.c \
                   $(BTE_PLATFORM_DIR)/mico_upio.c
                   