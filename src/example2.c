#include "raylib.h"
#include "susievector.h"
#include "susietrig.h"

#define SCREENWIDTH 600
#define SCREENHEIGHT 400

#define MAXNUMPARTICLES 100

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
    bool isAlive;
}Particle;

typedef struct ParticleSystem
{
    Particle particles[MAXNUMPARTICLES];
    int NumOfParticles;
    union 
    {
        Vector2D_susie origin;
        struct
        {
            float x;
            float y;
        };
    };
    

}ParticleSystem;

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
    particle.isAlive = false;
    return particle;
}

ParticleSystem CreateSystem(const float x, const float y, int NumOfParticles)
{
    ParticleSystem system;
    if (NumOfParticles > MAXNUMPARTICLES)
    {
        NumOfParticles = MAXNUMPARTICLES;
    }
    system.NumOfParticles = NumOfParticles;
    system.x = x;
    system.y = y;
    int Count;
    for (Count = 0;Count<NumOfParticles;Count++)
    {
        system.particles[Count] = CreateParticle(x,y);
    }
    return system;
}

void ParticleUpdate(Particle * particle)
{
    if (particle->isAlive)
    {
        particle->velocity = Vector2DAdd_susie(particle->acceleration,particle->velocity);
        particle->location = Vector2DAdd_susie(particle->velocity,particle->location);
        particle->lifespan-= GetFrameTime();
        particle->acceleration = Vector2DEmpty_susie();
        particle->angleValue+= particle->aVelocity;
        if (ParticleIsDead(*particle))
        {
           *particle = CreateParticle(SCREENWIDTH/2,10); 
        }
    }
    
}

void ParticleSystemUpdate(ParticleSystem * system)
{
    int Count;
    for (Count = 0;Count<system->NumOfParticles;Count++)
    {
        ParticleUpdate(system->particles + Count);
    }
}

void DisplayParticle(Particle particle)
{
    DrawAccurateRect(particle.x, particle.y,20.f,20.f,particle.angle,(Color){255.f,255.f,255.f,255.f/4.f * particle.lifespan});
}

void DisplayParticleSystem(ParticleSystem system)
{
    int Count;
    for (Count = 0;Count<system.NumOfParticles;Count++)
    {
        if (system.particles[Count].isAlive)
        {
            DisplayParticle(system.particles[Count]);
        }
    }
}

bool ParticleIsDead(Particle particle)
{
    return particle.lifespan <= 0.f;
}

Vector2D_susie ApplyForce(Vector2D_susie initial_acceleration,Vector2D_susie force, float mass)
{
    return Vector2DAdd_susie(Vector2DScalarDivision_susie(force,mass),initial_acceleration);
}

ParticleSystem ApplyForceToParticles(ParticleSystem system,Vector2D_susie force)
{
    int Count;
    for (Count = 0;Count<system.NumOfParticles;Count++)
    {
        if (system.particles[Count].isAlive)
        {
            system.particles[Count].acceleration = ApplyForce(system.particles[Count].acceleration,force,system.particles[Count].mass);
        }
    }
    return system;
}

int UnusedParticles(ParticleSystem particlesystem)
{
    int Count;
    for (Count = 0;Count<particlesystem.NumOfParticles;Count++)
    {
        if (!particlesystem.particles[Count].isAlive)
        {
            return Count;
        }
    }
    return -1;
}

int main()
{
    InitWindow(SCREENWIDTH,SCREENHEIGHT,"Particles Example 2");
    SetTargetFPS(60);
    //Particle particle = CreateParticle(SCREENWIDTH/2,10);
    ParticleSystem system = CreateSystem(SCREENWIDTH/2,10,MAXNUMPARTICLES);
    Vector2D_susie gravity = (Vector2D_susie){0.f,0.025f};
    float rate = 0.05f;
    float current = 0.f;
    while(!WindowShouldClose())
    {
        current+=GetFrameTime();
        if (current>= rate)
        {
            current = 0.f;
            int NewParticle = UnusedParticles(system);
            if (NewParticle != -1)
            {
                system.particles[NewParticle].isAlive = true;
            }
        }

        system = ApplyForceToParticles(system,gravity);
        ParticleSystemUpdate(&system);

        BeginDrawing();
        ClearBackground(DARKBLUE);
        DisplayParticleSystem(system);
        EndDrawing();

        /*
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
        */
    }
    CloseWindow();

    return 0;
}