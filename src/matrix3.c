#include "matrix3.h"
#include <math.h>

Mat3 mat3_rotation_x(double angle){
	double s = sin(angle);
	double c = cos(angle);

	return (Mat3){{
		{1, 0, 0},
		{0, c, -s},
		{0, s, c}
	}};
}

Mat3 mat3_rotation_y(double angle){
	double s = sin(angle);
        double c = cos(angle);

        return (Mat3){{
                {c, 0, s},
                {0, 1, 0},
                {-s, 0, c}
        }};
}

Mat3 mat3_rotation_z(double angle){
	double s = sin(angle);
        double c = cos(angle);

        return (Mat3){{
                {c, -s, 0},
                {s, c, 0},
                {0, 0, 1}
        }};
}


Mat3 mat3_identity(){
	return (Mat3){{
	{1, 0, 0},
	{0, 1, 0,},
	{0, 0, 1,}
	}};
}

Mat3 mat3_mul(Mat3 mat_a, Mat3 mat_b){
	Mat3 mat_p;

	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			mat_p.m[i][j] = 0;

			for (int k = 0; k < 3; k++){
				mat_p.m[i][j] += mat_a[i][k] * mat_b[k][j];
			}
		}
	}

	return mat_p;
}

Vec3 mat3_mul_vec3(Mat3 mat_a, Vec3 v){
	Vec3 p;

	p.x = vec3_dot((Vec3){mat_a.m[0][0], mat_a.m[0][1], mat_a.m[0][2])}, v);
	p.y = vec3_dot((Vec3){mat_a.m[1][0], mat_a.m[1][1], mat_a.m[1][2])}, v);
	p.z = vec3_dot((Vec3){mat_a.m[2][0], mat_a.m[2][1], mat_a.m[2][2])}, v);

	return p;
}
