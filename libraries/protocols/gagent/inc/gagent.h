/**
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of MXCHIP Corporation.
 *
 */

#ifndef __GAGENT_H_
#define __GAGENT_H_

#include "gagent_def.h"
#include "gagent_config.h"

//------------------------------- USER APIs-------------------------------------
//get sdk version
// byte3-byte2-byte1-byte0
// <00>-<main_ver>-<sub_ver>-<rev>
uint32_t gagent_sdk_version(void);

// gagent start to connect
// return GAGENT_OK: success
//        else return error code(<0)
int32_t gagent_connection_start(gagent_context *gagent_ctx);

// reset device from cloud
// return GAGENT_OK: success
//        else return error code(<0)
int32_t gagent_connection_reset(void);

// upload mcu data to apps && cloud
// return GAGENT_OK: success
//        else return error code(<0)
int32_t gagent_upload_mcu_data_to_all_apps(uint32_t sn, uint8_t *data, uint32_t data_len);

// send ack for app2dev data which need ack
// return GAGENT_OK: success
//        else return error code(<0)
int32_t gagent_app2dev_ack(uint32_t sn, uint8_t *data, uint32_t data_len);

// upload log to apps && cloud
// return GAGENT_OK: success
//        else return error code(<0)
int32_t gagent_upload_log(gagent_log_type_t log_type, uint8_t log_level, int8_t *log_tag, 
                          int8_t *log_origin, int8_t *log, uint32_t log_len);

// open bind mode
// bindable:        1: open bind mode, 0: close bind mode
// bindable_time_s: 0: never timeout, >0: close bind mode after seconds (bindable==1) 
void gagent_set_bind_mode(uint8_t bindable, uint16_t bindable_time_s);

// helper functions
int _format_mac(char *dst_mac, char *src_mac, int len);
char* _DataToHexStringLowercase( const uint8_t *inBuf, size_t inBufLen );
void make_rand_passcode(int8_t *data);

//--------------------------------- CALLBACKS ----------------------------------
// cloud status changed
extern void gagent_delegate_cloud_connected(void);
extern void gagent_delegate_cloud_disconnected(void);
extern void gagent_delegate_cloud_online_app_amount_changed(uint32_t app_amount);
// process app2dev data
extern void gagent_delegate_process_app2dev_data(uint8_t *payload, uint32_t payload_len);
extern void gagent_delegate_process_app2dev_data_with_ack(uint8_t *payload, uint32_t payload_len, uint16_t sn);
// setings
extern void gagent_delegate_update_device_id(int8_t *did, uint16_t did_len);
extern void gagent_delegate_update_loglevel(uint8_t loglevel);

// ota version compare
// return GAGENT_TRUE: do ota;
//        GAGENT_FALSE: not need ota.
extern int32_t gagent_delegate_cloud_ota_version_check(int8_t *soft_ver, int8_t *url, void *hctx);

// ota data process
// return GAGENT_TRUE: process data ok;
//        GAGENT_FALSE: process data faild.
extern int32_t gagent_delegate_cloud_ota_data_process(uint32_t offset, uint8_t *data, uint32_t data_len, gagent_ota_params_t *ota_params);

#endif  // __GAGENT_H_
