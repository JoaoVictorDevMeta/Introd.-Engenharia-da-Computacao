#include "vector3d.h"
#include <math.h>

// x, y e z são definidos na utlização da struct Vec3.
// a.x é o elemento x do vetor, e assim por diante.
// Esta definido no arquivo vector3d.h.

Vec3 vec3_add(Vec3 a, Vec3 b) {
    return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z}; 
}

Vec3 vec3_sub(Vec3 a, Vec3 b) {
    return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec3 vec3_scale(Vec3 v, double s) {
    return (Vec3){v.x * s, v.y * s, v.z * s};
}

double vec3_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 vec3_product(Vec3 a, Vec3 b) {
    return (Vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

double vec3_length(Vec3 v) {
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vec3 vec3_normalize(Vec3 v) {
    double len = vec3_length(v);
    if (len < 1e-12) {  // vetor nulo
        return (Vec3){0.0, 0.0, 0.0};
    }
    return vec3_scale(v, 1.0 / len);
}

double vec3_distance(Vec3 a, Vec3 b) {
    Vec3 diff = vec3_sub(a, b);
    return vec3_length(diff);
}

Vec3 vec3_torque(Vec3 r, Vec3 F) {
    return vec3_product(r, F);
}

void vec3_print(Vec3 v, const char* nome) {
    printf("%s = (%.4f, %.4f, %.4f)\n", nome, v.x, v.y, v.z);
}