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
