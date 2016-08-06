/**
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of MXCHIP Corporation.
 *
 */
#include "mico.h"
#include "vendor_mico.h"
#include "json.h"
#include "SocketUtils.h"
#include "device.h"
#include "haProtocol.h"

#define remote_debug(M, ...) custom_log("REMOTE", M, ##__VA_ARGS__)
#define remote_debug_trace() custom_log_trace("REMOTE")

#define MAX_RECONN_NUM      30
#define REMOTE_BUFFER_SIZE  2048
#define REMOTE_HB_INTERVAL  15

static char* ssl_active = NULL;
static char *remote_buffer = NULL;
static mico_semaphore_t hb_sem = NULL;

const char HeartTick[] = "\
{\"code\":101,\"device\":{\"feed_id\":\"%s\",\"access_key\":\"%s\",\"firm_version\":%d}}\
\n";

const char control_ack[] =
  "{\"code\": 102,\
\"attribute\":%s,\
\"result\":0,\
\"control_resp\":{\"current_value\":0,\"stream_id\":\"switch\"},\
\"device\":{\"feed_id\":\"%s\",\"accees_key\":\"%s\"}\
}\n";

const char ota_ack[] =
  "{\"code\": 105,\
\"result\":0,\
\"serial\":%d,\
\"device\":{\"feed_id\":\"%s\",\"accees_key\":\"%s\"}\
}\n";

const char ota_end_ack[] =
  "{\"code\": 107,\
\"device\":{\"feed_id\":\"%s\",\"accees_key\":\"%s\"},\
\"firm_version\":%d,\
\"status\":%d,\
\"session_id\":\"%s\"\
}\n";

void thread_ota(void *arg);

void httpdecode(char *p, int isFind)    // isFind
{
  int i = 0;
  int isLine = 0;
  isLine = !isFind;

  while (*(p + i)) {
    if (isLine == 0) {
      if (*(p + i) == '\n') {
        if (*(p + i + 2) == '\n') {
          i += 1;
          isLine = 1;
        }
      }

      i++;
      continue;
    }

    if ((*p = *(p + i)) == '%') {
      *p = *(p + i + 1) >= 'A' ? ((*(p + i + 1) & 0XDF) - 'A') + 10 : (*(p + i + 1) - '0');
      *p = (*p) * 16;
      *p += *(p + i + 2) >= 'A' ? ((*(p + i + 2) & 0XDF) - 'A') + 10 : (*(p + i + 2) - '0');
      i += 2;
    } else if (*(p + i) == '+') {
      *p = ' ';
    }

    p++;
  }

  *p = '\0';
}

int URLEncode(const char* str, const int strSize, char* result, const int resultSize)
{
  int i;
  int j = 0;//for result index
  char ch;

  if ((str == NULL) || (result == NULL) || (strSize <= 0) || (resultSize <= 0)) {
    return 0;
  }

  for (i = 0; (i < strSize) && (j < resultSize); ++i) {
    ch = str[i];

    if (((ch >= 'A') && (ch < 'Z')) ||
        ((ch >= 'a') && (ch < 'z')) ||
        ((ch >= '0') && (ch < '9'))) {
      result[j++] = ch;
    } else if (ch == ' ') {
      result[j++] = '+';
    } else if (ch == '.' || ch == '-' || ch == '_' || ch == '*') {
      result[j++] = ch;
    } else {
      if (j + 3 < resultSize) {
        sprintf(result + j, "%%%02X", (unsigned char)ch);
        j += 3;
      } else {
        return 0;
      }
    }
  }

  result[j] = '\0';
  return j;
}

int url_signature_encode(char *url, uint32_t urllen, char *dst, uint32_t len)
{
  char *signature_start;
  char *get_page_start;

  char *signature_buffer = (char*)malloc(128);

  if (NULL == signature_buffer) {
    return -1;
  }

  memset(signature_buffer, 0, 128);

  signature_start = strstr(url, "Signature");

  if (NULL == signature_start) {
    return -1;
  }

  URLEncode(signature_start, urllen - (signature_start - url), signature_buffer, 128);

  get_page_start = strstr(url, "/devpro");
  memcpy(dst, get_page_start, urllen - (get_page_start - url));
  signature_start = strstr(dst, "Signature");

  if (NULL == signature_start) {
    return -1;
  }

  memcpy(signature_start, signature_buffer, strlen(signature_buffer));

  free(signature_buffer);

  return 0;
}

/*
jd_time_t cloud_time;

void jdsmart_get_time(jd_time_t *time)
{
  if (NULL != time) {
    memcpy(time, &cloud_time, sizeof(jd_time_t));
  }
}
*/

void *getSSLptr()
{
  return ssl_active;
}

void deleteFeedId(char *buf)
{
  char *feedid, *comma;
  feedid = strstr(buf, "\"feed_id\"");
  require_quiet(feedid != NULL, exit);

  comma = strstr(feedid, ",");
  require_quiet(comma != NULL, exit);

  memset(feedid, 0x20, comma - feedid + 1);

exit:
  return;
}

