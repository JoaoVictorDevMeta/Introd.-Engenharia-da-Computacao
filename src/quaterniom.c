#include "quaterniom.h"
#include <math.h>

// Multiplica dois quatérnios (combina rotações)
Quat quat_mul(Quat q1, Quat q2) {
    Quat res;
    res.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    res.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    res.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    res.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    return res;
}

// Retorna o conjugado (inverso da rotação para quatérnios unitários)
Quat quat_conjugate(Quat q) {
    return (Quat){q.w, -q.x, -q.y, -q.z};
}

// Normaliza o quatérnio para evitar drifting numérico.
// drifting numérico é o desvio causado pela limitação
// de casa decimal em dados double e float.
// Garanita que a norma seja 1.0.
Quat quat_normalize(Quat q) {
    double len = sqrt(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
    if (len < 1e-12) {
        return (Quat){1.0, 0.0, 0.0, 0.0}; // Retorna a rotação identidade
    }
    double inv_len = 1.0 / len;
    return (Quat){q.w * inv_len, q.x * inv_len, q.y * inv_len, q.z * inv_len};
}

// Converte um vetor velocidade angular (rad/s) em um quatérnio puro
Quat quat_from_omega(Vec3 omega) {
    // Um quatérnio puro possui a parte escalar (w) igual a zero
    return (Quat){0.0, omega.x, omega.y, omega.z};
}

Quat quat_from_axis_angle(Vec3 axis, double angle) {
    Quat q;
    double half_angle = angle * 0.5;
    double s = sin(half_angle);
    
    // É vital que o eixo passado esteja normalizado!
    // Assumindo que 'axis' já venha normalizado:
    q.w = cos(half_angle);
    q.x = axis.x * s;
    q.y = axis.y * s;
    q.z = axis.z * s;
    
    return q;
}