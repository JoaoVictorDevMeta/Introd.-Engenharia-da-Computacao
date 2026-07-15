#ifndef INTEGRATION_H
#define INTEGRATION_H

#include "vector3d.h"
#include "quaterniom.h"

typedef struct {
        Vec3 position;
        Vec3 velocity;

        Quat orientation;

        Vec3 angularVelocity;
} State;

typedef struct {
        Vec3 dPosition;
        Vec3 dVelocity;

        Quat dOrientation;

        Vec3 dAngularVelocity;
} Derivative;

typedef void (*DerivativeFunction)(
        const State *state,
        Derivative *derivative
);

void euler(State *state, double dt, DerivativeFunction evaluate);

void rk4(State *state, double dt, DerivativeFunction evaluate);




#endif
