#include "vector3d.h"
#include "corposmove.h"

// Gera uma hitbox esferica para facilitar verificacao de colisoes
typedef struct
{
    Vec3 pos;
    double radius;
} AABB;

AABB createHitBoxSphere(RigidBody *part);

int checkAABBoverlapSphere(AABB *a, AABB *b); // 0 para nao colidindo, 1 para colidindo

// Colisao OBB, mais precisa que a esferica se tratando de cubos
typedef struct
{
    Vec3 min;
    Vec3 max;
    Vec3 vertices[8];
} OBB;

typedef struct
{
    int isColliding; // 0 para nao colidindo, 1 para colidindo
    Vec3 mtvAxis;    // Vetor de translacao minima para empurrar os objetos
    double overlap;  // Distancia intersectada pelas partes
} CollisionData;

void calculateBoxVertices(RigidBody *part, Vec3 vertices[8]);

OBB createHitBoxCube(RigidBody *part);

CollisionData checkOBBcollision(RigidBody *a, RigidBody *b);

void correctPosition(RigidBody *a, RigidBody *b);

void resolveCollisionImpulse(RigidBody *a, RigidBody *b, Vec3 mtvAxis);

// Funcao que executa todo o processo de verificacao de colisao e correcao de posicao.
void collisionDetectEvent(RigidBody *a, RigidBody *b);