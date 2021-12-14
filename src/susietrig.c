#include "susietrig.h"
#include "math.h"



Angle RadiansToDegrees_susie(Angle angle)
{
    angle.value = angle.value * 180 / M_PI;
    angle.angleType = Degrees;
    return angle;
}

Angle DegreesToRadians_susie(Angle angle)
{
    angle.value = angle.value *  M_PI / 180;
    angle.angleType = Radians;
    return angle;
}

Vector2D_susie Translate_susie(Angle angle, float distance)
{
    Vector2D_susie Translated_Vector;
    if (angle.angleType == Degrees)
    {
        angle = DegreesToRadians_susie(angle);
    }
    Translated_Vector.x = distance * cos(angle.value);
    Translated_Vector.y = distance * sin(angle.value);
    return Translated_Vector;
}

Angle EmptyAngle()
{
    Angle angle;
    angle.angleType = Radians;
    angle.value = 0.f;
    return angle;
}

Angle FindAngle(Vector2D_susie vector)
{
    Angle angle;
    angle.value = atan2(vector.y,vector.x);
    angle.angleType = Radians;
    return angle;
}

Angle Rotate(Vector2D_susie vector, enum AngleType type)
{
    Angle angle;
    angle.value = atan2f(vector.y,vector.x);
    angle.angleType = type;
    if(type == Degrees)
    {
        angle = RadiansToDegrees_susie(angle);
    }
    return angle;
}

void DrawAccurateRect(float x, float y, float width, float height, Angle angle, Color color)
{
    if (angle.angleType == Degrees)
    {
        angle = DegreesToRadians_susie(angle);
        
    }
    Vector2D_susie translate = Translate_susie((Angle){angle.value + M_PI/4,Radians},sqrt(width*width + height*height)/2);
    DrawRectanglePro((Rectangle){x-translate.x,y-translate.y,width,height},(Vector2){0,0},RadiansToDegrees_susie(angle).value,color);
}