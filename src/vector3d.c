#include "vector3d.h"
#include <math.h>

// x, y e z são definidos na utlização da struct Vec3.
// a.x é o elemento x do vetor, e assim por diante.
// Esta definido no arquivo vector3d.h.

Vec3 vec3_add(Vec3 a, Vec3 b) {
    // Retorna soma de todos os elementos dos dois vetores
    return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z}; 
}

Vec3 vec3_sub(Vec3 a, Vec3 b) {
    // Retorna subtração de todos os elementos dos dois vetores
    return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec3 vec3_scale(Vec3 v, double s) {
    // escalonamento de todos os elementos do vetor
    // a partir de um lambda
    // Retorna o vetor multiplicado pelo escalar s
    return (Vec3){v.x * s, v.y * s, v.z * s};
}

double vec3_dot(Vec3 a, Vec3 b) {
    // produto escalar dos dois vetores
    // multiplicação dos elementos
    // Retorna a soma dos produtos dos elementos correspondentes
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 vec3_product(Vec3 a, Vec3 b) {
    // produto vetorial dos dois vetores
    // Retorna o vetor perpendicular aos dois vetores
    return (Vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

double vec3_length(Vec3 v) {
    // norma ou módulo do vetor, ou seja, o comprimento do vetor
    // raiz quadrada da soma dos quadrados dos elementos
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vec3 vec3_normalize(Vec3 v) {
    // normalização do vetor, ou seja, o vetor unitário na mesma direção
    double len = vec3_length(v);
    if (len < 1e-12) {  // vetor nulo
        return (Vec3){0.0, 0.0, 0.0};
    }
    return vec3_scale(v, 1.0 / len);
}

double vec3_distance(Vec3 a, Vec3 b) {
    // distância entre dois pontos no espaço 3D
    // é a norma do vetor diferença entre os dois pontos
    Vec3 diff = vec3_sub(a, b);
    return vec3_length(diff);
}

Vec3 vec3_torque(Vec3 r, Vec3 F) {
    // torque = r x F
    return vec3_product(r, F);
}

void vec3_print(Vec3 v, const char* nome) {
    printf("%s = (%.4f, %.4f, %.4f)\n", nome, v.x, v.y, v.z);
}