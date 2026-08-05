#ifndef VETOR3D_H // Include guard, garante leitura única do arquivo.
#define VETOR3D_H

#include <stdio.h>
#include <math.h>

// Header File. Uma Lib
// Arquivo para estrutura de dados e operações com vetores 3D.
// Base da fundação matemática.

// Novo tipo de Dado, Vetor.
// Assim como em Cálculo Vetorial, aqui será representado por x, y e z.
// Poderia-se utilizar um array, mas pela quantidade exata de 3 elementos
// e pela clareza de leitura, a struct é mais adequada.

typedef struct {
    double x, y, z;
} Vec3;

Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_scale(Vec3 v, double s);

double vec3_dot(Vec3 a, Vec3 b);       // Produto escalar
Vec3   vec3_product(Vec3 a, Vec3 b);     // Produto vetorial

double vec3_length(Vec3 v);
Vec3   vec3_normalize(Vec3 v);         // Retorna (0,0,0) se v for nulo

// Distância entre dois pontos
double vec3_distance(Vec3 a, Vec3 b);

// Aplicação física: torque = r x F
Vec3 vec3_torque(Vec3 r, Vec3 F);

// Depuração
void vec3_print(Vec3 v, const char* nome);

#endif