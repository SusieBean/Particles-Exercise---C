#include "raylib.h"
#include "susievector.h"
#include "susietrig.h"

#define SCREENWIDTH 600
#define SCREENHEIGHT 400

float RandomFloat(float min, float max)
{
    float Random = (float)GetRandomValue(0,10000)/10000.f;
    float difference = max - min;
    return min + Random*difference;
}

typedef struct Particle
{
    union 
    {
        Vector2D_susie location;
        struct
        {
            float x;
            float y;
        };
    };
    Vector2D_susie velocity;
    Vector2D_susie acceleration;
    union{
        Angle angle;
        struct
        {
            float angleValue;
            enum AngleType angleType;
        };
    };
    float aVelocity;
    float lifespan;
    float mass;
}Particle;


Particle CreateParticle(const float x, const float y)
{
    Particle particle;
    particle.x = x;
    particle.y = y;
    particle.velocity = (Vector2D_susie){RandomFloat(-1.f,1.f),0.f};
    particle.acceleration = Vector2DEmpty_susie();
    particle.lifespan = 4.0f;
    particle.mass = 1.f;
    particle.angleValue = 0.f;
    particle.angleType = Radians;
    particle.aVelocity = 0.05f;
    return particle;
}

void ParticleUpdate(Particle * particle)
{
    particle->velocity = Vector2DAdd_susie(particle->acceleration,particle->velocity);
    particle->location = Vector2DAdd_susie(particle->velocity,particle->location);
    particle->lifespan-= GetFrameTime();
    particle->acceleration = Vector2DEmpty_susie();
    particle->angleValue+= particle->aVelocity;
}

void DisplayParticle(Particle particle)
{
    DrawAccurateRect(particle.x, particle.y,20.f,20.f,particle.angle,(Color){255.f,255.f,255.f,255.f/4.f * particle.lifespan});
}

bool ParticleIsDead(Particle particle)
{
    return particle.lifespan <= 0.f;
}

Vector2D_susie ApplyForce(Vector2D_susie initial_acceleration,Vector2D_susie force, float mass)
{
    return Vector2DAdd_susie(Vector2DScalarDivision_susie(force,mass),initial_acceleration);
}


int main()
{
    InitWindow(SCREENWIDTH,SCREENHEIGHT,"Particles Exercise 2");
    SetTargetFPS(60);
    Particle particle = CreateParticle(SCREENWIDTH/2,10);
    Vector2D_susie gravity = (Vector2D_susie){0.f,0.025f};
    while(!WindowShouldClose())
    {
        if (ParticleIsDead(particle))
        {
            particle = CreateParticle(SCREENWIDTH/2,10); 
        }
        particle.acceleration = ApplyForce(particle.acceleration,gravity,1.f);
        ParticleUpdate(&particle);


        BeginDrawing();
        ClearBackground(DARKBLUE);
        DisplayParticle(particle);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}