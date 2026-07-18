#include "collisions.h"
#include <math.h>

// Criacao de uma hitbox esferica para verificacoes simplificadas
AABB createHitBoxSphere(RigidBody *part)
{
    AABB hitbox;
    hitbox.pos = (Vec3){
        part->position.x,
        part->position.y,
        part->position.z};
    // Raio da esfera = Distancia do centro ate um vertice.
    hitbox.radius = vec3_distance(hitbox.pos, (Vec3){part->position.x + part->shape.comprimento / 2,
                                                     part->position.y + part->shape.profundidade / 2,
                                                     part->position.z + part->shape.altura / 2});
    return hitbox;
}

// Verificacao de colisoes entre hitboxes esfericas
int checkAABBoverlapSphere(AABB *partA, AABB *partB)
{
    double dAB = vec3_distance(partA->pos, partB->pos);
    double radiusSum = partA->radius + partB->radius;

    if (dAB > radiusSum)
    {
        return 0;
    }

    return 1;
}

// Calculo de posicao das vertices de um cuboide
void calculateBoxVertices(RigidBody *part, Vec3 vertices[8])
{
    double hx = part->shape.comprimento / 2;
    double hy = part->shape.profundidade / 2;
    double hz = part->shape.altura / 2;

    Vec3 vertices[8] = {
        {-hx, -hy, -hz}, {hx, -hy, -hz}, {-hx, -hy, hz}, {hx, -hy, hz}, {-hx, hy, -hz}, {hx, hy, -hz}, {-hx, hy, hz}, {hx, hy, hz}};

    double m[4][4] = {{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};

    // Converte os angulos de graus para radianos
    double radX = part->angle.x * 0.0174533;
    double radY = part->angle.y * 0.0174533;
    double radZ = part->angle.z * 0.0174533;

    // Calcula os senos e cossenos
    double cx = cos(radX), sx = sin(radX);
    double cy = cos(radY), sy = sin(radY);
    double cz = cos(radZ), sz = sin(radZ);

    // Monta a matriz de rotacao combinada
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

// Criacao de uma hitbox OBB
OBB createHitBoxCube(RigidBody *part)
{
    OBB hitbox;
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

// Verificacao de colisoes entre hitboxes OBB, retorna informacoes sobre a colisao
CollisionData checkOBBcollision(RigidBody *partA, RigidBody *partB)
{
    CollisionData result;
    result.isColliding = 0;
    result.overlap = 999999.0;

    OBB boxA = createHitBoxCube(partA);
    OBB boxB = createHitBoxCube(partB);

    Vec3 axes[15];

    // Extracao dos eixos da hitbox A
    Vec3 aX = vec3_normalize(vec3_sub(boxA.vertices[1], boxA.vertices[0]));
    Vec3 aY = vec3_normalize(vec3_sub(boxA.vertices[4], boxA.vertices[0]));
    Vec3 aZ = vec3_normalize(vec3_sub(boxA.vertices[2], boxA.vertices[0]));
    axes[0] = aX;
    axes[1] = aY;
    axes[2] = aZ;
    // Extracao dos eixos da hitbox B
    Vec3 bX = vec3_normalize(vec3_sub(boxB.vertices[1], boxB.vertices[0]));
    Vec3 bY = vec3_normalize(vec3_sub(boxB.vertices[4], boxB.vertices[0]));
    Vec3 bZ = vec3_normalize(vec3_sub(boxB.vertices[2], boxB.vertices[0]));
    axes[3] = bX;
    axes[4] = bY;
    axes[5] = bZ;
    // Eixos do produto vetorial cruzado
    axes[6] = vec3_product(aX, bX);
    axes[7] = vec3_product(aX, bY);
    axes[8] = vec3_product(aX, bZ);
    axes[9] = vec3_product(aY, bX);
    axes[10] = vec3_product(aY, bY);
    axes[11] = vec3_product(aY, bZ);
    axes[12] = vec3_product(aZ, bX);
    axes[13] = vec3_product(aZ, bY);
    axes[14] = vec3_product(aZ, bZ);

    // Projecao nos 15 eixos
    for (int i = 0; i < 15; i++)
    {
        if (axes[i].x == 0 && axes[i].y == 0 && axes[i].z == 0)
            continue;

        double minA = vec3_dot(boxA.vertices[0], axes[i]);
        double maxA = minA;
        double minB = vec3_dot(boxB.vertices[0], axes[i]);
        double maxB = minB;

        for (int j = 0; j < 8; j++)
        {
            double projectionA = vec3_dot(boxA.vertices[j], axes[i]);
            if (projectionA < minA)
                minA = projectionA;
            if (projectionA > maxA)
                maxA = projectionA;
            double projectionB = vec3_dot(boxB.vertices[j], axes[i]);
            if (projectionB < minA)
                minB = projectionB;
            if (projectionB > maxA)
                maxB = projectionB;
        }

        if (maxA < minB || maxB < minA)
            return result;

        double max = (maxA < maxB) ? maxA : maxB;
        double min = (minA > minB) ? minA : minB;

        double axisOverlap = max - min;

        if (axisOverlap < result.overlap)
        {
            result.overlap = axisOverlap;
            result.mtvAxis = axes[i];
        }
    }

    Vec3 directionAtoB = vec3_sub(partA->position, partB->position);
    if (vec3_dot(result.mtvAxis, directionAtoB) < 0)
    {
        result.mtvAxis = vec3_scale(result.mtvAxis, -1);
    }

    result.isColliding = 1;
    return result;
}

void resolveCollisionImpulse(RigidBody *partA, RigidBody *partB, Vec3 mtvAxis)
{
    Vec3 normal = vec3_normalize(mtvAxis);
    Vec3 relativeVelocity = vec3_sub(partA->linearVelocity, partB->linearVelocity);
    double velocityOnNormal = vec3_dot(relativeVelocity, normal); // velocidade ao longo da normal da colisao

    if (velocityOnNormal > 0)
        return; // Objetos ja estao se afastando

    double restitution = 0.5; // coeficiente de restituicao da colisao

    double impulseScalar = -(1.0 + restitution) * velocityOnNormal;
    impulseScalar *= (partA->shape.massa * partB->shape.massa) / (partA->shape.massa + partB->shape.massa);

    Vec3 impulse = vec3_scale(normal, impulseScalar);

    Vec3 impulseA = vec3_scale(impulse, 1.0 / partA->shape.massa);
    partA->linearVelocity = vec3_add(partA->linearVelocity, impulseA);
    Vec3 impulseB = vec3_scale(impulse, 1.0 / partB->shape.massa);
    partB->linearVelocity = vec3_sub(partB->linearVelocity, impulseB);
}

void collisionDetectEvent(RigidBody *partA, RigidBody *partB)
{
    AABB sphereA = createHitBoxSphere(partA);
    AABB sphereB = createHitBoxSphere(partB);

    if (!checkAABBoverlapSphere(&sphereA, &sphereB))
        return;

    // Caso o primeiro teste verifique colisao, um segundo teste mais preciso e realizado.

    CollisionData collisionResult = checkOBBcollision(partA, partB);

    if (!collisionResult.isColliding)
        return;

    resolveCollisionImpulse(partA, partB, collisionResult.mtvAxis);
}