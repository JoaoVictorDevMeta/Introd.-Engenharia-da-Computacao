#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include "integration.h"
#include "vector3d.h"
#include "quaterniom.h"

#define TRAIL_LENGTH 300
#define DT_PHYSICS   0.08   // Passo GRANDE propositalmente pra evidenciar erro do Euler

// =================================================================
// LEI DA FISICA: Orbita gravitacional (campo central)
// F = -GM/r^2, apontando pro centro (0,0,0)
// =================================================================
void derivOrbita(const State *state, Derivative *d) {
    // Taxa de variacao da posicao = velocidade
    d->dPosition = state->velocity;

    // Vetor posicao em relacao ao centro
    Vec3 r = state->position;
    double dist = vec3_length(r);

    // Evita divisao por zero se passar muito perto
    if (dist < 0.5) dist = 0.5;

    // Aceleracao: a = -GM/r^3 * r  (forca sempre puxa pro centro)
    double GM = 800.0;
    Vec3 accel = vec3_scale(r, -GM / (dist * dist * dist));
    d->dVelocity = accel;

    // Sem rotacao por enquanto (particula pontual)
    d->dOrientation = quat_scale(
        quat_mul(quat_from_omega(state->angularVelocity), state->orientation),
        0.5
    );
    d->dAngularVelocity = (Vec3){0.0, 0.0, 0.0};
}

// =================================================================
// ENERGIA TOTAL (cinetica + potencial) — usada pra medir estabilidade
// E = 0.5*v^2 - GM/r   (massa = 1)
// Se o integrador for bom, essa energia deve ficar CONSTANTE.
// =================================================================
double computeEnergy(const State *s) {
    double v2 = vec3_dot(s->velocity, s->velocity);
    double r  = vec3_length(s->position);
    if (r < 0.5) r = 0.5;
    double GM = 800.0;
    return 0.5 * v2 - GM / r;
}

// =================================================================
// CONVERSOR: nosso Vec3 (engine) -> Vector3 (Raylib pra desenhar)
// =================================================================
Vector3 toRaylib(Vec3 v) {
    return (Vector3){(float)v.x, (float)v.y, (float)v.z};
}

