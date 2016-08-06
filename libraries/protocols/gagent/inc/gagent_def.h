/**
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of MXCHIP Corporation.
 *
 */
#ifndef __GAGENT_DEF_H_
#define __GAGENT_DEF_H_

#include "mico.h"

//-------------------------------------------------------------------------------------------------
// DEFINES
// 

// lib version 
#define GAGENT_SDK_MAIN_VERSION       (0x00)
#define GAGENT_SDK_SUB_VERSION        (0x01)
#define GAGENT_SDK_REV_VERSION        (0x01)
#define GAGENT_SDK_VERSION        ((GAGENT_SDK_MAIN_VERSION << 16) | (GAGENT_SDK_SUB_VERSION << 8 ) | (GAGENT_SDK_REV_VERSION))

// log level set bit mask
#define  GAGENT_LOG_ERROR_BIT        (0)
#define  GAGENT_LOG_WARNING_BIT      (1)
#define  GAGENT_LOG_INFO_BIT         (2)
#define  GAGENT_LED_SW_BIT           (3)

#define GAGENT_ERROR_LOG_MASK        (1 << GAGENT_LOG_ERROR_BIT)
#define GAGENT_WARNING_LOG_MASK      (1 << GAGENT_LOG_WARNING_BIT)
#define GAGENT_INFO_LOG_MASK         (1 << GAGENT_LOG_INFO_BIT)
#define GAGENT_ALL_LOG_MASK          (GAGENT_ERROR_LOG_MASK | GAGENT_WARNING_LOG_MASK | GAGENT_INFO_LOG_MASK)
// led switch bit mask
#define GAGENT_WIFI_LED_SWITCH_MASK  (1 << GAGENT_LED_SW_BIT)

// log level
#define GAGENT_LOG_LEVEL_NONE        (0x00)
#define GAGENT_LOG_LEVEL_ERROR       (0x00 | GAGENT_ERROR_LOG_MASK)
#define GAGENT_LOG_LEVEL_WARNING     (0x00 | GAGENT_ERROR_LOG_MASK | GAGENT_WARNING_LOG_MASK)
#define GAGENT_LOG_LEVEL_INFO        (0x00 | GAGENT_ERROR_LOG_MASK | GAGENT_WARNING_LOG_MASK | GAGENT_INFO_LOG_MASK)
#define GAGENT_LOG_LEVEL_ALL         (0x00 | GAGENT_ALL_LOG_MASK)

#define GAGENT_LOG_ERROR             (0x00 | GAGENT_ERROR_LOG_MASK)
#define GAGENT_LOG_WARNING           (0x00 | GAGENT_WARNING_LOG_MASK)
#define GAGENT_LOG_INFO              (0x00 | GAGENT_INFO_LOG_MASK)

#define GAGENT_APP2DEV_DATA_MCU_RETRY_CNT   (3)
#define GAGENT_APP2DEV_DATA_MCU_TIMEOUT     (200)

#define GAGENT_TRUE     (1)
#define GAGENT_FALSE    (0)

#define GAGENT_PRODUCT_KEY_LEN              32
#define GAGENT_HARDWARE_VER_LEN             8
#define GAGENT_SOFTWARE_VER_LEN             8

#define GAGENT_PASSCODE_MAXLEN              32
#define GAGENT_DID_LEN                      24
#define GAGENT_FIRMWARELEN                  8

#define GAGENT_GENERAL_PROTOCOL_VER_LEN     8
#define GAGENT_BUSINESS_PROTOCOL_VER_LEN    8
#define GAGENT_ATTRIBUTES_LEN               8

#define GAGENT_MAC_LEN                      12
#define GAGENT_IP_LEN                       16

#define GAGENT_OTA_MAX_MD5_SIZE             32

//GAgentStatus
#define WIFI_MODE_AP                  (1<<0)
#define WIFI_MODE_STATION             (1<<1)
#define WIFI_MODE_ONBOARDING          (1<<2)
#define WIFI_MODE_BINDING             (1<<3)
#define WIFI_STATION_CONNECTED        (1<<4)
#define WIFI_CLOUD_CONNECTED          (1<<5)
#define WIFI_LEVEL                    (7<<8)
#define WIFI_CLIENT_ON                (1<<11)
#define WIFI_MODE_TEST                (1<<12)

#define WIFI_GAGENTSTATUS_MASK       0x1FFF

