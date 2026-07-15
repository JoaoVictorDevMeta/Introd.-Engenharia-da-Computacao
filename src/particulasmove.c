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
    double normaR = vec3_lenght(r);
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

//void RunSimulationGravity2( int totalSimulation, Particle particle[]){
    
   // particle[0].position.x = 0;
  //  particle[0].position.y = 0;
//    particle[0].position.z = 0;

 //Vec3 r = vec3_sub(particle[1].position, particle[0].position);
    //Vec3 vRel; //Derivada da r, isto é, velocidade relativa.
    //double normaR = vec3_lenght(r);
    //double G =  6.674*pow(10,-11);    
    //double momentoAngular; //= vec3_normalize(vec3_product(r,));

   //double currentTime = 0; // This accumulates the time that has
    // passed.
   //double dt = 1; // Each step will take one second.

    //srand((unsigned)time(NULL));
    //InitializeParticles(particle, 2);
    //PrintParticles(particle, 2);

    
  //  while (currentTime < totalSimulationTime) {
    
            
            //Vec3 force = ComputeForcePlanetario2(particle[0],particle[1]);
            
            //for(int i = 0; i <2; i++){


            //Vec3 acceleration = (Vec3){force.x /
              //  particle[i].massa, force.y / particle[1].massa, force.z/particle[1].massa};

            //particle[i].velocity.x += acceleration.x * dt;
            //particle[i].velocity.y += acceleration.y * dt;
          //  particle[i].velocity.z += acceleration.z * dt;
            
        //}        

        //vRel = vec3_sub(particle[1].position, particle[0].position);
        //momentoAngular = vec3_normalize(vec3_product(v,vRel));
        //double p = (pow(momentoAngular,2))/(G*particle[0].massa); 
        //double e = ((pow(vRel,2))/2)-((G*particle[0].massa)/2);
        //double b = sqrt(1+((2*e*pow(momentoAngular,2))/(pow(G*particle[0].massa,2))));
        //double theta = acos((1/b)*((p/nomraR)))

        //for(int i = 0; i<2; i++){

          //  particle[i].velocity.x

        //}

    //    PrintParticles(particle, 2);
      //  currentTime += dt;
    //} 


   

//}