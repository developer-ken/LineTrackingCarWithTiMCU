#include "Vectors.hpp"
#include <math.h>

PoleVector2d::PoleVector2d(double angle, double distance)
{
    Angle = angle;
    Distance = distance;
}

Vector2d PoleVector2d::ToLinearVect()
{
    return Vector2d(Distance * cos(Angle), Distance * sin(Angle));
}

Vector2d PoleVector2d::VectorDistanceTo(Vector2d p)
{
    Vector2d v2d = ToLinearVect();
    return Vector2d(p.X - v2d.X, p.Y - v2d.Y);
}

Vector2d PoleVector2d::VectorDistanceTo(PoleVector2d p)
{
    return VectorDistanceTo(p.ToLinearVect());
}

double PoleVector2d::AbsDistanceTo(Vector2d p)
{
    return VectorDistanceTo(p).Length();
}

double PoleVector2d::AbsDistanceTo(PoleVector2d p)
{
    return VectorDistanceTo(p).Length();
}

double Vector2d::Length()
{
    return sqrt(X * X + Y * Y);
}

Vector2d Vector2d::VectorDistanceTo(PoleVector2d p){
    return VectorDistanceTo(p.ToLinearVect());
}

Vector2d Vector2d::VectorDistanceTo(Vector2d p){
    return Vector2d(p.X - X, p.Y - Y);
}

double Vector2d::AbsDistanceTo(PoleVector2d p){
    return AbsDistanceTo(p.ToLinearVect());
}

double Vector2d::AbsDistanceTo(Vector2d p){
    return VectorDistanceTo(p).Length();
}