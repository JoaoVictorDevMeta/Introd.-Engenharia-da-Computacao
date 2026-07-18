#include "collisions.h"
#include <math.h>

void calculateBoxVertices(RigidBody *part, Vec3 vertices[8])
{
    double hx = part->shape.comprimento / 2;
    double hy = part->shape.profundidade / 2;
    double hz = part->shape.altura / 2;

    Vec3 vertices[8] = {
        {-hx, -hy, -hz}, {hx, -hy, -hz}, {-hx, -hy, hz}, {hx, -hy, hz}, {-hx, hy, -hz}, {hx, hy, -hz}, {-hx, hy, hz}, {hx, hy, hz}};

    double m[4][4] = {{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};

    // Converte os ângulos de graus para radianos
    double radX = part->angle.x * 0.0174533;
    double radY = part->angle.y * 0.0174533;
    double radZ = part->angle.z * 0.0174533;

    // Calcula Senos e Cossenos
    double cx = cos(radX), sx = sin(radX);
    double cy = cos(radY), sy = sin(radY);
    double cz = cos(radZ), sz = sin(radZ);

    // Monta a matriz de rotação combinada (Ordem Y-X-Z)
    m[0][0] = cy * cz + sy * sx * sz;
    m[0][1] = cz * sx * sy - cy * sz;
    m[0][2] = cx * sy;

    m[1][0] = cx * sz;
    m[1][1] = cx * cz;
    m[1][2] = -sx;

    m[2][0] = cy * sx * sz - sy * cz;
    m[2][1] = sy * sz + cy * cz * sx;
    m[2][2] = cx * cy;

    for (int i = 0; i < 8; i++)
    {
        // Rotaciona o ponto local ao redor de (0,0,0)
        Vec3 rotatedV;

        rotatedV.x = vertices[i].x * m[0][0] + vertices[i].y * m[1][0] + vertices[i].z * m[2][0];
        rotatedV.y = vertices[i].x * m[0][1] + vertices[i].y * m[1][1] + vertices[i].z * m[2][1];
        rotatedV.z = vertices[i].x * m[0][2] + vertices[i].y * m[1][2] + vertices[i].z * m[2][2];

        // Move o ponto rotacionado para a posição final do objeto no mundo
        vertices[i].x = rotatedV.x + part->position.x;
        vertices[i].y = rotatedV.y + part->position.y;
        vertices[i].z = rotatedV.z + part->position.z;
    }
}

AABBcube createHitBoxCube(RigidBody *part)
{
    AABBcube hitbox;
    hitbox.body = part;

    calculateBoxVertices(part, hitbox.vertices);

    hitbox.min = hitbox.vertices[0];
    hitbox.max = hitbox.vertices[0];

    for (int i = 1; i < 8; i++)
    {
        // Atualiza os valores mínimos
        if (hitbox.vertices[i].x < hitbox.min.x)
            hitbox.min.x = hitbox.vertices[i].x;
        if (hitbox.vertices[i].y < hitbox.min.y)
            hitbox.min.y = hitbox.vertices[i].y;
        if (hitbox.vertices[i].z < hitbox.min.z)
            hitbox.min.z = hitbox.vertices[i].z;

        // Atualiza os valores máximos
        if (hitbox.vertices[i].x > hitbox.max.x)
            hitbox.max.x = hitbox.vertices[i].x;
        if (hitbox.vertices[i].y > hitbox.max.y)
            hitbox.max.y = hitbox.vertices[i].y;
        if (hitbox.vertices[i].z > hitbox.max.z)
            hitbox.max.z = hitbox.vertices[i].z;
    }

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
    hitbox.radius = vec3_distance(hitbox.pos, (Vec3){part->position.x + part->shape.comprimento / 2,
                                                     part->position.y + part->shape.profundidade / 2,
                                                     part->position.z + part->shape.altura / 2});
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
