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
#include "MICOAppDefine.h"
#include "vendor_mico.h"
#include "json.h"

#define THREAD_SRCH_HEAP        1024

#define srch_debug(M, ...) custom_log("SEARCH", M, ##__VA_ARGS__)
#define srch_debug_trace() custom_log_trace("SEARCH")

// <jinfeng> added temp
#pragma pack(1)
typedef struct {
  unsigned int magic;
  unsigned int len;
  unsigned int enctype;
  unsigned char checksum;
} common_header_t;

typedef struct {
  unsigned int type;
  unsigned char cmd[2];
} cmd_header;
#pragma pack()
// </jinfeng>

static uint8_t *search_buffer = NULL;

static int PacketBuild(char* pBuf, int enctype, int type, json_object *root)
{
  common_header_t* pCommon = (common_header_t*)pBuf;
  pCommon->magic = 0x55AA;
  pCommon->enctype = 0; // in debug mode, no encryptocol

  pBuf += sizeof(common_header_t);
  cmd_header* pCmd = (cmd_header*)(pBuf);
  pCmd->type = type;
  memcpy(pCmd->cmd, "OK", 2);

  char* pData = pBuf + sizeof(cmd_header);

//  char* psJson = (char *)json_object_to_json_string(root);
//  int length = strlen(psJson);
//	srch_debug("psJson(%d) %s", length, psJson);
//  memcpy(pData, psJson, length);

  sprintf(pData, "%s", json_object_to_json_string(root));

  pCommon->len = strlen(pData) + sizeof(cmd_header);

  int i = 0;
  unsigned char sum = 0;

  for (i = 0; i < pCommon->len; i++) {
    sum += *(pBuf + i);
  }

  pCommon->checksum = sum;
  return pCommon->len + sizeof(common_header_t);
}

static json_object* PacketAnalyse(cmd_header**pCmd, char *pBuf, int length)
{
  common_header_t* pCommon = (common_header_t*)pBuf;

  if ((pCommon->magic & 0xFFFFFF00) == 0x30303000) {
    static cmd_header Cmd;
    Cmd.type = (pCommon->magic & 0xFF) - '0';
    *pCmd = &Cmd;
    json_object* pRet = json_tokener_parse(pBuf + 4);
    return pRet;
  } else if ((pCommon->magic != 0x55AA) || (length != (pCommon->len + sizeof(common_header_t)))) {
    return NULL;
  } else {
    pBuf +=  sizeof(common_header_t);
    int i = 0;
    char sum = 0;

    for (i = 0; i < pCommon->len; i++) {
      sum +=  *(pBuf + i);
    }

    *pCmd = (cmd_header*)pBuf;
    pBuf +=  sizeof(cmd_header);

    if (pCommon->checksum != sum) {
      *pCmd = NULL;
      return NULL;
    }
    srch_debug("recv srch json(%d): %s", strlen(pBuf), pBuf);
    json_object* pRet = json_tokener_parse(pBuf);

    return pRet;
  }
}


