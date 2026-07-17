#include "vector3d.h"
#include "corposmove.h"

typedef struct
{
    RigidBody *body; // Referencia para instancia original

    Vec3 min;
    Vec3 max;
} AABBcube;

typedef struct
{
    RigidBody *body; // Referencia para instancia original

    Vec3 pos;
    int radius;
    Vec3 min;
    Vec3 max;
} AABBsphere;

AABBcube createHitBoxCube(RigidBody *part);
AABBsphere createHitBoxSphere(RigidBody *part);

int testAABBoverlapCube(AABBcube *a, AABBcube *b);
int testAABBoverlapSphere(AABBsphere *a, AABBsphere *b);