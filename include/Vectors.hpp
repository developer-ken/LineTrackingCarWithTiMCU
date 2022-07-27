#ifndef _VECTORS_H_
#define _VECTORS_H_

//采用同ROS标准的坐标系，X向前，Y向左，逆时针转动为正方向

class Vector2d;
class PoleVector2d
{
public:
    PoleVector2d();
    PoleVector2d(double angle, double distance);
    Vector2d VectorDistanceTo(PoleVector2d p);
    Vector2d VectorDistanceTo(Vector2d p);
    double AbsDistanceTo(PoleVector2d p);
    double AbsDistanceTo(Vector2d p);
    Vector2d ToLinearVect();
    double Angle, Distance;
};

class Vector2d
{
public:
    Vector2d(double x, double y);
    PoleVector2d ToPoleVect();
    Vector2d VectorDistanceTo(PoleVector2d p);
    Vector2d VectorDistanceTo(Vector2d p);
    double AbsDistanceTo(PoleVector2d p);
    double AbsDistanceTo(Vector2d p);
    double Length();
    double X, Y;
};

#endif