#include "collisions.h"
#include <math.h>

AABBcube createHitBoxCube(RigidBody *part)
{
    AABBcube hitbox;
    hitbox.body = part;
    hitbox.min = (Vec3){
        part->position.x - part->shape.comprimento / 2,
        part->position.y - part->shape.profundidade / 2,
        part->position.z - part->shape.altura / 2};
    hitbox.max = (Vec3){
        part->position.x + part->shape.comprimento / 2,
        part->position.y + part->shape.profundidade / 2,
        part->position.z + part->shape.altura / 2};
    ;
    return hitbox;
}

AABBsphere createHitBoxSphere(RigidBody *part)
{
    AABBsphere hitbox;
    hitbox.body = part;
    hitbox.pos = (Vec3){
        part->position.x,
        part->position.y,
        part->position.z};
    hitbox.radius = part->shape.comprimento;

    return hitbox;
}

int testAABBoverlapCube(AABBcube *a, AABBcube *b)
{
    double d1x = b->min.x - a->max.x;
    double d2x = a->min.x - b->max.x;
    double d1y = b->min.y - a->max.y;
    double d2y = a->min.y - b->max.y;
    double d1z = b->min.z - a->max.z;
    double d2z = a->min.z - b->max.z;

    if (d1x > 0.0f || d1y > 0.0f || d1z > 0.0f)
    {
        return 0;
    }
    if (d2x > 0.0f || d2y > 0.0f || d2z > 0.0f)
    {
        return 0;
    }

    return 1;
}

int testAABBoverlapSphere(AABBsphere *a, AABBsphere *b)
{
    double dAB = vec3_distance(a->pos, b->pos);
    double radiusSum = a->radius + b->radius;

    if (dAB > radiusSum)
    {
        return 0;
    }

    return 1;
}
