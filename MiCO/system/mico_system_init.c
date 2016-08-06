/**
 ******************************************************************************
 * @file    mico_system_init.c
 * @author  William Xu
 * @version V1.0.0
 * @date    05-May-2014
 * @brief   This file provide the mico system initialize function.
 ******************************************************************************
 *
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of MXCHIP Corporation.
 ******************************************************************************
 */

#include <time.h>

#include "mico.h"

#ifdef AIRKISS_DISCOVERY_ENABLE
#include "easylink/airkiss_discovery.h"
#endif


/******************************************************
 *               Variables Definitions
 ******************************************************/


OSStatus mico_system_init( mico_Context_t* in_context )
{
  OSStatus err = kNoErr;

  require_action( in_context, exit, err = kNotPreparedErr );

  /* Initialize mico notify system */
  err = system_notification_init( in_context );
  require_noerr( err, exit ); 

#ifdef MICO_SYSTEM_MONITOR_ENABLE
  /* MiCO system monitor */
  err = mico_system_monitor_daemen_start( );
  require_noerr( err, exit ); 
#endif

#ifdef MICO_CLI_ENABLE
  /* MiCO command line interface */
  cli_init();
#endif

  /* Network PHY driver and tcp/ip static init */
  err = system_network_daemen_start( in_context );
  require_noerr( err, exit ); 

#ifdef MICO_WLAN_CONNECTION_ENABLE
  if( in_context->flashContentInRam.micoSystemConfig.configured == wLanUnConfigured ||
      in_context->flashContentInRam.micoSystemConfig.configured == unConfigured){
    system_log("Empty configuration. Starting configuration mode...");

#if (MICO_CONFIG_MODE == CONFIG_MODE_EASYLINK) || \
    (MICO_CONFIG_MODE == CONFIG_MODE_SOFT_AP) ||  \
    (MICO_CONFIG_MODE == CONFIG_MODE_USER) ||  \
    (MICO_CONFIG_MODE == CONFIG_MODE_EASYLINK_WITH_SOFTAP)
    err = system_easylink_start( in_context );
    require_noerr( err, exit );
#elif ( MICO_CONFIG_MODE == CONFIG_MODE_WAC)
    err = system_easylink_wac_start( in_context );
    require_noerr( err, exit );
#else
    #error "Wi-Fi configuration mode is not defined"
#endif
  }
#ifdef MFG_MODE_AUTO
  else if( in_context->flashContentInRam.micoSystemConfig.configured == mfgConfigured ){
    system_log( "Enter MFG mode automatically" );
    mico_mfg_test( in_context );
    mico_thread_sleep( MICO_NEVER_TIMEOUT );
  }
#endif
  else{
    system_log("Available configuration. Starting Wi-Fi connection...");
    system_connect_wifi_fast( in_context );
  }
#endif
  
  /* System discovery */
#ifdef MICO_SYSTEM_DISCOVERY_ENABLE
  system_discovery_init( in_context );
#endif
  
  /*Local configuration server*/
#ifdef MICO_CONFIG_SERVER_ENABLE
  config_server_start( in_context );
#endif
  
#ifdef AIRKISS_DISCOVERY_ENABLE
  err = airkiss_discovery_start( AIRKISS_APP_ID, AIRKISS_DEVICE_ID );
  require_noerr( err, exit );
#endif

  require_noerr_action( err, exit, system_log("Closing main thread with err num: %d.", err) );
exit:
  
  return err;
}


