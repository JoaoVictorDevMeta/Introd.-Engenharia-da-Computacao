#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "corposmove.h"

void CalculateBoxInertia(BoxShape *boxShape) {
    double m = boxShape -> massa;
    double w = boxShape -> comprimento;
    double h = boxShape -> altura;
    double d = boxShape -> profundidade;
    boxShape -> momentOfInertia.x = (1.0/12.0) * m * (pow(h,2)+pow(d,2));
    boxShape -> momentOfInertia.y = (1.0/12.0) * m * (pow(w,2)+pow(d,2));
    boxShape -> momentOfInertia.z = (1.0/12.0) * m * (pow(w,2)+pow(h,2));
}

void PrintRigidBodies(RigidBody rigidBody[], int numBodies) {
    for (int i = 0; i < numBodies; i++) {
        printf("Corpo[%i] p = (%.2lf, %.2lf, %.2lf), a = (%.2lf, %.2lf, %.2lf)\n", i,
            rigidBody[i].position.x, rigidBody[i].position.y, rigidBody[i].position.z,
            rigidBody[i].angle.x, rigidBody[i].angle.y, rigidBody[i].angle.z);
    }
}

void InitializeRigidBodies(RigidBody rigidBody[], int numBodies) {
    for (int i = 0; i < numBodies; i++) {
        rigidBody[i].position = (Vec3){rand() % 50, rand() % 50, rand() % 50};
        rigidBody[i].angle = (Vec3){((rand() % 360) / 360.f * 3.1415 * 2),((rand() % 360) / 360.f * 3.1415 * 2),((rand() % 360) / 360.f * 3.1415 * 2)};
        rigidBody[i].linearVelocity = (Vec3){0, 0, 0};
        rigidBody[i].angularVelocity = (Vec3){0,0,0};

        BoxShape shape;
        shape.massa = 10;
        shape.comprimento = 1 + rand() % 2;
        shape.altura = 1 + rand() % 2;
        shape.profundidade = 1 + rand() % 2;
        CalculateBoxInertia(&shape);
        rigidBody[i].shape = shape;
    }
}

void ComputeForceAndTorqueGravidade(RigidBody rigidBody[], int numBodies) {
    for (int i = 0; i < numBodies; i++) {
        Vec3 f = (Vec3){0, 0, 9.81 * rigidBody[i].shape.massa};
        rigidBody[i].force = f;

        Vec3 r = (Vec3){rigidBody[i].shape.profundidade / 2.0,
                        rigidBody[i].shape.comprimento / 2.0, 
                        rigidBody[i].shape.altura / 2.0};

        rigidBody[i].torque = vec3_product(r, rigidBody[i].force);
    }
}

void simulaRigidoGravidade(RigidBody rigidBody[], int numBodies) {
    float totalSimulationTime = 10;

    float currentTime = 0;
   
    float dt = 1; 

    srand((unsigned)time(NULL));
    InitializeRigidBodies(rigidBody, numBodies);
    PrintRigidBodies(rigidBody, numBodies);

    while (currentTime < totalSimulationTime) {
        // Atualiza forças e torques de todos os corpos antes de integrar
        ComputeForceAndTorqueGravidade(rigidBody, numBodies);

        for (int i = 0; i < numBodies; ++i) {

            Vec3 linearAcceleration =
                (Vec3){rigidBody[i].force.x / rigidBody[i].shape.massa,
                rigidBody[i].force.y / rigidBody[i].shape.massa, rigidBody[i].force.z/rigidBody[i].shape.massa};

            rigidBody[i].linearVelocity.x += linearAcceleration.x * dt;
            rigidBody[i].linearVelocity.y += linearAcceleration.y * dt;
            rigidBody[i].linearVelocity.z += linearAcceleration.z * dt;

            rigidBody[i].position.x += rigidBody[i].linearVelocity.x * dt;
            rigidBody[i].position.y += rigidBody[i].linearVelocity.y * dt;
            rigidBody[i].position.z += rigidBody[i].linearVelocity.z * dt;

            Vec3 angularAcceleration = (Vec3){rigidBody[i].torque.x / rigidBody[i].shape.momentOfInertia.x,
            rigidBody[i].torque.y / rigidBody[i].shape.momentOfInertia.y, rigidBody[i].torque.z / rigidBody[i].shape.momentOfInertia.z};

            rigidBody[i].angularVelocity.x += angularAcceleration.x * dt;
            rigidBody[i].angularVelocity.y += angularAcceleration.y * dt;
            rigidBody[i].angularVelocity.z += angularAcceleration.z * dt;
            
            rigidBody[i].angle.x += rigidBody[i].angularVelocity.x * dt;
            rigidBody[i].angle.y += rigidBody[i].angularVelocity.y * dt;
            rigidBody[i].angle.z += rigidBody[i].angularVelocity.z * dt;
        }

        PrintRigidBodies(rigidBody, numBodies);
        currentTime += dt;
    }
}