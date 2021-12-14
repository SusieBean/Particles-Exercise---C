#include "raylib.h"
#include "susievector.h"
#include "susietrig.h"

#define SCREENWIDTH 600
#define SCREENHEIGHT 400

#define MAXNUMPARTICLES 10
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
    float maxlifespan;
    float mass;
    bool isAlive;

    void (*DrawParticle)(struct Particle *);
}Particle;

void DisplayParticle(Particle * particle);
void DrawConfetti(Particle * particle);

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
    bool isActive;


}ParticleSystem;

Particle CreateParticle(const float x, const float y, float rate,int particlenum)
{
    Particle particle;
    particle.x = x;
    particle.y = y;
    particle.velocity = (Vector2D_susie){RandomFloat(-1.f,1.f),0.f};
    particle.acceleration = Vector2DEmpty_susie();
    particle.lifespan = rate * (float)particlenum;
    particle.maxlifespan = particle.lifespan;
    particle.mass = 1.f;
    particle.angleValue = 0.f;
    particle.angleType = Radians;
    particle.aVelocity = 0.05f;
    particle.isAlive = false;
    particle.DrawParticle = &DisplayParticle;
    return particle;
}

Particle CreateConfetti(const float x, const float y, float rate,int particlenum)
{
    /*
    Particle particle;
    particle.x = x;
    particle.y = y;
    particle.velocity = (Vector2D_susie){RandomFloat(-1.f,1.f),0.f};
    particle.acceleration = Vector2DEmpty_susie();
    particle.lifespan = rate * (float)particlenum;
    particle.maxlifespan = particle.lifespan;
    particle.mass = 1.f;
    particle.angleValue = 0.f;
    particle.angleType = Radians;
    particle.aVelocity = 0.05f;
    particle.isAlive = false;
    */
    Particle particle = CreateParticle(x,y,rate,particlenum);
    particle.DrawParticle = &DrawConfetti;
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
    system.isActive = false;

    
    system.rate = rate;
    system.currentrate = 0;
    int Count;
    for (Count = 0;Count<NumOfParticles/2;Count++)
    {
        system.particles[2*Count + 1] = CreateParticle(x,y,rate,NumOfParticles);
        system.particles[2*Count] = CreateConfetti(x,y,rate,NumOfParticles);
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
        if (system->particles[Count].isAlive)
        {
            ParticleUpdate(system->particles + Count);
            if (ParticleIsDead(*(system->particles + Count)))
            {
                
            (system->particles + Count)->isAlive = false; 
            //system->isActive = !(Count == system->NumOfParticles-1 && !system->particles[Count].isAlive);
            
            }
        }

    }
}

void DisplayParticle(Particle * particle)
{
    
    //DrawAccurateRect(particle.x, particle.y,20.f,20.f,particle.angle,(Color){255.f,255.f,255.f,255.f/4.f * particle.lifespan});
    DrawCircle(particle->x,particle->y,10.f,(Color){100.f + 150.f*(particle->maxlifespan - particle->lifespan)/particle->maxlifespan,100.f + 150.f * particle->lifespan/particle->maxlifespan,255.f,255.f/particle->maxlifespan * particle->lifespan});
}

void DrawConfetti(Particle * particle)
{
    DrawCircle(particle->x,particle->y,10.f,(Color){255.f,255.f,255.f,255.f});
    //DrawAccurateRect(particle->x, particle->y,20.f,20.f,particle->angle,(Color){255.f,255.f,255.f,255.f/particle->maxlifespan * particle->lifespan});
}

void DisplayParticleSystem(ParticleSystem system)
{
    int Count;
    for (Count = 0;Count<system.NumOfParticles;Count++)
    {
        if (system.particles[Count].isAlive)
        {
            void (*Draw)(Particle*) = system.particles[Count].DrawParticle;
            (Draw)(&system.particles[Count]);
            //DisplayParticle(system.particles[Count]);
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

int UnusedSystems(ParticleSystem * systems, int Number)
{
    int Count;
    for (Count = 0;Count<Number;Count++)
    {
        if (!(systems + Count)->isActive)
        {
            return Count;
        }
    }
    return -1;
}

Particle resetParticle(Particle particle,float x, float y)
{
    particle.x = x;
    particle.y = y;
    particle.velocity = (Vector2D_susie){RandomFloat(-1.f,1.f),0.f};
    particle.acceleration = Vector2DEmpty_susie();
    particle.lifespan = particle.maxlifespan;
    particle.angleValue = 0.f;
    particle.aVelocity = 0.05f;
    particle.isAlive = false;
    return particle;
}

int main()
{
    InitWindow(SCREENWIDTH,SCREENHEIGHT,"Particles Example 5");
    SetTargetFPS(60);
    ParticleSystem system;
    int Count;
    system = CreateSystem(SCREENWIDTH/2,20,0.25,MAXNUMPARTICLES);
    system.isActive = true;
    Vector2D_susie gravity = (Vector2D_susie){0.f,0.025f};

    while(!WindowShouldClose())
    {
        
        if (system.isActive)
        {
            system.currentrate+=GetFrameTime();
            
            if (system.currentrate>= system.rate)
            {
                int NewParticle = UnusedParticles(system);
                system.currentrate = 0.f;
                if (NewParticle != -1)
                {
                    system.particles[NewParticle] = resetParticle(system.particles[NewParticle],system.x,system.y);
                    system.particles[NewParticle].isAlive = true;
                }
                
            }

            system = ApplyForceToParticles(system,gravity);
            ParticleSystemUpdate(&system);
        }



        BeginDrawing();
        ClearBackground(BLACK);

        if (system.isActive)
            {
                DisplayParticleSystem(system);
            }
        
        EndDrawing();
    }
    CloseWindow();

    return 0;
}