// Documento header para definir as structs e funções relativas
// ao movimento dos corpos rígidos.

#ifndef CORPOSMOVE_H // Include guard, garante leitura única do arquivo.
#define CORPOSMOVE_H

#include <stdio.h>
#include <math.h>
#include "particulasmove.h"

typedef struct {        // Corpo rígido no formato de bloco retangular.
    double comprimento;
    double altura;
    double profundidade;
    double massa;
    Vec3 momentOfInertia;
} BoxShape;

//Definindo corpo rígido.
typedef struct {
    Vec3 position;
    Vec3 linearVelocity;
    Vec3 angle; //Ângulos entre os vetores i,j,k.
    Vec3 angularVelocity;
    Vec3 force;
    Vec3 torque;
    BoxShape shape;
} RigidBody;

//Cálculo do momento de inercia
void CalculateBoxInertia(BoxShape *boxShape);

//Printa a posição e ângulo do corpo rígido.
void PrintRigidBodies(RigidBody rigidBody[], int numBodies);

// Inicializa os corpos rígidos em uma posição aleatória, 
//velocidade linear e angular e com formato de caixa com dimensões aleatórias
void InitializeRigidBodies(RigidBody rigidBody[], int numBodies);

//Computa a força e torque no contexto da força da gravidade.
void ComputeForceAndTorqueGravidade(RigidBody rigidBody[], int numBodies);

//Simula no caso da força peso.
void simulaRigidoGravidade(RigidBody rigidBody[], int numBodies);

#endif