// =================================================================
// MAIN
// =================================================================
int main(void) {
    const int screenW = 1200;
    const int screenH = 800;
    InitWindow(screenW, screenH, "Phoenix Engine — Euler vs RK4");
    SetTargetFPS(60);

    // --- Camera 3D orbital ---
    Camera3D camera = {0};
    camera.position   = (Vector3){0.0f, 25.0f, 50.0f};
    camera.target     = (Vector3){0.0f, 0.0f,  0.0f};
    camera.up         = (Vector3){0.0f, 1.0f,  0.0f};
    camera.fovy       = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // --- Estado inicial (IDENTICO pros dois integradores) ---
    State st_euler = {0};
    State st_rk4   = {0};

    Vec3 pos0 = {18.0, 0.0, 0.0};   // Comeca longe do centro
    Vec3 vel0 = {0.0, 0.0, 7.5};    // Velocidade tangencial -> orbita!

    st_euler.position = pos0;  st_euler.velocity = vel0;
    st_euler.orientation = (Quat){1.0, 0.0, 0.0, 0.0};

    st_rk4.position = pos0;  st_rk4.velocity = vel0;
    st_rk4.orientation = (Quat){1.0, 0.0, 0.0, 0.0};

    // --- Rastro (trail) circular ---
    Vector3 trail_euler[TRAIL_LENGTH] = {0};
    Vector3 trail_rk4[TRAIL_LENGTH]   = {0};
    int t_count = 0;
    int t_idx   = 0;

    int running = 1;   // Pausa com espaco

    while (!WindowShouldClose()) {
        // ----- INPUT -----
        if (IsKeyPressed(KEY_SPACE)) running = !running;
        if (IsKeyPressed(KEY_R)) {
            // Reinicia ambos pro estado inicial
            st_euler.position = pos0;  st_euler.velocity = vel0;
            st_rk4.position   = pos0;  st_rk4.velocity   = vel0;
            t_count = 0;  t_idx = 0;
        }

        // ----- FISICA (passo fixo) -----
        if (running) {
            // Varios sub-passos por frame pra suavidade visual
            int sub = 4;
            double dt_step = DT_PHYSICS / sub;

            for (int i = 0; i < sub; i++) {
                euler(&st_euler, dt_step, derivOrbita);
                rk4(&st_rk4, dt_step, derivOrbita);
            }

            // Guarda posicao no rastro (buffer circular)
            trail_euler[t_idx] = toRaylib(st_euler.position);
            trail_rk4[t_idx]   = toRaylib(st_rk4.position);
            t_idx = (t_idx + 1) % TRAIL_LENGTH;
            if (t_count < TRAIL_LENGTH) t_count++;
        }

        // ----- CAMERA -----
        UpdateCamera(&camera, CAMERA_ORBITAL);

        // ----- RENDER -----
        BeginDrawing();
            ClearBackground((Color){5, 5, 15, 255});  // Azul-muito-escuro

            BeginMode3D(camera);
                // Sol no centro
                DrawSphere((Vector3){0,0,0}, 1.8f, YELLOW);
                DrawSphereWires((Vector3){0,0,0}, 1.8f, 16, 16, ORANGE);

                // Grade de referencia
                DrawGrid(50, 2.0f);

                // Rastro EULER (vermelho, mais grosso)
                for (int i = 1; i < t_count; i++) {
                    int curr = (t_idx - i + TRAIL_LENGTH) % TRAIL_LENGTH;
                    int prev = (curr - 1 + TRAIL_LENGTH) % TRAIL_LENGTH;
                    DrawLine3D(trail_euler[prev], trail_euler[curr], MAROON);
                }

                // Rastro RK4 (azul/ciano)
                for (int i = 1; i < t_count; i++) {
                    int curr = (t_idx - i + TRAIL_LENGTH) % TRAIL_LENGTH;
                    int prev = (curr - 1 + TRAIL_LENGTH) % TRAIL_LENGTH;
                    DrawLine3D(trail_rk4[prev], trail_rk4[curr], SKYBLUE);
                }

                // Esferas atuais
                DrawSphere(toRaylib(st_euler.position), 0.6f, RED);
                DrawSphere(toRaylib(st_rk4.position),   0.6f, BLUE);

            EndMode3D();

            // ----- UI 2D (texto por cima) -----
            DrawText("PHOENIX PHYSICS ENGINE", 10, 10, 24, GREEN);
            DrawText("[ESPACO] Pausar    [R] Reiniciar    Mouse: orbitar camera", 10, 42, 16, LIGHTGRAY);

            // Painel Euler
            DrawText("EULER (vermelho)", 10, 80, 18, RED);
            DrawText(TextFormat("Posicao:  %.2f  %.2f  %.2f",
                st_euler.position.x, st_euler.position.y, st_euler.position.z), 10, 105, 14, WHITE);
            DrawText(TextFormat("Energia:  %.4f  J", computeEnergy(&st_euler)), 10, 125, 14, WHITE);

            // Painel RK4
            DrawText("RK4 (azul)", 10, 160, 18, BLUE);
            DrawText(TextFormat("Posicao:  %.2f  %.2f  %.2f",
                st_rk4.position.x, st_rk4.position.y, st_rk4.position.z), 10, 185, 14, WHITE);
            DrawText(TextFormat("Energia:  %.4f  J", computeEnergy(&st_rk4)), 10, 205, 14, WHITE);

            // Legenda explicativa
            DrawText("Observacao: com passo grande (dt=0.08),", 10, 250, 14, GRAY);
            DrawText("o Euler ganha energia e espirala para fora.", 10, 268, 14, GRAY);
            DrawText("O RK4 conserva a orbita eliptica.", 10, 286, 14, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
