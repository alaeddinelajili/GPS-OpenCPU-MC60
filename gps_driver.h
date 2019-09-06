#ifndef __GPS_DRIVER_H__
#define __GPS_DRIVER_H__

#include "ql_type.h"



//this struct made by me
//you may add other fields in this struct it depends on what you need
typedef struct {
    u8 longitude[1000];
    u8 latitude[1000]; 
    u8 altitude[1000];
    u8 UTC_time[1000];
    u8 PDOP[1000];
    double PositionDeviation68;
    double PositionDeviation95;
}GPS_Data;

void PowerOnGPS(void);
void PowerOffGPS(void);
void GPS_receive_position(GPS_Data* gps_data);
void GPS_receive_PDOP(GPS_Data* gps_data);
void GPS_error_deviation(GPS_Data* gps_data);
void GPS_show_data(GPS_Data* gps_data);













#endif	//__GPS_DRIVER_H__