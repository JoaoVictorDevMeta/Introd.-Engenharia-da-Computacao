#include "quaterniom.h"
#include <math.h>

Quat quat_add(Quat q1, Quat q2) {
    Quat res;
    res.w = q1.w + q2.w;
    res.x = q1.x + q2.x;
    res.y = q1.y + q2.y;
    res.z = q1.z + q2.z;
    return res;
}

Quat quat_scale(Quat q1, double s){
   Quat res;
   res.w = q1.w * s;
   res.x = q1.x * s;
   res.y = q1.y * s;
   res.z = q1.z * s;
   return res;
}

Quat quat_mul(Quat q1, Quat q2) {
    Quat res;
    res.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    res.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    res.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    res.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    return res;
}

Quat quat_conjugate(Quat q) {
    return (Quat){q.w, -q.x, -q.y, -q.z};
}

Quat quat_normalize(Quat q) {
    double len = sqrt(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
    if (len < 1e-12) {
        return (Quat){1.0, 0.0, 0.0, 0.0}; 
    }
    double inv_len = 1.0 / len;
    return (Quat){q.w * inv_len, q.x * inv_len, q.y * inv_len, q.z * inv_len};
}

Quat quat_from_omega(Vec3 omega) {
    return (Quat){0.0, omega.x, omega.y, omega.z};
}

Quat quat_from_axis_angle(Vec3 axis, double angle) {
    Quat q;
    double half_angle = angle * 0.5;
    double s = sin(half_angle);
    
    q.w = cos(half_angle);
    q.x = axis.x * s;
    q.y = axis.y * s;
    q.z = axis.z * s;
    
    return q;
}
