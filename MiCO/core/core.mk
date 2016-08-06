#
#  UNPUBLISHED PROPRIETARY SOURCE CODE
#  Copyright (c) 2016 MXCHIP Inc.
#
#  The contents of this file may not be disclosed to third parties, copied or
#  duplicated in any form, in whole or in part, without the prior written
#  permission of MXCHIP Corporation.
#

NAME := Lib_MiCO_Kernel

GLOBAL_INCLUDES := 
                   
GLOBAL_DEFINES +=  

$(NAME)_SOURCES := mico_config.c

ifneq ($(wildcard $(CURDIR)MiCO.$(MODULE).$(TOOLCHAIN_NAME).a),)
$(NAME)_PREBUILT_LIBRARY := MiCO.$(MODULE).$(TOOLCHAIN_NAME).a
else
# Build from source (MXCHIP internal)
$(error The specific MiCO library is not found! )
endif
