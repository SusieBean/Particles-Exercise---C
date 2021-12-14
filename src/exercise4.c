#include "raylib.h"
#include "susievector.h"
#include "susietrig.h"

#define SCREENWIDTH 600
#define SCREENHEIGHT 400

#define MAXNUMPARTICLES 30


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
    float totallifespan;
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

typedef struct SpaceShip
{
        union 
    {
        Vector2D_susie position;
        struct 
        {
            float x;
            float y;
        };
        
    };
    
    Vector2D_susie velocity;
    Vector2D_susie acceleration;

    float mass;
    float width;
    float height;
    float maximum_speed;

    union 
    {
        Angle angle;
        struct 
        {
            float angleValue;
            enum AngleType angleType;
        };
        
    };

    float A_Velocity;
    float A_Acceleration;
    float A_maximum_speed;
}SpaceShip;


Particle CreateParticle(const float x, const float y)
{
    Particle particle;
    particle.x = x;
    particle.y = y;
    particle.velocity = (Vector2D_susie){RandomFloat(-0.5f,0.5f),RandomFloat(-0.5f,0.5f)};
    particle.acceleration = Vector2DEmpty_susie();
    particle.lifespan = 1.0f;
    particle.totallifespan = particle.lifespan;
    particle.mass = 1.f;
    particle.angleValue = 0.f;
    particle.angleType = Radians;
    particle.aVelocity = 0.05f;
    particle.isAlive = false;
    return particle;
}

ParticleSystem CreateSystem(const float x, const float y,float rate, int NumOfParticles)
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
    DrawAccurateRect(particle.x, particle.y,20.f,20.f,particle.angle,(Color){255.f,255.f * (particle.lifespan),55.f,255.f * particle.lifespan});
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


float Clamp_Float(float value, float min, float max)
{
    if (value < min)
    {
        return min;
    }
    if (value > max)
    {
        return max;
    }
    return value;
}


SpaceShip MakeSpaceShip()
{
    SpaceShip ship;
    ship.x = SCREENWIDTH/2;
    ship.y = SCREENHEIGHT/2;
    ship.velocity = Vector2DEmpty_susie();
    ship.acceleration = Vector2DEmpty_susie();
    ship.mass = 10.f;
    ship.maximum_speed = 5.f;
    ship.angleValue = 0.f;
    ship.angleType = Radians;
    ship.A_Velocity = 0.f;
    ship.A_Acceleration = 0.f;
    ship.A_maximum_speed = 1.0f;
    ship.width = 10.f;
    ship.height = 20.f; 

    return ship;
}

void UpdateShip(SpaceShip * ship)
{
    ship->angleValue += 0.05f*((int)IsKeyDown(KEY_LEFT) - (int)IsKeyDown(KEY_RIGHT));

    ship->acceleration = Vector2DScalarMultiplication_susie(Translate_susie(ship->angle,1.f),(int)IsKeyDown(KEY_Z));
    ship->velocity = Vector2DAdd_susie(ship->velocity,ship->acceleration);
    ship->velocity =Vector2DScalarMultiplication_susie(ship->velocity,0.95f);
    ship->velocity = Vector2DLimit_susie(ship->velocity,ship->maximum_speed);
    ship->position = Vector2DAdd_susie(ship->position,ship->velocity);

    if (ship->x >= SCREENWIDTH || ship->x < 0)
    {
        ship->x = SCREENWIDTH - ship->x;
    }
    if (ship->y >= SCREENHEIGHT || ship->y < 0)
    {
        ship->y = SCREENHEIGHT - ship->y;
    }

    ship->acceleration = Vector2DEmpty_susie();

}

void DrawShip(SpaceShip * ship)
{
    DrawRectanglePro((Rectangle){ship->x,ship->y,ship->height,ship->width},(Vector2){0.f,0.f},
        RadiansToDegrees_susie(ship->angle).value,YELLOW);
}

int main()
{
    InitWindow(SCREENWIDTH,SCREENHEIGHT,"Particles Exercise 4");
    SetTargetFPS(60);
    SpaceShip ship = MakeSpaceShip();
    ParticleSystem particleSystem = CreateSystem(ship.x,ship.y,0.02,MAXNUMPARTICLES);
    while(!WindowShouldClose())
    {
        UpdateShip(&ship);
        particleSystem.origin = ship.position;
        if (IsKeyDown(KEY_Z))
        {
            particleSystem.currentrate+= GetFrameTime();
            if (particleSystem.currentrate>= particleSystem.rate)
            {
                particleSystem.currentrate = 0.f;
                
                int NewParticle = UnusedParticles(particleSystem);
                if(NewParticle != -1)
                {
                    Angle angle = ship.angle;
                    angle.value += RandomFloat(-0.3f,0.3f);
                    particleSystem.particles[NewParticle] = CreateParticle(particleSystem.x,particleSystem.y);
                    particleSystem.particles[NewParticle].isAlive = true;
                    particleSystem.particles[NewParticle].velocity = Translate_susie(angle,-5.f);
                }
            }
        }
        ParticleSystemUpdate(&particleSystem);
        BeginDrawing();
        ClearBackground(BLACK);
        DisplayParticleSystem(particleSystem);
        DrawShip(&ship);
        EndDrawing();

    }
    CloseWindow();
    return 0;
}