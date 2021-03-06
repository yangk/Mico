/**
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of MXCHIP Corporation.
 *
 */

#pragma once

/** @file
 *  Smartbridge's Interface Header with Bluetooth Stack
 */

//#include "mico_utilities.h"
#include "mico_bt_smartbridge.h"
#include "mico_bt_smart_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#define ATT_DEFAULT_MTU           (23)
#define ATT_STANDARD_VALUE_LENGTH (ATT_DEFAULT_MTU - 3)
#define ATT_STANDARD_TIMEOUT      (500)

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

OSStatus smartbridge_bt_interface_initialize( void );
OSStatus smartbridge_bt_interface_deinitialize( void );

OSStatus  smartbridge_bt_interface_stop_scan( void  );

OSStatus  smartbridge_bt_interface_start_scan( const mico_bt_smart_scan_settings_t* setting, mico_bt_smart_scan_complete_callback_t complete_callback, mico_bt_smart_advertising_report_callback_t advertising_report_callback );
mico_bool_t    smartbridge_bt_interface_is_scanning( void );

OSStatus  smartbridge_bt_interface_connect( const mico_bt_smart_device_t* remote_device, const mico_bt_smart_connection_settings_t* settings, mico_bt_smartbridge_disconnection_callback_t disconnection_callback, mico_bt_smartbridge_notification_callback_t notification_callback );

OSStatus  smartbridge_bt_interface_cancel_last_connect( mico_bt_device_address_t address );

OSStatus  smartbridge_bt_interface_disconnect( uint16_t connection_handle );

mico_bool_t  smartbridge_bt_interface_add_device_to_whitelist( const mico_bt_device_address_t* device_address, mico_bt_smart_address_type_t address_type );

mico_bool_t  smartbridge_bt_interface_remove_device_from_whitelist( const mico_bt_device_address_t* device_address, mico_bt_smart_address_type_t address_type );

OSStatus  smartbridge_bt_interface_get_whitelist_size( uint32_t *size );

OSStatus  smartbridge_bt_interface_clear_whitelist( void );

OSStatus  smartbridge_bt_interface_set_attribute_timeout( uint32_t timeout_seconds );

OSStatus  smartbridge_bt_interface_set_connection_tx_power( uint16_t connection_handle, int8_t transmit_power_dbm );

OSStatus  smartbridge_bt_interface_set_max_concurrent_connections( uint8_t count );


OSStatus smartbridge_bt_interface_discover_all_primary_services( uint16_t connection_handle, mico_bt_smart_attribute_list_t* service_list );
OSStatus smartbridge_bt_interface_discover_all_characteristics_in_a_service( uint16_t connection_handle, uint16_t start_handle, uint16_t end_handle, mico_bt_smart_attribute_list_t* characteristic_list );
OSStatus smartbridge_bt_interface_discover_all_characteristic_descriptors( uint16_t connection_handle, uint16_t start_handle, uint16_t end_handle, mico_bt_smart_attribute_list_t* no_value_descriptor_list );
OSStatus smartbridge_bt_interface_discover_primary_services_by_uuid( uint16_t connection_handle, const mico_bt_uuid_t* uuid, mico_bt_smart_attribute_list_t* service_list );
OSStatus smartbridge_bt_interface_find_included_services( uint16_t connection_handle, uint16_t start_handle, uint16_t end_handle, mico_bt_smart_attribute_list_t* include_list );
OSStatus smartbridge_bt_interface_discover_characteristic_by_uuid( uint16_t connection_handle, const mico_bt_uuid_t* uuid, uint16_t start_handle, uint16_t end_handle, mico_bt_smart_attribute_list_t* characteristic_list );

OSStatus smartbridge_bt_interface_read_characteristic_value( uint16_t connection_handle, uint16_t handle, const mico_bt_uuid_t* type, mico_bt_smart_attribute_t** characteristic_value );
OSStatus smartbridge_bt_interface_read_long_characteristic_value( uint16_t connection_handle, uint16_t handle, const mico_bt_uuid_t* type, mico_bt_smart_attribute_t** characteristic_value );
OSStatus smartbridge_bt_interface_read_characteristic_descriptor( uint16_t connection_handle, uint16_t handle, const mico_bt_uuid_t* uuid, mico_bt_smart_attribute_t** descriptor );
OSStatus smartbridge_bt_interface_read_long_characteristic_descriptor( uint16_t connection_handle, uint16_t handle, const mico_bt_uuid_t* uuid, mico_bt_smart_attribute_t** descriptor );
OSStatus smartbridge_bt_interface_read_characteristic_values_using_uuid( uint16_t connection_handle, const mico_bt_uuid_t* uuid, mico_bt_smart_attribute_list_t* characteristic_value_list );

OSStatus smartbridge_bt_interface_write_characteristic_value( uint16_t connection_handle, mico_bt_smart_attribute_t* attribute );
OSStatus smartbridge_bt_interface_write_long_characteristic_value( uint16_t connection_handle, mico_bt_smart_attribute_t* attribute );
OSStatus smartbridge_bt_interface_write_long_characteristic_descriptor( uint16_t connection_handle, const mico_bt_smart_attribute_t* descriptor );
OSStatus smartbridge_bt_interface_write_characteristic_descriptor(  uint16_t connection_handle, mico_bt_smart_attribute_t* attribute );

#ifdef __cplusplus
} /* extern "C" */
#endif
