#ifndef QUATERNIOM_H // Include guard, garante leitura única do arquivo.
#define QUATERNIOM_H

#include "vector3d.h"

typedef struct {
    double w; // Parte escalar (cos(theta/2)).
    double x, y, z; // Parte vetorial (eixo * sin(theta/2)).
} Quat;

// Multiplicação de Quatérnios (Combina duas rotações em uma).
Quat quat_mul(Quat q1, Quat q2);

// Conjugado de um Quatérnio (Inverte o sentido da rotação).
Quat quat_conjugate(Quat q);

// Normalização (Garante que o quatérnio continue unitário).
Quat quat_normalize(Quat q);

// Converte um vetor velocidade angular (w) em um quatérnio puro para a derivada.
Quat quat_from_omega(Vec3 omega);

// Cria um quatérnio de rotação a partir de um ângulo (em radianos) e um eixo unitário.
Quat quat_from_axis_angle(Vec3 axis, double angle);

#endif