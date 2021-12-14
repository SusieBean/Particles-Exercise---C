#include "raylib.h"
#include "susievector.h"

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
    float lifespan;
}Particle;


Particle CreateParticle(const float x, const float y)
{
    Particle particle;
    particle.x = x;
    particle.y = y;
    particle.velocity = (Vector2D_susie){RandomFloat(-1.f,1.f),RandomFloat(0.f,2.f)};
    particle.acceleration = (Vector2D_susie){0.f,0.025f};
    particle.lifespan = 4.0f;
    return particle;
}

void ParticleUpdate(Particle * particle)
{
    particle->velocity = Vector2DAdd_susie(particle->acceleration,particle->velocity);
    particle->location = Vector2DAdd_susie(particle->velocity,particle->location);
    particle->lifespan-= GetFrameTime();
}

void DisplayParticle(Particle particle)
{
    DrawCircle(particle.x,particle.y,2.f,(Color){255.f,255.f,255.f,255.f/4.f * particle.lifespan});
}

bool ParticleIsDead(Particle particle)
{
    return particle.lifespan <= 0.f;
}


int main()
{
    InitWindow(SCREENWIDTH,SCREENHEIGHT,"Particles Example 1");
    SetTargetFPS(60);
    Particle particle = CreateParticle(SCREENWIDTH/2,10);
    while(!WindowShouldClose())
    {
        if (ParticleIsDead(particle))
        {
            particle = CreateParticle(SCREENWIDTH/2,10); 
        }
        ParticleUpdate(&particle);


        BeginDrawing();
        ClearBackground(DARKBLUE);
        DisplayParticle(particle);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}