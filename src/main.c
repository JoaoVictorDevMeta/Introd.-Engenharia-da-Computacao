#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include "integration.h"
#include "vector3d.h"
#include "quaterniom.h"

#define TRAIL_LENGTH 300

// =================================================================
// LEI DA FISICA: Orbita gravitacional (campo central)
// =================================================================
void derivOrbita(const State *state, Derivative *d) {
    d->dPosition = state->velocity;

    Vec3 r = state->position;
    double dist = vec3_length(r);

    if (dist < 0.5) dist = 0.5;

    double GM = 800.0;
    Vec3 accel = vec3_scale(r, -GM / (dist * dist * dist));
    d->dVelocity = accel;

    d->dOrientation = quat_scale(
        quat_mul(quat_from_omega(state->angularVelocity), state->orientation),
        0.5
    );
    d->dAngularVelocity = (Vec3){0.0, 0.0, 0.0};
}

// =================================================================
// ENERGIA TOTAL (cinetica + potencial)
// =================================================================
double computeEnergy(const State *s) {
    double v2 = vec3_dot(s->velocity, s->velocity);
    double r  = vec3_length(s->position);
    if (r < 0.5) r = 0.5;
    double GM = 800.0;
    return 0.5 * v2 - GM / r;
}

// Conversor Vec3 -> Vector3 Raylib
Vector3 toRaylib(Vec3 v) {
    return (Vector3){(float)v.x, (float)v.y, (float)v.z};
}