int JDCmdProcess(char *buffer)
{
  deleteFeedId(buffer);
  mico_Context_t* Context = mico_system_context_get();
  application_config_t *app_config = Context->user_config_data;
  json_object *pRoot, *pJCon, *pJatt;
  pRoot = json_tokener_parse(buffer);

  if (pRoot == NULL) goto ERR2;

  json_object* pJCode = json_object_object_get(pRoot, "code");
  int code_id = json_object_get_int(pJCode);
  remote_debug("code id %d", code_id);
  switch (code_id) {
    case 1001:
      break;
      pJCon = json_object_object_get(pRoot, "time");

      if (pJCon == NULL)
        goto ERR1;

      char* sValue = (char *)json_object_to_json_string(pJCon);

      // some errors here!!!
#if JD_TIME_DEBUG
      sscanf(sValue, "%d-%d-%d %d:%d:%d",
             &cloud_time.year,
             &cloud_time.month,
             &cloud_time.day,
             &cloud_time.hour,
             &cloud_time.min,
             &cloud_time.sec);


      remote_debug("%d.%d.%d %d:%d:%d\r\n",
                   cloud_time.year,
                   cloud_time.month,
                   cloud_time.day,
                   cloud_time.hour,
                   cloud_time.min,
                   cloud_time.sec);
#endif

      break;

    case 1002:
      pJCon = json_object_object_get(pRoot, "control");

      if (pJCon == NULL) goto ERR1;

      if(device_command_execute_jdsmart(pJCon) == -1)
        break;
      pJatt = json_object_object_get(pRoot, "attribute");
      char *out = (char *)json_object_to_json_string(pJatt);
      int length = sprintf(remote_buffer, control_ack, out, app_config->jd_config.feed_id,
                           app_config->jd_config.access_key);
//      remote_debug("|>> %*s", length, remote_buffer);

      if (ssl_active) {
        ssl_send(ssl_active, remote_buffer, length);
      }

      break;

#if USE_JD_OTA

    case 1005:
      int serial;
      char* url;
      char *session_id;
      int version;

      // Step 1: parse ota paramters
      pJCon = cJSON_GetObjectItem(pRoot, "serial");

      if (pJCon == NULL) {
        remote_debug("[ x ] json format error\r\n");
        cJSON_Delete(pRoot);
        return -1;
      } else {
        serial = pJCon->valueint;
      }

      pJCon = cJSON_GetObjectItem(pRoot, "session_id");

      if (pJCon == NULL) {
        remote_debug("[ x ] json format error\r\n");
        cJSON_Delete(pRoot);
        return -1;
      } else {
        session_id = pJCon->valuestring;
      }

      pJCon = cJSON_GetObjectItem(pRoot, "update");

      if (pJCon == NULL) {
        remote_debug("[ x ] json format error\r\n");
        cJSON_Delete(pRoot);
        return -1;
      } else {
        int iCount = cJSON_GetArraySize(pJCon);
        int i = 0;
        cJSON* pItem = cJSON_GetArrayItem(pJCon, 1);
        url = pItem->valuestring;

        pItem = cJSON_GetArrayItem(pJCon, 2);
        version = pItem->valueint;
      }

      // Step 2: OTA Response
      length = sprintf(remote_buffer, ota_ack,
                       serial, info->cloud.feed_id, info->cloud.access_key);

      if (ssl_active) {
        ret = ssl_send(ssl_active, remote_buffer, length);

        if (ret > 0) {
          remote_debug("|>> %s", remote_buffer);
          remote_debug("\r\n");
        }
      }

      // Step 3: Get Binary through HTTP channel
      jd_ota_t otainfo;
      memset(&otainfo, 0, sizeof(otainfo));

      otainfo.buffer = remote_buffer;
      otainfo.url = url;
      mico_rtos_init_semaphore(&otainfo.sem, 1);

      mico_rtos_create_thread(NULL, MICO_APPLICATION_PRIORITY, "thread ota", thread_ota, THREAD_OTA_STACK, &otainfo);
      mico_rtos_get_semaphore(&otainfo.sem, MICO_WAIT_FOREVER);

      mico_rtos_deinit_semaphore(&otainfo.sem);

      // Step 4: OTA Result
      length = sprintf(remote_buffer, ota_end_ack,
                       info.cloud.feed_id,
                       info.cloud.access_key,
                       // use the firmware version which apeared in ota request
                       version,
                       otainfo.result,
                       session_id);

      if (ssl_active) {
        ret = ssl_send(ssl_active, remote_buffer, length);

        if (ret > 0) {
          remote_debug("|>> %s", remote_buffer);
          remote_debug("\r\n");
        } else {
          remote_debug("[ x ] ssl_send(active) ota result failed\r\n");
        }
      }

      // Step 5: Reboot
      board_reboot(REBOOT_OTA);

      break;
#endif
  }
  if(pRoot)
    json_object_put(pRoot);

  return 0;
ERR1:
  if(pRoot)
    json_object_put(pRoot);
ERR2:
  remote_debug("error2");
  //remote_debug("[ x ] Server json error before: [%s]\r\n", cJSON_GetErrorPtr());
  return 0;
}

