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
    // Logger::Log("u");
    ldlidar->read_lidar_data();
    if (ldlidar->angles[0] < 180 && ldlidar->angles[ldlidar->angles.size() - 1] > 180.0)
        ClosestPoint = PoleVector2d(500, 65535);
    for (int i = 0; i < POINT_PER_PACK; i++)
    {
        double len = ldlidar->distances[i];
        double ang = ldlidar->angles[i];
        // Logger::Log("----------distance:" + String(len));
        if (len > 0)
        {
            //    Logger::Log("d" + String(len));
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
    if (ClosestPoint.Distance > pv2d.Distance && pv2d.Distance > 0.1 &&
        pv2d.Angle >= 200 && pv2d.Angle <= 320)
    {
        ClosestPoint = pv2d;
    }
}