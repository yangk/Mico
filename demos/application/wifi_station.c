#include "mico.h"
#include "wifi_station.h"
#include "opt.h"
#include "sntp.h"

#define wifi_station_log(M, ...) custom_log("WIFI", M, ##__VA_ARGS__)
#define wifi_station_trace() custom_log_trace("WIFI")

const char *ap_ssid = "Eastsoft_30019";
const char *ap_key = "1234567890";

void wifi_station_thread(void *inContext)
{
    network_InitTypeDef_adv_st  wNetConfigAdv={0};
    IPStatusTypedef ip_status={0};
    
    wifi_station_trace();
    
	strcpy((char*)wNetConfigAdv.ap_info.ssid, ap_ssid);   /* wlan ssid string */
	strcpy((char*)wNetConfigAdv.key, ap_key);                /* wlan key string or hex data in WEP mode */
	wNetConfigAdv.key_len = strlen(ap_key);                  /* wlan key length */
	wNetConfigAdv.ap_info.security = SECURITY_TYPE_AUTO;          /* wlan security mode */
	wNetConfigAdv.ap_info.channel = 0;                            /* Select channel automatically */
	wNetConfigAdv.dhcpMode = DHCP_Client;                         /* Fetch Ip address from DHCP server */
	wNetConfigAdv.wifi_retry_interval = 100;                      /* Retry interval after a failure connection */

    do
    {
        LinkStatusTypeDef link_status={0};
        micoWlanGetLinkStatus(&link_status);
        msleep(100);
        if (link_status.is_connected == 1)
        {
            micoWlanGetIPStatus(&ip_status, Station);
            wifi_station_log("Already connected to %s successful", ap_ssid);
            wifi_station_log("IP Address : %s", ip_status.ip );
            wifi_station_log("Mask : %s", ip_status.mask );
            wifi_station_log("Gateway : %s", ip_status.gate );
            wifi_station_log("Broadcast : %s", ip_status.broadcastip );
            wifi_station_log("DNS Server : %s", ip_status.dns );
            wifi_station_log("Mac Address: %s", ip_status.mac);

            switch (ip_status.dhcp)
            {
                case DHCP_Disable:
                    wifi_station_log("DHCP Off");
                break;

                case DHCP_Client:
                    wifi_station_log("DHCP Client");
                break;

                case DHCP_Server:
                    wifi_station_log("DHCP Server");
                break;

                default:
                    
                break;
            }
            sntp_client_start();
            
            break;
        }

        else
        {
            /* Connect Now! */
            wifi_station_log("connecting to %s...,key:%s", wNetConfigAdv.ap_info.ssid,wNetConfigAdv.key);
            micoWlanStartAdv(&wNetConfigAdv);
            mico_thread_sleep(5);
        }
    }while(1);
    while(1)
    {
        LinkStatusTypeDef link_status={0};
        micoWlanGetLinkStatus(&link_status);
        msleep(100);
        if (link_status.is_connected != 1)
        {
            wifi_station_log("connecting to %s...,key:%s", wNetConfigAdv.ap_info.ssid,wNetConfigAdv.key);
            micoWlanStartAdv(&wNetConfigAdv);
            mico_thread_sleep(5);
        }
        else
            mico_thread_sleep(60);
    }
}