void report_heartick( void )
{
  mico_Context_t* Context = mico_system_context_get();
  application_config_t *app_config = Context->user_config_data;

  if (NULL == ssl_active) {
    return;
  }

  sprintf(remote_buffer, HeartTick, app_config->jd_config.feed_id,
          app_config->jd_config.access_key, FIRMWARE_REVISION_NUM);

  ssl_send(ssl_active, remote_buffer, strlen(remote_buffer));

  remote_debug("Heartbeat tick");
}

void thread_remote(void *arg)
{
  UNUSED_PARAMETER(arg);
  OSStatus err;
  int errno = 0;
  struct sockaddr_t addr;
  struct timeval_t timeout;
  static uint8_t reConnCount = 0;
  char ipstr[16];
  fd_set readfd;
  int remote_fd = -1 , len;
  mico_Context_t* Context = mico_system_context_get();
  application_config_t *app_config = Context->user_config_data;
  remote_buffer = (char*)malloc(REMOTE_BUFFER_SIZE);
  require_action(remote_buffer, EXIT_THREAD, err = kNoMemoryErr);/*alloc failed*/
  timeout.tv_sec = REMOTE_HB_INTERVAL;
  timeout.tv_usec = 0;

  wait_network_up();
  while(1) {
    if(ssl_active == NULL) {
      err = gethostbyname((char *)app_config->jd_config.domain, (uint8_t *)ipstr, 16);
      require_noerr(err, Reconn);

      remote_debug("server address: %s, ip: %s", app_config->jd_config.domain, ipstr);

      remote_fd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

      addr.s_ip = inet_addr(ipstr);
      addr.s_port = app_config->jd_config.port;
      remote_debug("Connecting to server: ip=%s  port=%d!", ipstr, addr.s_port);

      err = connect(remote_fd, &addr, sizeof(addr));
      require_noerr(err, EXIT_THREAD);

      ssl_active = ssl_connect(remote_fd, 0, 0, &errno);
      remote_debug("ssl connect, send HTTP POST, ssl_active %p, errno %d", ssl_active, errno);
      require_noerr_action( NULL == ssl_active, Reconn, remote_debug("ERROR: ssl disconnect") );
      mico_rtos_set_semaphore(&hb_sem);
      set_network_state(REMOTE_CONNECT, 1);
    } else {

      FD_ZERO(&readfd);
      FD_SET(remote_fd, &readfd);

      select(1, &readfd, NULL, NULL, &timeout);
      /*recv wlan data*/
      if (FD_ISSET( remote_fd, &readfd )) { /*read what,write back*/
        memset(remote_buffer, 0, REMOTE_BUFFER_SIZE);
        len = ssl_recv(ssl_active, remote_buffer, REMOTE_BUFFER_SIZE);
        if( len <= 0) {
          set_network_state(REMOTE_CONNECT, 0);
          remote_debug("socket disconnected");
          goto Reconn;
        }
        remote_debug("recv from server, len=%d", len);
        remote_buffer[len] = '\0';
        httpdecode(remote_buffer, 0);
        JDCmdProcess(remote_buffer);
      } 
    }
    continue;

  Reconn:
    if (NULL != ssl_active) {
      ssl_close(ssl_active);
      ssl_active = NULL;
    }
    if(remote_fd != -1) {
      SocketClose(&remote_fd);
      remote_fd = -1;
    }
    sleep(3);
  }

EXIT_THREAD:
  free(remote_buffer);
  if(remote_fd != -1){
    close(remote_fd);
    remote_fd = -1;
  }
  if(ssl_active != NULL){
    ssl_close(ssl_active);
    ssl_active = NULL;
  }
  mico_rtos_delete_thread(NULL);
}

void thread_heartbeat(void *arg)
{
  UNUSED_PARAMETER(arg);

  mico_rtos_init_semaphore(&hb_sem, 1);

  while(1) {
    mico_rtos_get_semaphore(&hb_sem, REMOTE_HB_INTERVAL * 1000);
    report_heartick();
  }

}

void start_jdsmart(void)
{
  extern void thread_search_init();

  thread_search_init();
  mico_rtos_create_thread(NULL, MICO_APPLICATION_PRIORITY, "thread heartbeat",
                          thread_heartbeat, 1 * 1024, NULL);
  mico_rtos_create_thread(NULL, MICO_APPLICATION_PRIORITY, "thread jdsmart",
                          thread_remote, 10 * 1024, NULL);
}

