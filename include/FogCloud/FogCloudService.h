/**
 ******************************************************************************
 * @file    FogCloudService.h
 * @author  Eshen Wang
 * @version V0.2.0
 * @date    23-Nov-2014
 * @brief   This header contains interfaces of FogCloud service operation
 ******************************************************************************
 *
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

#ifndef __FOGCLOUD_SERVICE_H_
#define __FOGCLOUD_SERVICE_H_

#include "FogCloudServiceDef.h"
#include "FogCloudUtils.h"

/*******************************************************************************
 * INTERFACES
 ******************************************************************************/

OSStatus FogCloudServiceInit(easycloud_service_context_t* const context);
OSStatus FogCloudServiceStart(easycloud_service_context_t* const context);  //start thread

easycloud_service_state_t FogCloudServiceState(easycloud_service_context_t* const context);

OSStatus FogCloudActivate(easycloud_service_context_t* const context);
OSStatus FogCloudAuthorize(easycloud_service_context_t* const context);

OSStatus FogCloudPublish(easycloud_service_context_t* const context, 
                          const unsigned char *msg, unsigned int msgLen);
// publish to any topic
OSStatus FogCloudPublishto(easycloud_service_context_t* const context, 
                            const char* topic, 
                            const unsigned char *msg, unsigned int msgLen);
// publish to sub-level "device_id/in/<level>"
OSStatus FogCloudPublishtoChannel(easycloud_service_context_t* const context, 
                            const char* channel, 
                            const unsigned char *msg, unsigned int msgLen);

OSStatus FogCloudGetLatestRomVersion(easycloud_service_context_t* const context); //get rom version
OSStatus FogCloudGetRomData(easycloud_service_context_t* const context,
                             ecs_ota_flash_params_t ota_flash_params); //get rom data && write to flash

OSStatus FogCloudDeviceReset(easycloud_service_context_t* const context);

OSStatus FogCloudServiceStop(easycloud_service_context_t* const context);
OSStatus FogCloudServiceDeInit(easycloud_service_context_t* const context);

int FogCloudServiceVersion(easycloud_service_context_t* const context);

#endif
