/**
 ******************************************************************************
 * @file    ble_advertisements.h
 * @author  Jian Zhang
 * @version V1.0.0
 * @date    25-May-2016
 * @brief   This file provides GATT definitions for BLE advertisement demo device
 ******************************************************************************
 *
 *  The MIT License
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is furnished
 *  to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ******************************************************************************
 */

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
 *                                Constants
 ******************************************************************************/

/******************************************************************************
 *                                Constants
 ******************************************************************************/

/******************************************************************************
 *                             Type Definitions
 ******************************************************************************/

typedef enum
{
    HDLS_GENERIC_ATTRIBUTE = 0x1,
    HDLC_GENERIC_ATTRIBUTE_SERVICE_CHANGED,
    HDLC_GENERIC_ATTRIBUTE_SERVICE_CHANGED_VALUE,

    HDLS_GENERIC_ACCESS = 0x14,
    HDLC_GENERIC_ACCESS_DEVICE_NAME,
    HDLC_GENERIC_ACCESS_DEVICE_NAME_VALUE,

    HDLC_GENERIC_ACCESS_APPEARANCE,
    HDLC_GENERIC_ACCESS_APPEARANCE_VALUE,

    HDLS_HELLO_SENSOR = 0x30,
    HDLC_HELLO_SENSOR_NOTIFY,
    HDLC_HELLO_SENSOR_NOTIFY_VALUE,
    HDLC_HELLO_SENSOR_NOTIFY_CFG_DESC,

    HDLC_HELLO_SENSOR_COLOR,
    HDLC_HELLO_SENSOR_COLOR_VALUE,

    HDLS_SPP = 0x40,
    HDLC_SPP_IN,
    HDLC_SPP_IN_VALUE,
    HDLC_SPP_IN_DESCRIPTION,

    HDLC_SPP_OUT,
    HDLC_SPP_OUT_VALUE,
    HDLC_SPP_OUT_DESCRIPTION,

    HDLS_DEV_INFO = 0x50,
    HDLC_DEV_INFO_MFR_NAME,
    HDLC_DEV_INFO_MFR_NAME_VALUE,

    HDLC_DEV_INFO_MODEL_NUM,
    HDLC_DEV_INFO_MODEL_NUM_VALUE,

    HDLC_DEV_INFO_SYSTEM_ID,
    HDLC_DEV_INFO_SYSTEM_ID_VALUE,

    HDLS_BAT = 0x60,
    HDLC_BAT_LEVEL,
    HDLC_BAT_LEVEL_VALUE,

} hello_sensor_db_tags;



