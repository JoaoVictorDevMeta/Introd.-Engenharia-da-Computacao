//Determinar movimento de partículas, isto é, apenas com massa, mas sem formato.

#ifndef PARTICULASMOVE_H // Include guard, garante leitura única do arquivo.
#define PARTICULASMOVE_H

#include <stdio.h>
#include <math.h>
#include "vector3d.h"

typedef struct{

  Vec3 position;
    Vec3 velocity;
    double massa;

} Particle;


//Printa as componentes vetoriais.
void PrintParticles(Particle particle[], int num_particles);


//Inicializa partículas em posição aleatória, com velocidade 0 e massa 1 kg.
void InitializeParticles(Particle particle[], int num_particles);

//Calcula o vetor força:

//Força gravitacional de um corpo com respeito à terra.
Vec3 ComputeForceGravidade(Particle particle);

//Lei de Hooke.
Vec3 ComputeForceMola(Particle particle);

//Movimento planetário.
Vec3 ComputeForcePlanetario(Particle particle1, Particle particle2);

//Roda a simulação de gravidade Euler.
void RunSimulationGravidadeEuler(int totalSimulationTime, Particle particle[], int num_particles);

//Simulação Runge Kutta.
void RunSimulationGravidadeRK(int totalSimulationTime, Particle particle[], int num_particles);

#endif