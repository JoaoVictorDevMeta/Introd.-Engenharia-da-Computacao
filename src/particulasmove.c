#include <stdio.h>
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

Vec3 ComputeForceGravidade(Particle particle){

    return (Vec3){0,0, particle.massa * -9.81};

}

Vec3 ComputeForceMolaHorizontal(Particle particle, double k){

   return (Vec3){(-k) * particle.position.x, 0, 0};

}


Vec3 ComputeForcePlanetario(Particle particle1, Particle particle2){

    Vec3 r = vec3_sub(particle1.position, particle2.position);
    double normaR = vec3_lenght(r);
    double G =  6.674*pow(10,-11);
    return (Vec3){((G * (particle1.massa * particle2.massa))/pow(normaR,3))*r.x,((G * (particle1.massa * particle2.massa))/pow(normaR,3))*r.y, ((G * (particle1.massa * particle2.massa))/pow(normaR,3))*r.z };

}

//-----------------Simulações para as forças com Euler-------------------------------

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

void RunSimulationGravidadeRK(int totalSimulationTime, Particle particle[], int num_particles);