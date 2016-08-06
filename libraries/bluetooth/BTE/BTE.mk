#
#  UNPUBLISHED PROPRIETARY SOURCE CODE
#  Copyright (c) 2016 MXCHIP Inc.
#
#  The contents of this file may not be disclosed to third parties, copied or
#  duplicated in any form, in whole or in part, without the prior written
#  permission of MXCHIP Corporation.
#

NAME := Lib_Bluetooth_Embedded_Stack_for_MiCO

GLOBAL_INCLUDES := . \
                   ../include \
                   WICED \
                   Components/stack/include \
                   Projects/bte/main
                   
GLOBAL_DEFINES +=  BUILDCFG

ifneq ($(wildcard $(CURDIR)Lib_BTE.$(HOST_ARCH).$(TOOLCHAIN_NAME).release.a),)
$(NAME)_PREBUILT_LIBRARY := Lib_BTE.$(HOST_ARCH).$(TOOLCHAIN_NAME).release.a
else
# Build from source
include $(CURDIR)BTE_src.mk
endif
