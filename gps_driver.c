


//lib made by me



#include "ril.h"
#include "ril_util.h"
#include "ql_type.h"
#include "ql_trace.h"
#include "ql_system.h"
#include "ql_uart.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ril_gps.h"
#include "ql_gnss.h"
#include "nema_pro.h"
#include "ql_gpio.h"
#include "gps_driver.h"

#define DEBUG_ENABLE 1
#if DEBUG_ENABLE > 0
#define DEBUG_PORT  UART_PORT1
#define DBG_BUF_LEN   512
static char DBG_BUFFER[DBG_BUF_LEN];
#define APP_DEBUG(FORMAT,...) {\
    Ql_memset(DBG_BUFFER, 0, DBG_BUF_LEN);\
    Ql_sprintf(DBG_BUFFER,FORMAT,##__VA_ARGS__); \
    if (UART_PORT2 == (DEBUG_PORT)) \
    {\
        Ql_Debug_Trace(DBG_BUFFER);\
    } else {\
        Ql_UART_Write((Enum_SerialPort)(DEBUG_PORT), (u8*)(DBG_BUFFER), Ql_strlen((const char *)(DBG_BUFFER)));\
    }\
}
#else
#define APP_DEBUG(FORMAT,...) 
#endif

void PowerOnGPS(void){

	 //POWER ON THE GPS
				s32 iRet = 0;
                iRet = RIL_GPS_Open(1);
                if(RIL_AT_SUCCESS != iRet) 
                {
                    APP_DEBUG("Power on GPS fail, iRet = %d.\r\n", iRet);
                }

                APP_DEBUG("Power on GPS Successful.\r\n");

}

void PowerOffGPS(void){

				s32 iRet = 0;
	  			iRet = RIL_GPS_Open(0);
                if(RIL_AT_SUCCESS != iRet) 
                {
                    APP_DEBUG("Power off GPS fail, iRet = %d.\r\n", iRet);
                }

                APP_DEBUG("Power off GPS Successful.\r\n");

}

void GPS_receive_position(GPS_Data* gps_data){

				s32 iRet = 0;
				u8 rdBuff[1000];
                u8 item[10] = {0};
                u8 buf[1000];
                Ql_memset(rdBuff,0,sizeof(rdBuff));
                Ql_memset(buf,0,sizeof(buf));

               

                //GGA
                s32 iRett;
                Ql_strcpy(item,"GGA");
                iRett = RIL_GPS_Read(item,rdBuff);
                if(RIL_AT_SUCCESS != iRett)
                {
                    APP_DEBUG("Read %s information failed.\r\n",item);
                }
                else
                {
                //erase old data
                
                Ql_memset(gps_data->altitude,0,sizeof(gps_data->altitude));
                Ql_memset(gps_data->UTC_time,0,sizeof(gps_data->UTC_time));
                //update gps_data 
                Ql_memmove(gps_data->altitude,rdBuff + Ql_strlen(rdBuff) - 25,4);
                Ql_memmove(gps_data->UTC_time,rdBuff + 19,10);
                
                }


                 // RMC
                Ql_strcpy(item,"RMC");
                Ql_memset(rdBuff,0,sizeof(rdBuff));
                iRet = RIL_GPS_Read(item,rdBuff);
                if(RIL_AT_SUCCESS != iRet)
                {
                    APP_DEBUG("Read %s information failed.\r\n",item);
                }
                else
                {   
                    Ql_memset(rdBuff+56,0,sizeof(u8));
                    Ql_memmove(buf,rdBuff+32,24);
                    //erase old position on gps_data
                    Ql_memset(gps_data->longitude,0,sizeof(gps_data->longitude));
                    Ql_memset(gps_data->latitude,0,sizeof(gps_data->latitude));
                    //update gps_data 
                    Ql_memmove(gps_data->latitude,buf,11);
                    Ql_memmove(gps_data->longitude,buf+12,12);
                
                }




}

void GPS_receive_PDOP(GPS_Data* gps_data){

				//GSA
				s32 iRet = 0;
				u8 rdBuff[1000];
                u8 item[10] = {0};
              
                Ql_strcpy(item,"GSA");
                Ql_memset(rdBuff,0,sizeof(rdBuff));
                iRet = RIL_GPS_Read(item,rdBuff);
                if(RIL_AT_SUCCESS != iRet)
                {
                    APP_DEBUG("Read %s information failed.\r\n",item);
                }
                else
                {
                //erase old data
                Ql_memset(gps_data->PDOP,0,sizeof(gps_data->PDOP));
                //update
                Ql_memmove(gps_data->PDOP,rdBuff + Ql_strlen(rdBuff)-9,4);
               
                }

}

void GPS_error_deviation(GPS_Data* gps_data){


				int UERE; //User Equivalent Range Error , value can be between 4 and 15 , under a clear view of the sky it may be 5
				UERE = 6;
				gps_data->PositionDeviation68= Ql_atof(gps_data->PDOP) * UERE;
                gps_data->PositionDeviation95= gps_data->PositionDeviation68*2;
                 
}

void GPS_show_data(GPS_Data* gps_data){

				APP_DEBUG("LONGITUDE : %s\r\n",gps_data->longitude);
                APP_DEBUG("LATITUDE : %s\r\n",gps_data->latitude);	
                APP_DEBUG("PDOP : %s\r\n",gps_data->PDOP);
				APP_DEBUG("Position Accuracy of 68.27%% level of reliability : %f m\r\n",gps_data->PositionDeviation68);
                APP_DEBUG("Position Accuracy 95%% level of reliability : %f m\r\n",gps_data->PositionDeviation95);   
                APP_DEBUG("TIME : %s\r\n",gps_data->UTC_time);
                APP_DEBUG("ALTITUDE : %s\r\n",gps_data->altitude);
}







