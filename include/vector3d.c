#include "vector3d.h"

Vec3 vec3_add(Vec3 a, Vec3 b){
	Vec3 r;
	
	r.x = a.x + b.x;
	r.y = a.y + b.y;
	r.z = a.z + b.z;
	
	return r;
}
