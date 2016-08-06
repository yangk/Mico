/**
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of MXCHIP Corporation.
 *
 */

#ifndef __GAGENT_CONFIG_H_
#define __GAGENT_CONFIG_H_

//-------------------------------------------------------------------------------------------------
// DEFINES
//
#define GAGENT_MAX_DATA_SIZE                      (1024)
#define GAGENT_RESERVED_AREA1_SIZE                (8)
#define GAGENT_RESERVED_AREA2_SIZE                (8)

#define GAGENT_STARTUP_PRONOUNCE_PERIOD_S         (30)  // startup pronounce in 30s
#define GAGENT_STARTUP_PRONOUNCE_INTERVAL_S       (1)  // send startup pronounce interval 1s

#define GAGENT_CLOUD_RESTART_INTERVAL_S           (15)  // retry cloud task every 15s if not started

#define GAGENT_LAN_ENABLE
#define GAGENT_WAN_ENABLE

//------------------------------------------------------------------------------------------------
// STRUCTURES
// 

#endif  // __GAGENT_CONFIG_H_
