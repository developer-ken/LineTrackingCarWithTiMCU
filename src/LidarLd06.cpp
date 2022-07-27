#include <Arduino.h>
#include "LidarLd06.h"
#include "Vectors.hpp"

LidarLd06::~LidarLd06()
{
    uart.end();
}

void LidarLd06::begin(HardwareSerial serialport, int baudrate)
{
    uart = serialport;
    uart.begin(baudrate);
}

void LidarLd06::update()
{
    while (uart.available() > 0)
    {
        uint8_t byte1 = uart.read();
        if (uart.available() > 0)
        {
            uint8_t byte2 = uart.peek();
            if (byte1 == 0x54 && byte2 == 0x2C)
            {
                readPointer = 0;
            }
        }
        buffer[readPointer++] = byte1;
        if (readPointer >= 47)
        {
            // UpdatePointcloud
            LiDARFrameTypeDef lidarFrame = *(LiDARFrameTypeDef *)buffer;
            double anglebit = ((lidarFrame.end_angle - lidarFrame.start_angle) / POINT_PER_PACK) * 1.0 / 100.0;
            for (int i = 0; i < POINT_PER_PACK; i++)
            {
                if (lidarFrame.point[i].distance != 0)
                {
                    PoleVector2d pv2d = PoleVector2d(lidarFrame.start_angle + (anglebit * i), lidarFrame.point[i].distance);
                    AddPoint(pv2d);
                }
            }
            readPointer = 0;
        }
    }
}

void LidarLd06::AddPoint(PoleVector2d pv2d)
{
    pcloudpointer = (1 + pcloudpointer) % 360;
    pointCloud[pcloudpointer] = pv2d;
    if (ClosestPoint.Distance > pv2d.Distance)
    {
        ClosestPoint = pv2d;
    }
}