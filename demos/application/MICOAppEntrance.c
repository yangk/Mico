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
#include "MICOAppDefine.h"
#include "wifi_station.h"

#define app_log_trace() custom_log_trace("APP")
#define app_log(M, ...) custom_log("APP", M, ##__VA_ARGS__)

OSStatus startUserMainThread(app_context_t *app_context)
{
    OSStatus err = kNoErr;
    //require_action(app_context, exit, err = kParamErr);

    err = mico_rtos_create_thread(NULL, MICO_APPLICATION_PRIORITY, "WIFI Station", wifi_station_thread, STACK_SIZE_WIFI_STATION_THREAD, NULL );
    //require_noerr_action( err, exit, app_log("ERROR: Unable to start the WIFI Station thread.") );
exit:
    return err;
}

int application_start(void)
{
  app_log_trace();
  OSStatus err = kNoErr;

  /* mico system initialize */
  MicoInit( );
  err = startUserMainThread(NULL);
  require_noerr_action( err, exit, app_log("ERROR: start user_main thread failed!") );
  

exit:
  mico_rtos_delete_thread(NULL);
  return err;
}
