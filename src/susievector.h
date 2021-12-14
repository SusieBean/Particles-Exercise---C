#ifndef SUSIE___VECTOR___GUARD
#define SUSIE___VECTOR___GUARD

#include "raylib.h"

//structs of vector types

typedef struct MyVector2D
{
    float x;
    float y;
} Vector2D_susie;

typedef struct MyVector3D
{
    float x;
    float y;
    float z;
} Vector3D_susie;


//----------------FUNCTIONS-----------------------------------

//Vector2 to susie Vector2D
Vector2D_susie Vector2D_raylibToSusie(Vector2 vector);
//Vector2D_susie to Vector2
Vector2 Vector2D_susieToRaylib(Vector2D_susie vector);

//Vector3 to susie Vector3D
Vector3D_susie Vector3D_raylibToSusie(Vector3 vector);
//Vector3D to raylib Vector3
Vector3 Vector3D_susieToRaylib(Vector3D_susie vector);


//----ADDITION----

//Adding two 2 dimensional vectors
Vector2D_susie Vector2DAdd_susie(Vector2D_susie vector1, Vector2D_susie vector2);
//Adding two 3 dimensional vectors 
Vector3D_susie Vector3DAdd_susie(Vector3D_susie vector1, Vector3D_susie vector2);

//----SUBTRACTION----
//Subtracting two 2 dimensional vectors
Vector2D_susie Vector2DSubtract_susie(Vector2D_susie vector1, Vector2D_susie vector2);
//Subtracting two 3 dimensional vectors 
Vector3D_susie Vector3DSubtract_susie(Vector3D_susie vector1, Vector3D_susie vector2);

//----SCALAR MULTIPLICATION----
//multiplying a 2D vector by a scalar
Vector2D_susie Vector2DScalarMultiplication_susie(Vector2D_susie vector, float scalar);
//multiplying a 3D vector by a scalar
Vector3D_susie Vector3DScalarMultiplication_susie(Vector3D_susie vector, float scalar);

//----SCALAR DIVISION----
//multiplying a 2D vector by a scalar
Vector2D_susie Vector2DScalarDivision_susie(Vector2D_susie vector, float scalar);
//multiplying a 3D vector by a scalar
Vector3D_susie Vector3DScalarDivision_susie(Vector3D_susie vector, float scalar);

//----MAGNITUDE----
//find magnitude of a 2D vector
float Vector2DMagnitude_susie(Vector2D_susie vector);
//find magnitude of a 2D vector
float Vector3DMagnitude_susie(Vector3D_susie vector);

//----NORMALISING VECTORS----
//returns same 2D vector with a magnitude of 1
Vector2D_susie Vector2DNormalise_susie(Vector2D_susie vector);
//returns same 3D vector with a magnitude of 1
Vector3D_susie Vector3DNormalise_susie(Vector3D_susie vector);


//---LIMIT VECTORS
//if magnitude is higher than value, return 2D vector with same magnitude as maximum
Vector2D_susie Vector2DLimit_susie(Vector2D_susie vector, float limit);
//if magnitude is higher than value, return 2D vector with same magnitude as maximum
Vector3D_susie Vector3DLimit_susie(Vector3D_susie vector, float limit);

//---RAND DIRECTION
//create a random normalised 2D vector
Vector2D_susie Vector2DRand_susie();
//create a random normalised 3D vector
Vector3D_susie Vector3DRand_susie();

//---EMPTY VECTOR
//create empty vector2D
Vector2D_susie Vector2DEmpty_susie();
//create empty vector3D
Vector3D_susie Vector3DEmpty_susie();


#endif