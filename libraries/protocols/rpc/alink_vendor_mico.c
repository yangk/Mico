/**
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of MXCHIP Corporation.
 *
 */

#include "platform.h"
#include "mico.h"
#include "MICOAppDefine.h"
#include "alink_export.h"
#include "alink_export_rawdata.h"
#include "vendor_mico.h"
#include "json.h"
#include "device.h"
#include "haProtocol.h"

struct alink_config alink_config;
#ifdef SANDBOX
static const char* g_cert_str_sandbox =
  "-----BEGIN CERTIFICATE-----\r\n"
  "MIIEoTCCA4mgAwIBAgIJAMQSGd3GuNGVMA0GCSqGSIb3DQEBBQUAMIGRMQswCQYD\r\n"
  "VQQGEwJDTjERMA8GA1UECBMIWmhlamlhbmcxETAPBgNVBAcTCEhhbmd6aG91MRYw\r\n"
  "FAYDVQQKEw1BbGliYWJhIEdyb3VwMQ4wDAYDVQQLEwVBbGluazEOMAwGA1UEAxMF\r\n"
  "QWxpbmsxJDAiBgkqhkiG9w0BCQEWFWFsaW5rQGFsaWJhYmEtaW5jLmNvbTAeFw0x\r\n"
  "NDA4MjkwMzA5NDhaFw0yNDA4MjYwMzA5NDhaMIGRMQswCQYDVQQGEwJDTjERMA8G\r\n"
  "A1UECBMIWmhlamlhbmcxETAPBgNVBAcTCEhhbmd6aG91MRYwFAYDVQQKEw1BbGli\r\n"
  "YWJhIEdyb3VwMQ4wDAYDVQQLEwVBbGluazEOMAwGA1UEAxMFQWxpbmsxJDAiBgkq\r\n"
  "hkiG9w0BCQEWFWFsaW5rQGFsaWJhYmEtaW5jLmNvbTCCASIwDQYJKoZIhvcNAQEB\r\n"
  "BQADggEPADCCAQoCggEBAMHr21qKVy3g1GKWdeGQj3by+lN7dMjGoPquLxiJHzEs\r\n"
  "6auxiAiWez8pFktlekIL7FwK5F7nH1px5W5G8s3cTSqRvunex/Cojw8LbNAStpXy\r\n"
  "HrqUuDhL3DYF7x87M/7H3lqFuIlucSKNC60Yc03yuIR1I/W0di40rDNeXYuCzXIv\r\n"
  "yheg+b7zD939HOe+RS878hDW5/p75FY+ChI8GA4dPEQb5fyRrjHAXneo+S8jdnqr\r\n"
  "SCjHSS7+jI36dyEfZ72rkLNJ3v1WboH02Rchj1fFIfagn+Ij4v0ruejOTIvc/ngD\r\n"
  "OLZNTUyF4B3EG4IAZRlO12jDECc4Com0yfFQ0IxkNVMCAwEAAaOB+TCB9jAdBgNV\r\n"
  "HQ4EFgQU9iyOWx+oGSOhdlpHeWMYsHXRwwkwgcYGA1UdIwSBvjCBu4AU9iyOWx+o\r\n"
  "GSOhdlpHeWMYsHXRwwmhgZekgZQwgZExCzAJBgNVBAYTAkNOMREwDwYDVQQIEwha\r\n"
  "aGVqaWFuZzERMA8GA1UEBxMISGFuZ3pob3UxFjAUBgNVBAoTDUFsaWJhYmEgR3Jv\r\n"
  "dXAxDjAMBgNVBAsTBUFsaW5rMQ4wDAYDVQQDEwVBbGluazEkMCIGCSqGSIb3DQEJ\r\n"
  "ARYVYWxpbmtAYWxpYmFiYS1pbmMuY29tggkAxBIZ3ca40ZUwDAYDVR0TBAUwAwEB\r\n"
  "/zANBgkqhkiG9w0BAQUFAAOCAQEAO7u7ozEES2TgTepq3ZTk1VD5qh2zhcSLLr+b\r\n"
  "yDQlkbm0lnah/GOGmpr/Wlr8JSXUJEWhsLcbnG1dhbP72DzFHri8ME4wO8hbeyXU\r\n"
  "7cFOHmP4DZi+Ia2gyy/GZ66P6L9df89MJzMOt46NYn+A3rI12M0qTJ6GNdUHz2R9\r\n"
  "VGkahs6bfGQGi0Se24jj4Es+MeAlrG5U0d0wVY5Dt4jpS9wHLupyAiANbj4Ls5x2\r\n"
  "6cwS4+Q4ErezFMnKiQ5VKc6S88ohYszt82CYMPEqIiZRkCfjsVz/r6vw2DFwN0Ox\r\n"
  "8Cb9myZwXypcOZbI7M+9W8909Z+TSHW1UlNkyrIsqDGuzE866w==\r\n"
  "-----END CERTIFICATE-----\r\n";

