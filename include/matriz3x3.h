#ifndef MATRIZ3X3_H
#define MATRIZ3X3_H

#include "vector3d.h"
#include "quaterniom.h"

typedef struct {
    double m[3][3]; // Classica matriz 3x3.
} Mat3;

// Matriz identidade 3x3.
Mat3 mat3_identity(void);

// Matrizes de rotação elementares em torno dos eixos X, Y e Z (ângulo em radianos).
Mat3 mat3_rotation_x(double angle);
Mat3 mat3_rotation_y(double angle);
Mat3 mat3_rotation_z(double angle);

// Multiplica duas matrizes (Mat3 x Mat3)
Mat3 mat3_mul(Mat3 a, Mat3 b);

// Multiplica uma matriz por um vetor (Transforma o vetor de local para global)
Vec3 mat3_mul_vec3(Mat3 a, Vec3 v);

// Transposta da Matriz (Para matrizes de rotação, a transposta é igual à inversa!)
Mat3 mat3_transpose(Mat3 a);

// Inversa de uma Matriz (Crucial para achar a velocidade angular: w = I_inv * L)
Mat3 mat3_inverse(Mat3 a);

// Cria uma matriz de rotação 3x3 a partir de um quatérnio
Mat3 mat3_from_quat(Quat q);

#endif
