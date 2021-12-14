#include "susievector.h"
#include "math.h"
#include "raylib.h"


//Vector2 to susie Vector2D
Vector2D_susie Vector2D_raylibToSusie(Vector2 vector)
{
    return (Vector2D_susie){vector.x,vector.y};
}

//Vector2D_susie to Vector2
Vector2 Vector2D_susieToRaylib(Vector2D_susie vector)
{
    return (Vector2){vector.x,vector.y};
}

//Vector3 to susie Vector3D
Vector3D_susie Vector3D_raylibToSusie(Vector3 vector)
{
    return (Vector3D_susie){vector.x,vector.y,vector.z};
}

//Vector3D to raylib Vector3
Vector3 Vector3D_susieToRaylib(Vector3D_susie vector)
{
    return (Vector3){vector.x,vector.y,vector.z};
}

//ADDITION
Vector2D_susie Vector2DAdd_susie(Vector2D_susie vector1, Vector2D_susie vector2)
{
    return (Vector2D_susie)
    {
        vector1.x + vector2.x,
        vector1.y + vector2.y
    };
}

Vector3D_susie Vector3DAdd_susie(Vector3D_susie vector1, Vector3D_susie vector2)
{
    return (Vector3D_susie)
    {
        vector1.x + vector2.x,
        vector1.y + vector2.y,
        vector1.z + vector2.z
    };
}

//SUBTRACTION

Vector2D_susie Vector2DSubtract_susie(Vector2D_susie vector1, Vector2D_susie vector2)
{
    return (Vector2D_susie)
    {
        vector1.x - vector2.x,
        vector1.y - vector2.y
    };
}

Vector3D_susie Vector3DSubtract_susie(Vector3D_susie vector1, Vector3D_susie vector2)
{
    return (Vector3D_susie)
    {
        vector1.x - vector2.x,
        vector1.y - vector2.y,
        vector1.z - vector2.z
    };    
}

//SCALAR MULTIPLICATION

Vector2D_susie Vector2DScalarMultiplication_susie(Vector2D_susie vector, float scalar)
{
    return (Vector2D_susie)
    {
        vector.x * scalar,
        vector.y * scalar
    };
}

Vector3D_susie Vector3DScalarMultiplication_susie(Vector3D_susie vector, float scalar)
{
    return (Vector3D_susie)
    {
        vector.x * scalar,
        vector.y * scalar,
        vector.z * scalar
    };    
}

//SCALAR DIVISION

Vector2D_susie Vector2DScalarDivision_susie(Vector2D_susie vector, float scalar)
{
    if (scalar != 0)
    {
        return (Vector2D_susie)
        {
            vector.x / scalar,
            vector.y / scalar
        };        
    }
    return (Vector2D_susie){0.f,0.f};  
}


Vector3D_susie Vector3DScalarDivision_susie(Vector3D_susie vector, float scalar){
    if (scalar != 0)
    {
        return (Vector3D_susie)
        {
            vector.x / scalar,
            vector.y / scalar,
            vector.z / scalar
        };        
    }
    return (Vector3D_susie){0.f,0.f,0.f};  
}

//MAGNITUDE

float Vector2DMagnitude_susie(Vector2D_susie vector)
{
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

float Vector3DMagnitude_susie(Vector3D_susie vector)
{
    return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}


//NORMALISATION

Vector2D_susie Vector2DNormalise_susie(Vector2D_susie vector)
{
    float magnitude = Vector2DMagnitude_susie(vector);
    if (magnitude != 0)
    {
        return Vector2DScalarDivision_susie(vector,magnitude);
    }
    return (Vector2D_susie){0.f,0.f};
}

Vector3D_susie Vector3DNormalise_susie(Vector3D_susie vector)
{
    float magnitude = Vector3DMagnitude_susie(vector);
    if (magnitude != 0)
    {
        return Vector3DScalarDivision_susie(vector,magnitude);
    }
    return (Vector3D_susie){0.f,0.f,0.f};
}

//LIMIT
Vector2D_susie Vector2DLimit_susie(Vector2D_susie vector, float limit)
{
    float magnitude = Vector2DMagnitude_susie(vector);
    if (magnitude > limit)
    {
        return Vector2DScalarMultiplication_susie(Vector2DNormalise_susie(vector),limit);
        //return Vector2DScalarMultiplication_susie(vector,limit/magnitude);
    }
    return vector;
}

Vector3D_susie Vector3DLimit_susie(Vector3D_susie vector, float limit)
{
    float magnitude = Vector3DMagnitude_susie(vector);
    if (magnitude > limit)
    {
        return Vector3DScalarMultiplication_susie(vector,limit/magnitude);
    }
    return vector;    
}

//RANDOM

Vector2D_susie Vector2DRand_susie()
{
    return Vector2DNormalise_susie((Vector2D_susie){(float)GetRandomValue(-1000,1000),(float)GetRandomValue(-1000,1000)});
}

Vector3D_susie Vector3DRand_susie()
{
    return Vector3DNormalise_susie((Vector3D_susie){(float)GetRandomValue(-1000,1000),
                                    (float)GetRandomValue(-1000,1000),
                                    (float)GetRandomValue(-1000,1000)});
}

//EMPTY

Vector2D_susie Vector2DEmpty_susie()
{
    return (Vector2D_susie){0.f,0.f};
}

Vector3D_susie Vector3DEmpty_susie()
{
    return (Vector3D_susie){0.f,0.f,0.f};
}