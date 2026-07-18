#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "particulasmove.h"
#include "vector3d.h"

void PrintParticles(Particle particle[], int num_particles){

    for (int i = 0; i < num_particles; i++) {
     // Particle *particle = &particles[i];
      printf("particle[%i] (%.4lf, %.4lf, %.4lf)\n", i,
      particle[i].position.x, particle[i].position.y, particle[i].position.z);
 }

}

void InitializeParticles(Particle particle[], int num_particles) {
    for (int i = 0; i < num_particles; i++) {
        particle[i].position = (Vec3){rand() % 50, rand() % 50, rand() % 50};
        particle[i].velocity = (Vec3){0, 0, 0};
        particle[i].massa = 1;
    }
}

//------------------------Gravidade Comum--------------------------
Vec3 ComputeForceGravidade(Particle particle){

    return (Vec3){0,0, particle.massa * -9.81};

}

//---------------- Força da Mola -------------------------//

Vec3 ComputeForceMolaHorizontal(Particle particle, double molaConst){

   return (Vec3){(-molaConst) * particle.position.x, 0, 0};

}

//------------------------ Lei da Gravitação -----------------
Vec3 ComputeForcePlanetario2(Particle particle1, Particle particle2){

    Vec3 r = vec3_sub(particle2.position, particle1.position);
    double normaR = vec3_length(r);
    double G =  6.674*pow(10,-11);
    return (Vec3){((G * (particle1.massa * particle2.massa))/pow(normaR,3))*r.x,((G * (particle1.massa * particle2.massa))/pow(normaR,3))*r.y, ((G * (particle1.massa * particle2.massa))/pow(normaR,3))*r.z };

}

//-----------------Simulações gravidade com Euler -------------------------------

void RunSimulationGravidadeEuler(int totalSimulationTime, Particle particle[], int num_particles){
    
    float currentTime = 0; // This accumulates the time that has
    // passed.
    float dt = 1; // Each step will take one second.

    srand((unsigned)time(NULL));
    InitializeParticles(particle, num_particles);
    PrintParticles(particle, num_particles);

    while (currentTime < totalSimulationTime) {

        for (int i = 0; i < num_particles; i++) {

            Vec3 force = ComputeForceGravidade(particle[i]);
            Vec3 acceleration = (Vec3){force.x /
                particle[i].massa, force.y / particle[i].massa, force.z/particle[i].massa};

            particle[i].velocity.x += acceleration.x * dt;
            particle[i].velocity.y += acceleration.y * dt;
            particle[i].velocity.z += acceleration.z * dt;

            particle[i].position.x += particle[i].velocity.x * dt;
            particle[i].position.y += particle[i].velocity.y * dt;
            particle[i].position.z += particle[i].velocity.z * dt;
        }

        PrintParticles(particle, num_particles);
        currentTime += dt;
    }

}

//-----------------------Simulação Força da Mola--------------

void RunSimulationMolaHorizontal(int totalSimulationTime, Particle particle[], double molaConst){
   double posInicial;

   double currentTime = 0; // This accumulates the time that has
    // passed.
   double dt = 1; // Each step will take one second.

    srand((unsigned)time(NULL));
    InitializeParticles(particle, 1);
    PrintParticles(particle, 1);

    while (currentTime < totalSimulationTime) {
        
        if(currentTime == 0){

           posInicial = particle[0].position.x;

        }
        //for (int i = 0; i < num_particles; i++) {

            Vec3 force = ComputeForceMolaHorizontal(particle[0], molaConst);
            Vec3 acceleration = (Vec3){force.x /
                particle[0].massa, force.y / particle[0].massa, force.z/particle[0].massa};

            particle[0].velocity.x += acceleration.x * dt;
            particle[0].velocity.y += acceleration.y * dt;
            particle[0].velocity.z += acceleration.z * dt;

            particle[0].position.x = posInicial*cos(sqrt(molaConst/particle[0].massa)*dt);
            particle[0].position.y = 0;
            particle[0].position.z = 0;
        

        PrintParticles(particle, 1);
        currentTime += dt;
    } 


}

//------------------- Simulação Lei da Gravitação (2 corpos) ---------------
// Órbita gravitacional entre duas partículas, força calculada via
// ComputeForcePlanetario2 (F = G*m1*m2/r^2, na direção de r).
void RunSimulationGravity2(int totalSimulationTime, Particle particle[]){

    double currentTime = 0; // This accumulates the time that has
    // passed.
    double dt = 1; // Each step will take one second.

    srand((unsigned)time(NULL));
    InitializeParticles(particle, 2);

    // Massas bem maiores que 1 kg para a força gravitacional ser perceptível
    // (com m=1 kg, G*m1*m2 é desprezível e a "órbita" nunca sai do lugar).
    particle[0].massa = 5.972e24; // ~massa da Terra
    particle[1].massa = 7.348e22; // ~massa da Lua
    particle[0].position = (Vec3){0, 0, 0};
    particle[1].position = (Vec3){384400000, 0, 0}; // ~distância Terra-Lua (m)
    particle[0].velocity = (Vec3){0, 0, 0};
    particle[1].velocity = (Vec3){0, 1022, 0}; // velocidade orbital aproximada (m/s)

    PrintParticles(particle, 2);

    while (currentTime < totalSimulationTime) {

        // Força que particle[1] exerce sobre particle[0], e a reação (Newton III)
        Vec3 forceOn0 = ComputeForcePlanetario2(particle[0], particle[1]);
        Vec3 forceOn1 = ComputeForcePlanetario2(particle[1], particle[0]);

        Vec3 acceleration0 = (Vec3){forceOn0.x / particle[0].massa,
            forceOn0.y / particle[0].massa, forceOn0.z / particle[0].massa};
        Vec3 acceleration1 = (Vec3){forceOn1.x / particle[1].massa,
            forceOn1.y / particle[1].massa, forceOn1.z / particle[1].massa};

        particle[0].velocity = vec3_add(particle[0].velocity, vec3_scale(acceleration0, dt));
        particle[1].velocity = vec3_add(particle[1].velocity, vec3_scale(acceleration1, dt));

        particle[0].position = vec3_add(particle[0].position, vec3_scale(particle[0].velocity, dt));
        particle[1].position = vec3_add(particle[1].position, vec3_scale(particle[1].velocity, dt));

        PrintParticles(particle, 2);
        currentTime += dt;
    }
}