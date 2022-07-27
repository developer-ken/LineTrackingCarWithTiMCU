#ifndef _LIDAR_LD06_H_
#define _LIDAR_LD06_H_
#include <Arduino.h>
#include "Vectors.hpp"
#define SERIAL_RX_BUFFER_SIZE 2048

#define POINT_PER_PACK 12
#define HEADER 0x54
typedef struct __attribute__((packed)) {
uint16_t distance;
uint8_t intensity;
} LidarPointStructDef;

typedef struct __attribute__((packed)) {
uint8_t header;
uint8_t ver_len;
uint16_t speed;
uint16_t start_angle;
LidarPointStructDef point[POINT_PER_PACK];
uint16_t end_angle;
uint16_t timestamp;
uint8_t crc8;
}LiDARFrameTypeDef;

class LidarLd06
{
public:
    PoleVector2d pointCloud[360];
    PoleVector2d ClosestPoint;
    ~LidarLd06();
    void begin(HardwareSerial ,int baudrate);
    void update();

private:
    HardwareSerial uart;
    uint8_t buffer[47];
    uint8_t readPointer;
    uint16_t pcloudpointer;
    void AddPoint(PoleVector2d);
};

#endif