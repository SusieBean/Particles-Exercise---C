#include "raylib.h"
#include "susievector.h"
#include "susietrig.h"

#define SCREENWIDTH 600
#define SCREENHEIGHT 400

#define MAXNUMPARTICLES 60
#define MAXNUMSYSTEMS 3

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

    float rate;
    float currentrate;
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

ParticleSystem CreateSystem(const float x, const float y, float rate, int NumOfParticles)
{
    ParticleSystem system;
    if (NumOfParticles > MAXNUMPARTICLES)
    {
        NumOfParticles = MAXNUMPARTICLES;
    }
    system.NumOfParticles = NumOfParticles;
    system.x = x;
    system.y = y;
    
    system.rate = rate;
    system.currentrate = 0;
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
    }
    
}

void ParticleSystemUpdate(ParticleSystem * system)
{
    int Count;
    for (Count = 0;Count<system->NumOfParticles;Count++)
    {
        ParticleUpdate(system->particles + Count);
        if (ParticleIsDead(*(system->particles + Count)))
        {
            
           (system->particles + Count)->isAlive = false; 
        
        }
    }
}

void DisplayParticle(Particle particle)
{
    
    //DrawAccurateRect(particle.x, particle.y,20.f,20.f,particle.angle,(Color){255.f,255.f,255.f,255.f/4.f * particle.lifespan});
    DrawCircle(particle.x,particle.y,10.f,(Color){100.f + 150.f*(4.0 - particle.lifespan)/4.f,100.f + 150.f * particle.lifespan/4.f,255.f,255.f/4.f * particle.lifespan});
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
    InitWindow(SCREENWIDTH,SCREENHEIGHT,"Particles Example 4");
    SetTargetFPS(60);
    ParticleSystem systems[MAXNUMSYSTEMS];
    int CurrentNumSystems = 0;
    Vector2D_susie gravity = (Vector2D_susie){0.f,0.025f};

    while(!WindowShouldClose())
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CurrentNumSystems < MAXNUMSYSTEMS)
        {
            systems[CurrentNumSystems] = CreateSystem(GetMousePosition().x,GetMousePosition().y,0.06,MAXNUMPARTICLES);
            CurrentNumSystems++;
        }


        int Count;
        for (Count = 0;Count<CurrentNumSystems;Count++)
        {
            systems[Count].currentrate+=GetFrameTime();
            if (systems[Count].currentrate>= systems[Count].rate)
            {
                systems[Count].currentrate = 0.f;
                int NewParticle = UnusedParticles(systems[Count]);
                if (NewParticle != -1)
                {
                    systems[Count].particles[NewParticle] = CreateParticle(systems[Count].x,systems[Count].y);
                    systems[Count].particles[NewParticle].isAlive = true;
                }
            }

            systems[Count] = ApplyForceToParticles(systems[Count],gravity);
            ParticleSystemUpdate(&systems[Count]);
        }


        BeginDrawing();
        ClearBackground(BLACK);
        for (Count = 0;Count<CurrentNumSystems;Count++)
        {
            DisplayParticleSystem(systems[Count]);
        }
        
        EndDrawing();
    }
    CloseWindow();

    return 0;
}