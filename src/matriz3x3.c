#include "matriz3x3.h"
#include <math.h>

Mat3 mat3_identity(void) {
    return (Mat3){{
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    }};
}

Mat3 mat3_rotation_x(double angle) {
    double s = sin(angle);
    double c = cos(angle);

    return (Mat3){{
        {1, 0, 0},
        {0, c, -s},
        {0, s, c}
    }};
}

Mat3 mat3_rotation_y(double angle) {
    double s = sin(angle);
    double c = cos(angle);

    return (Mat3){{
        {c, 0, s},
        {0, 1, 0},
        {-s, 0, c}
    }};
}

Mat3 mat3_rotation_z(double angle) {
    double s = sin(angle);
    double c = cos(angle);

    return (Mat3){{
        {c, -s, 0},
        {s, c, 0},
        {0, 0, 1}
    }};
}

Mat3 mat3_mul(Mat3 a, Mat3 b) {
    Mat3 p;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            p.m[i][j] = 0;

            for (int k = 0; k < 3; k++) {
                p.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }

    return p;
}

Vec3 mat3_mul_vec3(Mat3 a, Vec3 v) {
    Vec3 p;

    p.x = vec3_dot((Vec3){a.m[0][0], a.m[0][1], a.m[0][2]}, v);
    p.y = vec3_dot((Vec3){a.m[1][0], a.m[1][1], a.m[1][2]}, v);
    p.z = vec3_dot((Vec3){a.m[2][0], a.m[2][1], a.m[2][2]}, v);

    return p;
}

Mat3 mat3_transpose(Mat3 a) {
    Mat3 t;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            t.m[i][j] = a.m[j][i];
        }
    }

    return t;
}

Mat3 mat3_inverse(Mat3 a) {
    Mat3 inv;

    double det =
        a.m[0][0] * (a.m[1][1] * a.m[2][2] - a.m[1][2] * a.m[2][1]) -
        a.m[0][1] * (a.m[1][0] * a.m[2][2] - a.m[1][2] * a.m[2][0]) +
        a.m[0][2] * (a.m[1][0] * a.m[2][1] - a.m[1][1] * a.m[2][0]);

    if (fabs(det) < 1e-12) {
        return mat3_identity(); // Matriz singular: evita divisão por zero.
    }

    double invDet = 1.0 / det;
    Mat3 inv;

    inv.m[0][0] = (a.m[1][1] * a.m[2][2] - a.m[1][2] * a.m[2][1]) * invDet;
    inv.m[0][1] = (a.m[0][2] * a.m[2][1] - a.m[0][1] * a.m[2][2]) * invDet;
    inv.m[0][2] = (a.m[0][1] * a.m[1][2] - a.m[0][2] * a.m[1][1]) * invDet;

    inv.m[1][0] = (a.m[1][2] * a.m[2][0] - a.m[1][0] * a.m[2][2]) * invDet;
    inv.m[1][1] = (a.m[0][0] * a.m[2][2] - a.m[0][2] * a.m[2][0]) * invDet;
    inv.m[1][2] = (a.m[0][2] * a.m[1][0] - a.m[0][0] * a.m[1][2]) * invDet;

    inv.m[2][0] = (a.m[1][0] * a.m[2][1] - a.m[1][1] * a.m[2][0]) * invDet;
    inv.m[2][1] = (a.m[0][1] * a.m[2][0] - a.m[0][0] * a.m[2][1]) * invDet;
    inv.m[2][2] = (a.m[0][0] * a.m[1][1] - a.m[0][1] * a.m[1][0]) * invDet;

    return inv;
}

Mat3 mat3_from_quat(Quat q) {
    double x = q.x, y = q.y, z = q.z, w = q.w;

    double xx = x * x, yy = y * y, zz = z * z;
    double xy = x * y, xz = x * z, yz = y * z;
    double wx = w * x, wy = w * y, wz = w * z;

    return (Mat3){{
        {1 - 2 * (yy + zz), 2 * (xy - wz),     2 * (xz + wy)},
        {2 * (xy + wz),     1 - 2 * (xx + zz), 2 * (yz - wx)},
        {2 * (xz - wy),     2 * (yz + wx),     1 - 2 * (xx + yy)}
    }};
}
