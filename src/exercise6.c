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
    float lifespan;
    float maxlifespan;
    int CurrentNumber;
    bool isActive;


}ParticleSystem;


typedef struct Vase
{
    union{
        Vector2D_susie position;
        struct
        {
            float x, y;
        };
    };
    Vector2D_susie velocity;
    Vector2D_susie acceleration;
    ParticleSystem particles;
    float radius;
    float mass;
    bool isSmashed;     

}Vase;

Particle CreateParticle(const float x, const float y, float lifespan)
{
    Particle particle;
    particle.x = x;
    particle.y = y;
    particle.velocity = (Vector2D_susie){RandomFloat(-1.f,1.f),0.f};
    particle.acceleration = Vector2DEmpty_susie();
    particle.lifespan = lifespan;
    particle.maxlifespan = particle.lifespan;
    particle.mass = 1.f;
    particle.angleValue = 0.f;
    particle.angleType = Radians;
    particle.aVelocity = 0.05f;
    return particle;
}

ParticleSystem CreateSystem(const float x, const float y, float lifespan, int NumOfParticles)
{
    ParticleSystem system;
    if (NumOfParticles > MAXNUMPARTICLES)
    {
        NumOfParticles = MAXNUMPARTICLES;
    }
    system.NumOfParticles = NumOfParticles;
    system.x = x;
    system.y = y;
    system.lifespan = lifespan;
    system.maxlifespan = system.lifespan;
    system.CurrentNumber = 0;
    system.isActive = false;
    int Count;
    for (Count = 0;Count<NumOfParticles;Count++)
    {
        system.particles[Count] = CreateParticle(x,y,lifespan);
    }
    return system;
}

Vase CreateVase(float x, float y,float mass, float radius,float lifespan, int NumOfParticles)
{
    Vase vase;
    vase.x = x;
    vase.y = y;
    vase.mass = mass;
    vase.velocity = Vector2DEmpty_susie();
    vase.acceleration = Vector2DEmpty_susie();
    vase.radius = radius;
    vase.particles = CreateSystem(x,y,lifespan,NumOfParticles);
    vase.isSmashed = false;
    return vase;
}

ParticleUpdate(Particle * particle)
{
    particle->velocity = Vector2DAdd_susie(particle->acceleration,particle->velocity);
    particle->location = Vector2DAdd_susie(particle->velocity,particle->location);
    particle->lifespan-= GetFrameTime();
    particle->acceleration = Vector2DEmpty_susie();
    particle->angleValue+= particle->aVelocity;
    
}

ParticleSystem ParticleSystemUpdate(ParticleSystem * system)
{
    if (!system->isActive)
    {
        return;
    }
    int Count;
    system->lifespan-=GetFrameTime();
    for (Count = 0;Count<system->NumOfParticles;Count++)
    {
        ParticleUpdate(system->particles + Count);
    }
    if (system->lifespan <0)
    {
        system->isActive = false;
    }
}

void VaseUpdate(Vase * vase)
{
    if (!vase->isSmashed)
    {
        vase->velocity = Vector2DAdd_susie(vase->velocity,vase->acceleration);
        vase->position = Vector2DAdd_susie(vase->position,vase->velocity);
        vase->acceleration = Vector2DEmpty_susie();
        return;
    }
    ParticleSystemUpdate(&vase->particles);
}

void DisplayParticle(Particle particle)
{
    
    //DrawAccurateRect(particle.x, particle.y,20.f,20.f,particle.angle,(Color){255.f,255.f,255.f,255.f/4.f * particle.lifespan});
    DrawCircle(particle.x,particle.y,10.f,(Color){100.f + 150.f*(particle.maxlifespan - particle.lifespan)/particle.maxlifespan,100.f + 150.f * particle.lifespan/particle.maxlifespan,255.f,255.f/particle.maxlifespan * particle.lifespan});
}

void DisplayParticleSystem(ParticleSystem system)
{
    int Count;
    for (Count = 0;Count<system.NumOfParticles;Count++)
    {
        DisplayParticle(system.particles[Count]);
    }
}

void DisplayVase(Vase vase)
{


    if (!vase.isSmashed)
    {
        DrawCircle(vase.x,vase.y,vase.radius,SKYBLUE);
    }
    else if (vase.particles.isActive)
    {
        DisplayParticleSystem(vase.particles);
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

ParticleSystem ApplyForceToParticles(ParticleSystem system,Vector2D_susie force, bool isConstant)
{
    int Count;
    for (Count = 0;Count<system.NumOfParticles;Count++)
    {
        float mass = 1.f;
        if (!isConstant)
        {
            mass = system.particles[Count].mass;
        }
        system.particles[Count].acceleration = ApplyForce(system.particles[Count].acceleration,force,mass);
    }
    return system;
}

void ApplyForceToVase(Vase * vase, Vector2D_susie force, bool isConstant)
{
    if (!vase->isSmashed)
    {
        float mass = 1.f * (int)isConstant + vase->mass*(int)!isConstant;
        vase->acceleration = ApplyForce(vase->acceleration,force,mass);
        return;
    }
    else if(!vase->particles.isActive)
    {
        return;
    }
    vase->particles = ApplyForceToParticles(vase->particles,force,isConstant);
}

void Smash(Vase * vase)
{
    Angle angle = FindAngle(vase->velocity);
    float magnitude = 2.f;
    angle.value *= -1.f;
    vase->particles = CreateSystem(vase->x,vase->y,2.0f,MAXNUMPARTICLES);
    int Count;
    for (Count = 0;Count<MAXNUMPARTICLES;Count++)
    {
        Angle currentangle = angle;
        currentangle.value += RandomFloat(M_PI * -0.5f,M_PI * 0.5f);
        vase->particles.particles[Count].velocity = Translate_susie(currentangle,magnitude);
        
    }
    vase->particles.isActive = true;
    vase->isSmashed = true;
}

void ParticleCollision(Vase * vase)
{
    int Count;
    for (Count = 0;Count < vase->particles.NumOfParticles;Count++)
    {
        if (vase->particles.particles[Count].y + 10.f > SCREENHEIGHT)
        {
            vase->particles.particles[Count].y = SCREENHEIGHT - 10.f;
            vase->particles.particles[Count].velocity.y *= -1;
        }
    }
}



int main()
{
    InitWindow(SCREENWIDTH,SCREENHEIGHT,"Particles Example 6");
    SetTargetFPS(60);
    Vase vase = CreateVase(SCREENWIDTH/2,20.f,10.f,20.f,2.f,MAXNUMPARTICLES);
    Vector2D_susie gravity = (Vector2D_susie){0.f,0.05f};

    while(!WindowShouldClose())
    {
        ApplyForceToVase(&vase,gravity,true);
        VaseUpdate(&vase);

        if (vase.y + vase.radius >= SCREENHEIGHT && !vase.isSmashed)
        {
            Smash(&vase);
        }
        if (vase.isSmashed && vase.particles.isActive)
        {
            ParticleCollision(&vase);
        }


        BeginDrawing();
        ClearBackground(BLACK);
        DisplayVase(vase);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}