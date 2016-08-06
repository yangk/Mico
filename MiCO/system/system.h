/**
 ******************************************************************************
 * @file    system.h
 * @author  William Xu
 * @version V1.0.0
 * @date    22-July-2015
 * @brief   This file provide internal function prototypes for for mico system.
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

#pragma once

#include "common.h"
#include "mico_rtos.h"
#include "mico_wlan.h"

#ifndef MICO_PREBUILT_LIBS
#include "mico_config.h"
#endif

#ifdef MICO_BLUETOOTH_ENABLE
#include "mico_bt_nvram_access.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SYSTEM_VER_MAJOR    (2)
#define SYSTEM_VER_MINOR    (5)
#define SYSTEM_VER_REVISION (0)

#ifndef MICO_SYSTEM_LOG_INFO_DISABLE
  #define system_log(M, ...) custom_log("SYSTEM", M, ##__VA_ARGS__)
  #define system_log_trace() custom_log_trace("SYSTEM")
#else
  #define system_log(M, ...)
  #define system_log_trace()
#endif

/* Define MICO service thread stack size */
#define STACK_SIZE_LOCAL_CONFIG_SERVER_THREAD   0x300
#define STACK_SIZE_LOCAL_CONFIG_CLIENT_THREAD   0x650
#define STACK_SIZE_NTP_CLIENT_THREAD            0x550
#define STACK_SIZE_mico_system_MONITOR_THREAD   0x300

#define EASYLINK_BYPASS_NO                      (0)
#define EASYLINK_BYPASS                         (1)
#define EASYLINK_SOFT_AP_BYPASS                 (2)

#define maxSsidLen          32
#define maxKeyLen           64
#define maxNameLen          32
#define maxIpLen            16

#define SYS_MAGIC_NUMBR     (0xA43E2165)

typedef enum
{
  eState_Normal,
  eState_Software_Reset,
  eState_Wlan_Powerdown,
  eState_Restore_default,
  eState_Standby,
} system_state_t;

typedef enum  {
  /*All settings are in default state, module will enter easylink mode when powered on. 
  Press down Easyink button for 3 seconds (defined by RestoreDefault_TimeOut) to enter this mode */
  unConfigured,
  /*Module will enter easylink mode temperaly when powered on, and go back to allConfigured
    mode after time out (Defined by EasyLink_TimeOut), This mode is used for changing wlan
    settings if module is moved to a new wlan enviroment. Press down Easyink button to
    enter this mode */                
  wLanUnConfigured,
  /*Normal working mode, module use the configured settings to connecte to wlan, and run 
    user's threads*/
  allConfigured,
  /*If MFG_MODE_AUTO is enabled and MICO settings are erased (maybe a fresh device just has 
    been programed or MICO settings is damaged), module will enter MFG mode when powered on. */
  mfgConfigured
}Config_State_t;

/* Upgrade iamge should save this table to flash */
typedef struct  _boot_table_t {
  uint32_t start_address; // the address of the bin saved on flash.
  uint32_t length; // file real length
  uint8_t version[8];
  uint8_t type; // B:bootloader, P:boot_table, A:application, D: 8782 driver
  uint8_t upgrade_type; //u:upgrade, 
  uint16_t crc;
  uint8_t reserved[4];
}boot_table_t;

typedef struct _mico_sys_config_t
{
  /*Device identification*/
  char            name[maxNameLen];

  /*Wi-Fi configuration*/
  char            ssid[maxSsidLen];
  char            user_key[maxKeyLen]; 
  int             user_keyLength;
  char            key[maxKeyLen]; 
  int             keyLength;
  char            bssid[6];
  int             channel;
  SECURITY_TYPE_E security;

  /*Power save configuration*/
  bool            rfPowerSaveEnable;
  bool            mcuPowerSaveEnable;

  /*Local IP configuration*/
  bool            dhcpEnable;
  char            localIp[maxIpLen];
  char            netMask[maxIpLen];
  char            gateWay[maxIpLen];
  char            dnsServer[maxIpLen];

  /*EasyLink configuration*/
  Config_State_t  configured;
  uint8_t         easyLinkByPass;
  uint32_t        reserved;

  /*Services in MICO system*/
  uint32_t        magic_number;

  /*Update seed number when configuration is changed*/
  int32_t         seed;
} mico_sys_config_t;


#ifdef MICO_BLUETOOTH_ENABLE
//#pragma pack(1)
typedef PACKED struct
{
//    uint8_t bluetooth_local_addeess[6];
//    uint8_t bluetooth_local_name[249]; /* including null termination */
    uint8_t bluetooth_local_key[MICO_BT_PARA_LOCAL_KEY_DATA];
    uint8_t bluetooth_remote_key[MICO_BT_DCT_ENTRY_HDR_LENGTH + MICO_BT_DCT_MAX_KEYBLOBS][MICO_BT_DCT_MAX_DEVICES];
    //uint8_t padding[1];   /* to ensure 32-bit aligned size */
} mico_bt_config_t;
//#pragma pack()
#endif


typedef struct _flash_configuration_t {

  /*OTA options*/
  boot_table_t             bootTable;

  /*MICO system core configuration*/
  mico_sys_config_t        micoSystemConfig;

#ifdef MICO_BLUETOOTH_ENABLE
  mico_bt_config_t         bt_config; 
#endif
} flash_content_t;

typedef struct _current_mico_status_t 
{
  system_state_t        current_sys_state;
  mico_semaphore_t      sys_state_change_sem;
  /*MICO system Running status*/
  char                  localIp[maxIpLen];
  char                  netMask[maxIpLen];
  char                  gateWay[maxIpLen];
  char                  dnsServer[maxIpLen];
  char                  mac[18];
  char                  rf_version[50];
} current_mico_status_t;

typedef struct _mico_Context_t
{
  /*Flash content*/
  flash_content_t           flashContentInRam;
  mico_mutex_t              flashContentInRam_mutex;

  void *                    user_config_data;
  uint32_t                  user_config_data_size;

  /*Running status*/
  current_mico_status_t     micoStatus;
} system_context_t;

typedef enum {
  NOTIFY_STATION_UP = 1,
  NOTIFY_STATION_DOWN,

  NOTIFY_AP_UP,
  NOTIFY_AP_DOWN,
} notify_wlan_t; 


OSStatus system_notification_init( system_context_t * const inContext);

OSStatus system_network_daemen_start( system_context_t * const inContext );

OSStatus system_discovery_init( system_context_t * const inContext );

void system_connect_wifi_normal( system_context_t * const inContext );

void system_connect_wifi_fast( system_context_t * const inContext);

OSStatus system_easylink_wac_start( system_context_t * const inContext );

OSStatus system_easylink_wac_stop( void );

OSStatus system_easylink_start( system_context_t * const inContext );

OSStatus MICORestoreMFG                 ( system_context_t * const inContext );

OSStatus MICOReadConfiguration          ( system_context_t * const inContext );

void mico_mfg_test( system_context_t * const inContext );


#ifdef __cplusplus
} /*extern "C" */
#endif

