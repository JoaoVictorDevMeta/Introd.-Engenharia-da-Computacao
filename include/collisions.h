#include "vector3d.h"
#include "corposmove.h"

typedef struct
{
    RigidBody *body; // Referencia para instancia original

    Vec3 min;
    Vec3 max;

    Vec3 vertices[8];
} AABBcube;

typedef struct
{
    RigidBody *body; // Referencia para instancia original

    Vec3 pos;
    double radius;
} AABBsphere;

void calculateBoxVertices(RigidBody *part, Vec3 vertices[8]);

AABBcube createHitBoxCube(RigidBody *part);
AABBsphere createHitBoxSphere(RigidBody *part);

int testAABBoverlapCube(AABBcube *a, AABBcube *b);
int testAABBoverlapSphere(AABBsphere *a, AABBsphere *b);
