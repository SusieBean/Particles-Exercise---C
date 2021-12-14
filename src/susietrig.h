#ifndef SUSIE_______TRIG
#define SUSIE_______TRIG

#include <math.h>
#include "susievector.h"
#define M_PI 3.14159265358979323846


enum AngleType
{
    Radians,
    Degrees
};


typedef struct Angle
{
    float value;
    enum AngleType angleType;
}Angle;


typedef struct Angle2D
{
    float value_x;
    float value_y;
    enum AngleType AngleType;
    
}Angle2D;

Angle RadiansToDegrees_susie(Angle angle);
Angle DegreesToRadians_susie(Angle angle);
Vector2D_susie Translate_susie(Angle angle, float distance);
Angle EmptyAngle();
Angle FindAngle(Vector2D_susie vector);
Angle Rotate(Vector2D_susie vector, enum AngleType type);
void DrawAccurateRect(float x, float y, float width, float height, Angle angle, Color color);

#endif