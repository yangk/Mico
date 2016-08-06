/**
 ******************************************************************************
 * @file    MICOAppEntrance.c
 * @author  William Xu
 * @version V1.0.0
 * @date    05-May-2014
 * @brief   Mico application entrance, addd user application functons and threads.
 ******************************************************************************
 *  The MIT License
 *  Copyright (c) 2014 MXCHIP Inc.
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
 ******************************************************************************
 */

#include "mico.h"
#include "StringUtils.h"
#include "main.h"
#include "wifi_station.h"
#include "opt.h"

#define app_log_trace() custom_log_trace("APP")
#define app_log(M, ...) custom_log("APP", M, ##__VA_ARGS__)

app_context_t* app_context_global = NULL;

OSStatus startUserMainThread(app_context_t *app_context)
{
    OSStatus err = kNoErr;
    require_action(app_context, exit, err = kParamErr);

    for(int row=0;row<8;row+=2)
        OLED_ShowString(OLED_DISPLAY_COLUMN_START, row, "                ");
    
    err = mico_rtos_create_thread(NULL, MICO_APPLICATION_PRIORITY, "WIFI Station", wifi_station_thread, STACK_SIZE_WIFI_STATION_THREAD, NULL );
    err = mico_rtos_create_thread(NULL, MICO_APPLICATION_PRIORITY, "T_H thread", display_T_H_thread, STACK_SIZE_HT_THREAD, NULL );
    err = mico_rtos_create_thread(NULL, MICO_APPLICATION_PRIORITY, "light_dis thread", sensor_thread, STACK_SIZE_DEFAULT_THREAD, NULL );
    err = mico_rtos_create_thread(NULL, MICO_APPLICATION_PRIORITY, "Display_oled", display_time_thread, STACK_SIZE_DISPLAY_THREAD, NULL );
    
exit:
    return err;
}

int application_start(void)
{
    app_log_trace();
    OSStatus err = kNoErr;
    app_context_t* app_context;
    mico_Context_t* mico_context;

    /* Create application context */
    app_context = ( app_context_t *)calloc(1, sizeof(app_context_t) );
    require_action( app_context, exit, err = kNoMemoryErr );

    /* Create mico system context and read application's config data from flash */
    mico_context = mico_system_context_init( sizeof( application_config_t) );
    require_action(mico_context, exit, err = kNoResourcesErr);

    app_context->appConfig = mico_system_context_get_user_data( mico_context );
    app_context->mico_context = mico_context;

    /* mico system initialize */
    err = mico_system_init( mico_context );
    require_noerr( err, exit );

    err = startUserMainThread( app_context );
    require_noerr_action( err, exit, app_log("ERROR: start user_main thread failed!") );

exit:
    mico_rtos_delete_thread(NULL);
    return err;
}
