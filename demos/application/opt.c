#include <time.h>
#include "mico.h"
#include "string.h"
#include "opt.h"
#include "sntp.h"
#include "micokit_ext.h"
#include "sensor/DHT11/DHT11.h"

#define ext_opt_log(M, ...) custom_log("EXT", M, ##__VA_ARGS__)

void display_time_thread(void *args)
{
    struct tm currentTime;
    char buf[17] = {'\0'};

    while(1)
    {
        sntp_current_time_get(&currentTime);
        memset(buf, '\0', 17);
        snprintf(buf, 16, "%02d:%02d:%02d   W %d\r", currentTime.tm_hour, currentTime.tm_min,currentTime.tm_sec,currentTime.tm_wday);
        OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_1, buf);
        if((currentTime.tm_min == 0) && (currentTime.tm_sec == 0))
            sntp_client_start();
        mico_thread_sleep(1);
    }  
}
void display_T_H_thread(void *args)
{
    OSStatus err = kNoErr;
    char buf[16] = {0};
    uint8_t dht11_temp_data = 0;
    uint8_t dht11_hum_data = 0;
    
    err = DHT11_Init();
    require_noerr_action( err, exit, ext_opt_log("ERROR: Unable to Init DHT11") );
    msleep(1000);
    while(1)
    {
try:
        err = DHT11_Read_Data(&dht11_temp_data, &dht11_hum_data);
        require_noerr_action( err, try, ext_opt_log("ERROR: Can't Read Data") );

        memset(buf, '\0', 17);
        snprintf(buf, 16, "T:%3.1fC H:%3.1f%%\r", (float)dht11_temp_data, (float)dht11_hum_data);          
        OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_2, buf);
        mico_thread_sleep(10);
    }          
exit:
    ext_opt_log("ERROR: reach the display_T_H_thread exit!");
    mico_rtos_delete_thread(NULL); 
}

void sensor_thread(void *args)
{
    OSStatus err = kNoErr;
    char buf[17] = {'\0'};
    uint16_t apds9930_Prox = 0;
    uint16_t apds9930_Lux = 0;
    float Prox = 0.0;

    err = apds9930_sensor_init(); 
    require_noerr_action( err, exit, ext_opt_log("ERROR: Unable to Init APDS9930") );
    err = dc_motor_init();
    require_noerr_string( err, exit, "ERROR: Unable to Init DC motor" );
    while(1)  
    {     
        mico_thread_sleep(1);      
        err = apds9930_data_readout(&apds9930_Prox, &apds9930_Lux);     
        require_noerr_action( err, exit, ext_opt_log("ERROR: Can't Read Data") ); 
        Prox = (10239-apds9930_Prox)/100;
        memset(buf, '\0', 17);
        snprintf(buf, 16, "D:%03dmm L:%d\r", (uint32_t)Prox, apds9930_Lux);          
        OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_3, buf);
        //ext_opt_log("APDS9930  Prox: %.1fmm  Lux: %d", Prox, apds9930_Lux);
        if(Prox < 80.0)
        {
            dc_motor_set(1);
            rgb_led_init();
            /*open red led,#FF0000*/
            rgb_led_open(255, 0, 0);
            mico_thread_sleep(1);
            rgb_led_open(0, 255, 0);
            mico_thread_sleep(1); 
            rgb_led_open(0, 0, 255);
            mico_thread_sleep(1);
            rgb_led_open(0, 0, 0);
            dc_motor_set(0);
        }
    }
exit:  
    ext_opt_log("ERROR: reach the sensor_thread exit!");
    mico_rtos_delete_thread(NULL); 
}