#include "integration.h"

void euler(State *state, double dt, DerivativeFunction evaluate){

	Derivative k;

	evaluate(state, &k);

	state->position = vec3_add(state->position, vec3_scale(k.dPosition, dt)); //Atualiza a posição
	state->velocity = vec3_add(state->velocity, vec3_scale(k.dVelocity, dt)); //Atualiza a velocidade
	//state->orientation = ; //Atualiza a orientação
	state->angularVelocity = vec3_add(state->angularVelocity, vec3_scale(k.dAngularVelocity, dt)); //Atualiza a velocidadae angular

}

void rk4(){
}
