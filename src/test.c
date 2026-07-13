#include <stdio.h>
#include "vector3d.h"

//utilizado para teste de funções e etc
//contém aleatoriedades.

int main() {
    Vec3 v1 = {1.0, 2.0, 3.0};
    Vec3 v2 = {4.0, 5.0, 6.0};

    Vec3 result = vec3_product(v1,v2);

    printf("Vetor resultado:%.1f %.1f %.1f\n", result.x, result.y, result.z);

    return 0;

}
