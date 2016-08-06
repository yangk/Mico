#
#  UNPUBLISHED PROPRIETARY SOURCE CODE
#  Copyright (c) 2016 MXCHIP Inc.
#
#  The contents of this file may not be disclosed to third parties, copied or
#  duplicated in any form, in whole or in part, without the prior written
#  permission of MXCHIP Corporation.
#

NAME := Lib_BTE
ALWAYS_OPTIMISE := 1

BUILD_BTE_LIB = 1
MINDRAY = 1             ####for mindray minimize bte size

SOURCE_ROOT :=../../../

BT_CHIP              := 20702
BT_CHIP_REVISION     := B0

BYPASS_LIBRARY_POISON_CHECK=1

include BTE_src.mk


CFLAGS += -I$(SOURCE_ROOT)include
CFLAGS += -I$(SOURCE_ROOT)tools/prebuild_stubs
CFLAGS += -I$(SOURCE_ROOT)libraries/utilities/ring_buffer
CFLAGS += -I$(SOURCE_ROOT)libraries/bluetooth/include




#GLOBAL_INCLUDES :=

SOURCES := $($(NAME)_SOURCES)
CFLAGS += $(addprefix -I,$(GLOBAL_INCLUDES)) $(addprefix -D,$(GLOBAL_DEFINES)) $(addprefix -I,$($(NAME)_INCLUDES)) $(addprefix -D,$($(NAME)_DEFINES)) $($(NAME)_CFLAGS)


KEEP_LIST_DIR := $(SOURCE_ROOT)libraries/bluetooth_bte

KEEP_LIST:= $(NAME)*.a \
            bluetooth_bte.mk

include $(SOURCE_ROOT)makefiles/mico_library_build.mk




