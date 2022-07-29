#include <Arduino.h>
#include "LidarLd06.h"
#include "Vectors.hpp"
#include "Logger.h"

LidarLd06::~LidarLd06()
{
    free(ldlidar);
}

void LidarLd06::begin()
{
    ldlidar = new LD06forArduino();
}

void LidarLd06::update()
{
    ldlidar->read_lidar_data();
    ClosestPoint = PoleVector2d(0, 65535);
    for (int i = 0; i < POINT_PER_PACK; i++)
    {
        double len = ldlidar->distances[i];
        double ang = ldlidar->angles[i];
        // Logger::Log("----------distance:" + String(len));
        if (len > 0)
        {
            // Logger::Log("distance:" + String(len));
            PoleVector2d point;
            PoleVector2d pv2d = PoleVector2d(ang, len);
            AddPoint(pv2d);
        }
    }
}

void LidarLd06::AddPoint(PoleVector2d pv2d)
{
    pcloudpointer = (1 + pcloudpointer) % 360;
    pointCloud[pcloudpointer] = pv2d;
    if (ClosestPoint.Distance > pv2d.Distance || ClosestPoint.Distance < 0.1)
    {
        ClosestPoint = pv2d;
    }
}