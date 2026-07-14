#ifndef MATRIX_H
#define MATRIX_H

#include "vector3d.h"

typedef struct{
	double m[3][3];
}Mat3;


Mat3 mat3_rotation_x(double angle);
Mat3 mat3_rotation_y(double angle);
Mat3 mat3_rotation_z(double angle);


#endif