//------------------------------------------------------------------------------------------------
// STRUCTURES
//
typedef enum _gagent_errcode_t {
        GAGENT_OK = 0,
        GAGENT_ERR_UNKNOWN = -1,
        GAGENT_ERR_INVALID_PARAMS = -2,
        GAGENT_ERR_NOT_FOUND = -3,
        GAGENT_ERR_NO_MEMORY = -4,
        GAGENT_ERR_CREATE_THREAD_ERR = -5,
        
        GAGENT_ERR_GAL_BBASE = -10,
        GAGENT_ERR_GAL_INIT_ERR = GAGENT_ERR_GAL_BBASE-1,
        GAGENT_ERR_GAL_START_FAILED = GAGENT_ERR_GAL_BBASE-2,
        GAGENT_ERR_GAL_HANDLE_ERR = GAGENT_ERR_GAL_BBASE-3,
        GAGENT_ERR_GAL_SEND_ERR = GAGENT_ERR_GAL_BBASE-4,
        GAGENT_ERR_GAL_RECV_ERR = GAGENT_ERR_GAL_BBASE-5,
        GAGENT_ERR_GAL_FRAME_ERR = GAGENT_ERR_GAL_BBASE-6,
        GAGENT_ERR_GAL_DEAL_PACKET_ERR = GAGENT_ERR_GAL_BBASE-7,
        GAGENT_ERR_GAL_CREATE_ACK_ERR = GAGENT_ERR_GAL_BBASE-8,
        GAGENT_ERR_GAL_CREATE_ACK_PAYLOAD_ERR = GAGENT_ERR_GAL_BBASE-9,
        GAGENT_ERR_GAL_CREATE_FRAME_ERR = GAGENT_ERR_GAL_BBASE-10,
        GAGENT_ERR_GAL_STOP_FAILED = GAGENT_ERR_GAL_BBASE-11,

        GAGENT_ERR_GAC_BBASE = -50,
        GAGENT_ERR_GAC_INIT_ERR = GAGENT_ERR_GAC_BBASE-1,
        GAGENT_ERR_GAC_REGISTER_FAILED = GAGENT_ERR_GAC_BBASE-2,
        GAGENT_ERR_GAC_PROVISION_FAILED = GAGENT_ERR_GAC_BBASE-3,
        GAGENT_ERR_GAC_OTA_FAILED = GAGENT_ERR_GAC_BBASE-4,
        GAGENT_ERR_GAC_START_FAILED = GAGENT_ERR_GAC_BBASE-5,
        GAGENT_ERR_GAC_STOP_FAILED = GAGENT_ERR_GAC_BBASE-6,
        GAGENT_ERR_GAC_DEINIT_FAILED = GAGENT_ERR_GAC_BBASE-7,
        GAGENT_ERR_GAC_HANDLE_ERR = GAGENT_ERR_GAC_BBASE-8,
        GAGENT_ERR_GAC_PACKET_ERR = GAGENT_ERR_GAC_BBASE-9,
        GAGENT_ERR_GAC_DEAL_PACKET_ERR = GAGENT_ERR_GAC_BBASE-10,
        GAGENT_ERR_GAC_GET_STATUS_ERR = GAGENT_ERR_GAC_BBASE-11,
        GAGENT_ERR_GAC_SEND_ERR = GAGENT_ERR_GAC_BBASE-12,
        GAGENT_ERR_GAC_RECV_ERR = GAGENT_ERR_GAC_BBASE-13,
        GAGENT_ERR_GAC_FRAME_ERR = GAGENT_ERR_GAC_BBASE-14,
        GAGENT_ERR_GAC_UNKNOWN_TOPIC = GAGENT_ERR_GAC_BBASE-15,
        GAGENT_ERR_GAC_CONFIG_SOFTAP_MSG_ERR = GAGENT_ERR_GAC_BBASE-16,
        GAGENT_ERR_GAC_CREATE_ACK_ERR = GAGENT_ERR_GAC_BBASE-17
}gagent_errcode_t; 

typedef enum _gagent_log_type_t {
  GAGENT_LOG_TYPE_LAN = 1,
  GAGENT_LOG_TYPE_CLOUD,
  GAGENT_LOG_TYPE_ALL
}gagent_log_type_t;

typedef struct _mcu_info_t {
  uint8_t general_protocol_ver[GAGENT_GENERAL_PROTOCOL_VER_LEN+1];  // Wi-Fi<->MCU protocol
  uint8_t business_protocol_ver[GAGENT_BUSINESS_PROTOCOL_VER_LEN+1];  // user-defined product data point
  uint8_t hardware_ver[GAGENT_HARDWARE_VER_LEN+1];  // MCU hardware
  uint8_t software_ver[GAGENT_SOFTWARE_VER_LEN+1];  // MCU control software
  int8_t product_key[GAGENT_PRODUCT_KEY_LEN+1];
  uint16_t bindable_time;  // local control bindable time
  uint8_t attributes[GAGENT_ATTRIBUTES_LEN+1];  // mcu extra attributes
} mcu_info_t;

typedef struct  _client_info_t{
  uint32_t lan_client_online_num;
  uint32_t cloud_client_online_num;
}client_info_t;

typedef struct _wifi_info_t{
  uint8_t moduletype;
  uint8_t general_protocol_ver[GAGENT_GENERAL_PROTOCOL_VER_LEN+1];
  uint8_t hardware_ver[GAGENT_HARDWARE_VER_LEN+1];
  uint8_t software_ver[GAGENT_SOFTWARE_VER_LEN+1];
  int8_t mac[GAGENT_MAC_LEN+1];
  //uint8_t ip[GAGENT_IP_LEN+1];
  uint8_t attributes[GAGENT_ATTRIBUTES_LEN+1];
} wifi_info_t;

typedef struct _gagent_context {
  int8_t            passcode[GAGENT_PASSCODE_MAXLEN+1];
  int8_t            did[GAGENT_DID_LEN+1];
  int8_t            FirmwareVer[GAGENT_FIRMWARELEN+1];
 
  wifi_info_t       wifi_info;
  mcu_info_t        mcu_info;
  
  uint8_t           loglevel;
  void              *user_context;
  
  uint16_t          wifi_status;
  client_info_t     client_info;
}gagent_context;

typedef struct _gagent_ota_params_t{
  // firmware info
  uint32_t ota_firmware_size;
  int8_t ota_firmware_md5[GAGENT_OTA_MAX_MD5_SIZE+1];
  void *user_context;
  // runtime info
  uint32_t wrote_offset;  // need return to gagent to get next data
  int8_t calc_md5[GAGENT_OTA_MAX_MD5_SIZE];
}gagent_ota_params_t;

#endif  // __GAGENT_DEF_H_