int main(void) {
    const int screenW = 1200;
    const int screenH = 800;
    const int gridSize = 50;
    Color gridColor = Fade(GRAY, 0.3);

    InitWindow(screenW, screenH, "Phoenix Engine — Euler vs RK4");
    SetTargetFPS(60);

    // Camera 3D orbital
    Camera3D camera = {0};
    float cameraAngle = 0.0f;
    float cameraDistanceZ = 50.0f;
    
    camera.position   = (Vector3){0.0f, 25.0f, cameraDistanceZ};
    camera.target     = (Vector3){0.0f, 0.0f,  0.0f};
    camera.up         = (Vector3){0.0f, 1.0f,  0.0f};
    camera.fovy       = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Estados iniciais
    State st_euler = {0};
    State st_rk4   = {0};

    Vec3 pos0 = {18.0, 0.0, 0.0};
    Vec3 vel0 = {0.0, 0.0, 7.5};

    st_euler.position = pos0;  st_euler.velocity = vel0;
    st_euler.orientation = (Quat){1.0, 0.0, 0.0, 0.0};

    st_rk4.position = pos0;    st_rk4.velocity   = vel0;
    st_rk4.orientation = (Quat){1.0, 0.0, 0.0, 0.0};

    // Rastro (trail)
    Vector3 trail_euler[TRAIL_LENGTH] = {0};
    Vector3 trail_rk4[TRAIL_LENGTH]   = {0};
    int t_count = 0;
    int t_idx   = 0;

    int running = 1;
    
    // Configuração de Seletor e Fixed Timestep (60 Hz)
    int modoIntegracao = 0; // 0 = Ambos, 1 = Euler, 2 = RK4
    double tempoAcumulado = 0.0;
    const double dt_fixo = 1.0 / 60.0;

    while (!WindowShouldClose()) {
        // ----- INPUTS -----
        if (IsKeyPressed(KEY_SPACE)) running = !running;
        
        if (IsKeyPressed(KEY_T)) {
            st_euler.position = pos0;  st_euler.velocity = vel0;
            st_rk4.position   = pos0;  st_rk4.velocity   = vel0;
            t_count = 0;  t_idx = 0;
        }

        // Seletor de Integrador (Teclas E / R)
        if (IsKeyPressed(KEY_E)) modoIntegracao = 1; // Apenas Euler
        if (IsKeyPressed(KEY_R)) modoIntegracao = 2; // Apenas RK4
        if (IsKeyPressed(KEY_A)) modoIntegracao = 0; // Ambos

        // ----- FISICA COM FIXED TIMESTEP (60 Hz) -----
        if (running) {
            tempoAcumulado += GetFrameTime();

            while (tempoAcumulado >= dt_fixo) {
                if (modoIntegracao == 0 || modoIntegracao == 1) {
                    euler(&st_euler, dt_fixo, derivOrbita);
                }
                if (modoIntegracao == 0 || modoIntegracao == 2) {
                    rk4(&st_rk4, dt_fixo, derivOrbita);
                }
                tempoAcumulado -= dt_fixo;
            }

            trail_euler[t_idx] = toRaylib(st_euler.position);
            trail_rk4[t_idx]   = toRaylib(st_rk4.position);
            t_idx = (t_idx + 1) % TRAIL_LENGTH;
            if (t_count < TRAIL_LENGTH) t_count++;
        }

        // ----- CÂMERA -----
        Vector2 mouseDelta = GetMouseDelta();
        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            if (mouseDelta.x != 0.0f) {
                float sinal = (mouseDelta.x > 0) ? 1.0f : -1.0f;
                cameraAngle += 0.05f * sinal;
                camera.position.x = camera.target.x + cosf(cameraAngle) * cameraDistanceZ;
                camera.position.z = camera.target.z + sinf(cameraAngle) * cameraDistanceZ;
            }
        }

        // ----- RENDERIZAÇÃO -----
        BeginDrawing();
            ClearBackground((Color){5, 5, 15, 255});

            BeginMode3D(camera);
                DrawSphere((Vector3){0,0,0}, 1.8f, YELLOW);
                DrawSphereWires((Vector3){0,0,0}, 1.8f, 16, 16, ORANGE);

                for (int i = -gridSize; i <= gridSize; i += 2) {
                    DrawLine3D((Vector3){(float)i, 0, -gridSize}, (Vector3){(float)i, 0, gridSize}, gridColor);
                    DrawLine3D((Vector3){-gridSize, 0, (float)i}, (Vector3){gridSize, 0, (float)i}, gridColor);
                }

                DrawCylinderEx((Vector3){-gridSize, 0, 0}, (Vector3){gridSize, 0, 0}, 0.05f, 0.05f, 8, RED);
                DrawCylinderEx((Vector3){0, 0, -gridSize}, (Vector3){0, 0, gridSize}, 0.05f, 0.05f, 8, BLUE);

                if (modoIntegracao == 0 || modoIntegracao == 1) {
                    for (int i = 1; i < t_count; i++) {
                        int curr = (t_idx - i + TRAIL_LENGTH) % TRAIL_LENGTH;
                        int prev = (curr - 1 + TRAIL_LENGTH) % TRAIL_LENGTH;
                        DrawLine3D(trail_euler[prev], trail_euler[curr], MAROON);
                    }
                    DrawSphere(toRaylib(st_euler.position), 0.6f, RED);
                }

                if (modoIntegracao == 0 || modoIntegracao == 2) {
                    for (int i = 1; i < t_count; i++) {
                        int curr = (t_idx - i + TRAIL_LENGTH) % TRAIL_LENGTH;
                        int prev = (curr - 1 + TRAIL_LENGTH) % TRAIL_LENGTH;
                        DrawLine3D(trail_rk4[prev], trail_rk4[curr], SKYBLUE);
                    }
                    DrawSphere(toRaylib(st_rk4.position), 0.6f, BLUE);
                }

            EndMode3D();

            // HUD 2D
            DrawText("PHOENIX PHYSICS ENGINE", 10, 10, 24, GREEN);
            DrawText("[ESPACO] Pausar | [T] Reset | [E] So Euler | [R] So RK4 | [A] Ambos | FPS:", 10, 42, 16, LIGHTGRAY);
            DrawText(TextFormat("%d", GetFPS()), 680, 42, 16, GREEN);

            const char* txtModo = (modoIntegracao == 0) ? "AMBOS" : (modoIntegracao == 1) ? "EULER" : "RK4";
            DrawText(TextFormat("Metodo Ativo: %s", txtModo), 10, 65, 16, YELLOW);

            if (modoIntegracao == 0 || modoIntegracao == 1) {
                DrawText("EULER (vermelho)", 10, 95, 18, RED);
                DrawText(TextFormat("Pos: %.2f %.2f %.2f | Energia: %.4f J",
                    st_euler.position.x, st_euler.position.y, st_euler.position.z, computeEnergy(&st_euler)), 10, 120, 14, WHITE);
            }

            if (modoIntegracao == 0 || modoIntegracao == 2) {
                int posY = (modoIntegracao == 2) ? 95 : 150;
                DrawText("RK4 (azul)", 10, posY, 18, BLUE);
                DrawText(TextFormat("Pos: %.2f %.2f %.2f | Energia: %.4f J",
                    st_rk4.position.x, st_rk4.position.y, st_rk4.position.z, computeEnergy(&st_rk4)), 10, posY + 25, 14, WHITE);
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
