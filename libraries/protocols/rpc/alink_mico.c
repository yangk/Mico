/**
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of MXCHIP Corporation.
 *
 */

#ifdef _PLATFORM_MICO_


#include "mico.h"
#include "MICOAppDefine.h"
#include "alink_export.h"
#include "time.h"

#define DEV_NO_UUID 0xFF
#define DEV_UNREG_UUID 0x00

static unsigned int synced_time = 0, synced_utc_time = 0;

USED alink_time_t convert_time(const char *str)
{
  time_t t;
  sscanf(str, "%lu", &t);
  return t;
}

/*------------------------------------------------------------------------------*/

void clearLocalUTC()
{
  synced_utc_time = 0;
}

void setLocalUTC(const char *str)
{

  unsigned int t;

  sscanf(str, "%d", &t);
  synced_time = mico_get_time();
  synced_utc_time = t;

  //    setDeviceTime(t);

}

unsigned int getLocalUTC()
{

  return synced_utc_time + (mico_get_time() - synced_time) / 1000;

}

int needSyncUTC()
{
  int now;
  if (synced_utc_time == 0)
    return 1;

  now = mico_get_time();
  if (now < synced_time)
    return 1;

  // require sync utc time every 30 mins
  if ((now - synced_time) / 1000 > (30 * 60))
    return 1;

  return 0;

}

/*------------------------------------------------------------------------------*/

int set_kv(const char *k, const void *v, size_t len)
{
  mico_Context_t *context = mico_system_context_get();
  application_config_t *app_config = context->user_config_data;

  custom_log("WSF", "uuid %x", app_config->uuid[0]);
  memcpy(app_config->uuid, v, 32);
  app_config->uuid[32] = '\0';
  mico_system_context_update( context );

  return ALINK_OK;
}

int get_kv(const char *k, char *v, size_t len)
{
  mico_Context_t *context = mico_system_context_get();
  application_config_t *app_config = context->user_config_data;

  custom_log("WSF", "uuid %x", app_config->uuid[0]);
  if (app_config->uuid[0] != DEV_NO_UUID) {
    memcpy(v, app_config->uuid, 32);
    v[32] = '\0';
  } else {
    v[0] = '\0';
  }

  return ALINK_OK;
}

int is_main_uuid_inited(void)
{
  mico_Context_t *context = mico_system_context_get();
  application_config_t *app_config = context->user_config_data;
  int is_uuid_inited = 1;

  if ( app_config->uuid[0] == (unsigned char)DEV_NO_UUID
      || app_config->uuid[0] == (unsigned char)DEV_UNREG_UUID)
    is_uuid_inited = 0;

  return is_uuid_inited;
}

void alink_sleep(int millsec)
{
  msleep(millsec);
}

/*----------------------------------------------------------------------------*/

extern bool global_wifi_status;

void wait_network_up()
{
  while(global_wifi_status == false) {
    msleep(100);
  }
}


void wait_network_connected()
{
  wait_network_up();
}

int prepare_network()
{
  return ALINK_OK;
}

void __lseek(void)
{
}

void __close(void)
{
}

int remove(char const*str)
{
    return 0;
}

#endif