const char* sandbox_server = "110.75.122.2";
int sandbox_port = 80;


void sandbox_setting()
{
  alink_config.cert = g_cert_str_sandbox;
  alink_config.server = sandbox_server;
  alink_config.port = sandbox_port;
  alink_config.factory_reset = 0;
}

#endif

const char *mico_get_mac_addr()
{
  mico_Context_t *context = mico_system_context_get();
  return context->micoStatus.mac;
}

int get_available_memory(void* p1, void* p2)
{
  UNUSED_PARAMETER(p1);
  UNUSED_PARAMETER(p2);
  return MicoGetMemoryInfo()->free_memory;
}

#ifdef ALINK_RAWDATA
extern int alink_uart_get(char **data, int *len);
extern mico_timer_t _uart_reload_timer;
int alink_uart_put(char *buf, int len)
{
  if(_uart_reload_timer.handle != NULL)
    mico_reload_timer(&_uart_reload_timer);
  MicoUartSend(UART_FOR_APP, buf, len);
  return 0;
}
#endif

int alink_network_callback(void *mac, void *state)
{
  UNUSED_PARAMETER(mac);
  if (*(int *)state == ALINK_STATUS_LOGGED) {
		extern uint8_t alink_get_count;
		alink_get_count = 0;
    set_network_state(REMOTE_CONNECT, 1);
  } else {
    set_network_state(REMOTE_CONNECT, 0);
  }

  return 0;
}

int mico_start_alink(void)
{
  const uint8_t *cid;
  uint8_t cid_len = 0;
  mico_Context_t *context = mico_system_context_get();
  application_config_t *app_config = context->user_config_data;

  int ret = -1;
  cid = mico_generate_cid( &cid_len );
  memset(&alink_config, 0, sizeof(struct alink_config));
  struct device_info* dev = (struct device_info*) malloc(sizeof(struct device_info));

  memset(dev, 0, sizeof(struct device_info));
  strcpy(dev->key, ALINK_KEY);
  strcpy(dev->secret, ALINK_SECRET);

  sprintf(dev->name, "%s(%c%c%c%c)",DEV_NAME,
          context->micoStatus.mac[12], context->micoStatus.mac[13],
          context->micoStatus.mac[15], context->micoStatus.mac[16]);

  strcpy(dev->model, DEV_MODEL);
  strcpy(dev->manufacturer, DEV_MANUFACTURE);

  strcpy(dev->type, DEV_TYPE);
  strcpy(dev->category, DEV_CATEGORY);
  strcpy(dev->sn, DEV_SN);
  snprintf(dev->version, STR_NAME_LEN, "%d", FIRMWARE_REVISION_NUM);
  memcpy(dev->mac, mico_get_mac_addr(), 18);
  memcpy(dev->cid, cid, Min(cid_len, STR_CID_LEN) );

  custom_log("WSF", "dev->mac %s", dev->mac);
  custom_log("WSF", "dev->cid %s", dev->cid);
#ifdef ALINK_RAWDATA
	custom_log("WSF", "ALINK RAWDATA !!!");
#else
  dev->dev_callback[ACB_GET_DEVICE_STATUS] = device_status_get;
  dev->dev_callback[ACB_SET_DEVICE_STATUS] = device_command_execute;
#endif
  dev->sys_callback[ALINK_FUNC_SERVER_STATUS] = alink_network_callback;
  alink_set_callback(ALINK_FUNC_AVAILABLE_MEMORY, get_available_memory);

#ifdef DEBUG
  alink_set_loglevel(ALINK_LL_NONE);
#else
  alink_set_loglevel(ALINK_LL_NONE);
#endif

  memset(&alink_config, 0, sizeof(struct alink_config));
#ifdef SANDBOX
  sandbox_setting();
#endif
  if( app_config->resetflag == 1) {
    alink_config.factory_reset = 1;
  }

  alink_set_config(&alink_config);

#ifdef ALINK_RAWDATA
  alink_start_rawdata(dev, alink_uart_get, alink_uart_put);
#else
	alink_start(dev);  
#endif

  alink_wait_connect(NULL,-1); 

  free((void*)dev);

  return ret;
}