static void udp_data_process(unsigned char *buffer, unsigned int len, int srchfd, struct sockaddr_t *dest)
{
  int fd;
  mico_Context_t* context = mico_system_context_get();
  application_config_t *app_config = context->user_config_data;

  cmd_header* pCmd = NULL;
  int txLength;
  json_object* jDevice, *pItem;
  char* pStr;

  fd = srchfd;

  jDevice = PacketAnalyse(&pCmd, (char *)buffer, len);

  if (jDevice == NULL) {
    srch_debug("PacketAnalyse error\n\r");
    return;
  }
  srch_debug("create json root");
  json_object *root = json_object_new_object();

  if (root == NULL) {
    srch_debug("cJSON_CreateObject failed\n\r");
    json_object_put(jDevice);
    return;
  }

  srch_debug("type %d", pCmd->type);

  switch (pCmd->type) {
    case 1:

      srch_debug("[Srch Request] feedid\r\n");

      pItem = json_object_object_get(jDevice, "productuuid");

      if ((strcmp(PRODUCT_UUID, json_object_get_string(pItem))) ||
          (json_object_get_string(pItem) == 0) ||
          (strcmp("0", json_object_get_string(pItem)))) {
        if (app_config->jd_config.feed_id[0] == 0xff) {
          json_object_object_add(root, "feedid", json_object_new_string("0"));

          srch_debug("[Srch Response] [%s, %s, none]\r\n",
                     context->micoStatus.mac, PRODUCT_UUID);
        } else {
          json_object_object_add(root, "feedid", json_object_new_string((char *)app_config->jd_config.feed_id));

          srch_debug("[Srch Response] [%s, %s, %s]\r\n",
                     context->micoStatus.mac, PRODUCT_UUID, app_config->jd_config.feed_id);
        }

        json_object_object_add(root, "mac", json_object_new_string(context->micoStatus.mac));
        json_object_object_add(root, "productuuid", json_object_new_string(PRODUCT_UUID));
        int length = PacketBuild((char *)buffer, 2, 2, root);
        sendto(fd, buffer, length, 0, dest,  sizeof(struct sockaddr_t));
      }

      break;

    case 3:

      pItem = json_object_object_get(jDevice, "feedid");
      strcpy((char *)app_config->jd_config.feed_id, json_object_get_string(pItem));
      pItem = json_object_object_get(jDevice, "accesskey");
      strcpy((char *)app_config->jd_config.access_key, json_object_get_string(pItem));
      pItem = json_object_object_get(jDevice, "server");
      pStr = (char *)json_object_get_string(pItem);
      srch_debug("%s", pStr);

      //sscanf(pStr, "[ \"%s:%d\" ]", context->flashContentInRam.appConfig.jd_config.domain, (int*)&(context->flashContentInRam.appConfig.jd_config.port));

      char tmpstr[64];
      int i;
      memcpy(tmpstr, &pStr[3], strlen(pStr) - 4);

      for (i = 0; i < strlen(pStr); i++) {
        if (':' == tmpstr[i]) {
          break;
        }

        app_config->jd_config.domain[i] = tmpstr[i];
      }

      app_config->jd_config.domain[i] = 0;
      app_config->jd_config.port = atoi(&tmpstr[i + 1]);

      //sscanf(&tmpstr[i + 1], "%d", &context->flashContentInRam.appConfig.jd_config.port);


      srch_debug("[Srch Request] [%s, %s, %s, %d]\r\n",
                 app_config->jd_config.feed_id, app_config->jd_config.access_key,
                 app_config->jd_config.domain, app_config->jd_config.port);

      mico_system_context_update(context);

      json_object_object_add(root, "code", json_object_new_int(0));
      json_object_object_add(root, "msg", json_object_new_string("write feed_id and accesskey successfully!"));
      txLength = PacketBuild((char *)buffer, 2, 4, root);

      srch_debug("[Srch Response] Write OK\r\n");

      sendto(fd, buffer, txLength, 0, dest,  sizeof(struct sockaddr_t));

      break;
    default:
      break;
  }

  json_object_put(root);
  json_object_put(jDevice);
}


void thread_search(void *arg)
{
  int fd = -1;
  int ret;
  struct sockaddr_t addr;
  socklen_t addrLen;
  int len;

  search_buffer = (uint8_t*)malloc(THREAD_SRCH_HEAP);

  if (NULL == search_buffer) {
    srch_debug("can't malloc for search thread, reboot\r\n");
    mico_rtos_delete_thread(NULL);
  }

  while (1) {
    if (fd == -1) {
      fd = socket(AF_INET, SOCK_DGRM, IPPROTO_UDP);
      addr.s_ip = INADDR_ANY;
      addr.s_port = 80;
      ret = bind(fd, &addr, sizeof(addr));
      srch_debug("search fd %d", fd);

      if (-1 == ret) {
        close(fd);
        fd = -1;
      }
    } else {
      memset(search_buffer, 0, THREAD_SRCH_HEAP);
      len = recvfrom(fd, search_buffer, THREAD_SRCH_HEAP, 0, &addr, &addrLen);
      srch_debug("recv from udp len = %d", len);
      if (len <= 0) {
        close(fd);
        fd = -1;
      } else {
        udp_data_process(search_buffer, len, fd, &addr);
      }
    }
  }
}

void thread_search_init()
{
  mico_rtos_create_thread(NULL, MICO_APPLICATION_PRIORITY, "thread search",
                          thread_search, 2048, NULL);
}

