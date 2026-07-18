#include "integration.h"

void euler(State *state, double dt, DerivativeFunction evaluate){

	Derivative k;

	evaluate(state, &k);

	state->position = vec3_add(state->position, vec3_scale(k.dPosition, dt)); //Atualiza a posição
	state->velocity = vec3_add(state->velocity, vec3_scale(k.dVelocity, dt)); //Atualiza a velocidade
	state->orientation = quat_normalize(quat_add(state->orientation, quat_scale(k.dOrientation, dt))); //Atualiza a orientação
	state->angularVelocity = vec3_add(state->angularVelocity, vec3_scale(k.dAngularVelocity, dt)); //Atualiza a velocidadae angular

}

static State stateAdvance(const State *state, const Derivative *d, double dt) {
	// Inicializa copiando tudo do estado original
    State result = *state;

	result.position = vec3_add(state->position, vec3_scale(d->dPosition, dt));
	result.velocity = vec3_add(state->velocity, vec3_scale(d->dVelocity, dt));
	result.orientation = quat_add(state->orientation, quat_scale(d->dOrientation, dt));
	result.angularVelocity = vec3_add(state->angularVelocity, vec3_scale(d->dAngularVelocity, dt));

	return result;
}

void rk4(State *state, double dt, DerivativeFunction evaluate){

	Derivative k1, k2, k3, k4;
	State temp;

	evaluate(state, &k1);

	temp = stateAdvance(state, &k1, dt / 2.0);
	evaluate(&temp, &k2);

	temp = stateAdvance(state, &k2, dt / 2.0);
	evaluate(&temp, &k3);

	temp = stateAdvance(state, &k3, dt);
	evaluate(&temp, &k4);

	// Média ponderada das 4 derivadas: (k1 + 2*k2 + 2*k3 + k4) / 6
	Vec3 dPosition = vec3_scale(
		vec3_add(vec3_add(k1.dPosition, vec3_scale(k2.dPosition, 2.0)),
		         vec3_add(vec3_scale(k3.dPosition, 2.0), k4.dPosition)),
		1.0 / 6.0
	);

	Vec3 dVelocity = vec3_scale(
		vec3_add(vec3_add(k1.dVelocity, vec3_scale(k2.dVelocity, 2.0)),
		         vec3_add(vec3_scale(k3.dVelocity, 2.0), k4.dVelocity)),
		1.0 / 6.0
	);

	Quat dOrientation = quat_scale(
		quat_add(quat_add(k1.dOrientation, quat_scale(k2.dOrientation, 2.0)),
		         quat_add(quat_scale(k3.dOrientation, 2.0), k4.dOrientation)),
		1.0 / 6.0
	);

	Vec3 dAngularVelocity = vec3_scale(
		vec3_add(vec3_add(k1.dAngularVelocity, vec3_scale(k2.dAngularVelocity, 2.0)),
		         vec3_add(vec3_scale(k3.dAngularVelocity, 2.0), k4.dAngularVelocity)),
		1.0 / 6.0
	);

	state->position = vec3_add(state->position, vec3_scale(dPosition, dt));
	state->velocity = vec3_add(state->velocity, vec3_scale(dVelocity, dt));
	state->orientation = quat_normalize(quat_add(state->orientation, quat_scale(dOrientation, dt)));
	state->angularVelocity = vec3_add(state->angularVelocity, vec3_scale(dAngularVelocity, dt));
}

// 1. Derivada Concreta para Gravidade (Linear e Angular)
// Calcula a taxa de variação de um corpo sob efeito da força peso.
// Assume-se que o State possui as propriedades físicas necessárias.

void derivGravidade(const State *state, Derivative *d) {
    // A derivada da posição é a própria velocidade atual
    d->dPosition = state->velocity;

    // A aceleração linear (derivada da velocidade) é a gravidade pura.
    // Se a massa estivesse em State, seria: Força_Peso / massa = (0, 0, massa * g) / massa = g
    d->dVelocity = (Vec3){0.0, 0.0, G_GRAVITY};

    // A derivada da orientação (quaternio) baseia-se na velocidade angular atual.
    // Fórmula clássica: dq/dt = 0.5 * w_quat * q
    d->dOrientation = quat_scale(quat_mul(quat_from_omega(state->angularVelocity), state->orientation), 0.5);

    // Na gravidade uniforme aplicada no centro de massa, não há torque gerado.
    // Logo, a aceleração angular (derivada da velocidade angular) é zero.
    d->dAngularVelocity = (Vec3){0.0, 0.0, 0.0};
}


// 2. Derivada Concreta para Mola / Oscilador Harmônico (Lei de Hooke)
// Calcula a taxa de variação simulando uma mola presa à origem (0,0,0).
// Ajuste a molaConst conforme a rigidez que desejar simular.

void derivMola(const State *state, Derivative *d) {
    // Constante de rigidez da mola (K) e massa do corpo (m)
    // Em uma arquitetura ideal, esses dados seriam lidos de dentro de 'state'
    const double molaConst = 15.0;
    const double massa = 1.0;

    // A derivada da posição é a própria velocidade atual
    d->dPosition = state->velocity;

    // Força da mola: F = -k * x (aplicado aqui tridimensionalmente em relação à origem)
    Vec3 forcaMola = (Vec3){
        -molaConst * state->position.x,
        -molaConst * state->position.y,
        -molaConst * state->position.z
    };

    // Aceleração linear: a = F / m
    d->dVelocity = (Vec3){
        forcaMola.x / massa,
        forcaMola.y / massa,
        forcaMola.z / massa
    };

    // Mantendo a parte rotacional estática para o modelo simples de mola linear
    d->dOrientation = quat_scale(quat_mul(quat_from_omega(state->angularVelocity), state->orientation), 0.5);
    d->dAngularVelocity = (Vec3){0.0, 0.0, 0.0};
}