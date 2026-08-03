#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <string.h>

// ============================================================================
// TIPOS E ESTRUTURAS DE PRECISÃO DUPLA (DOUBLE)
// ============================================================================

typedef struct { double x, y, z; } Vector3D;

typedef struct {
    Vector3D pos;
    Vector3D vel;
} Estado3D;

typedef enum { METODO_EULER = 0, METODO_RK4 } MetodoIntegracao;
typedef enum { CENARIO_ORBITA = 0, CENARIO_PENDULO, CENARIO_QUEDA } CenarioID;

// Funções utilitárias de matemática para Vector3D
Vector3D V3D_Add(Vector3D a, Vector3D b) { return (Vector3D){ a.x + b.x, a.y + b.y, a.z + b.z }; }
Vector3D V3D_Sub(Vector3D a, Vector3D b) { return (Vector3D){ a.x - b.x, a.y - b.y, a.z - b.z }; }
Vector3D V3D_Scale(Vector3D v, double s) { return (Vector3D){ v.x * s, v.y * s, v.z * s }; }
double V3D_Dot(Vector3D a, Vector3D b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
double V3D_Length(Vector3D v) { return sqrt(V3D_Dot(v, v)); }
Vector3D V3D_Normalize(Vector3D v) { 
    double len = V3D_Length(v); 
    if (len > 0.0) return V3D_Scale(v, 1.0 / len); 
    return (Vector3D){0,0,0}; 
}
// Conversor para renderização do Raylib
Vector3 V3D_ToRaylib(Vector3D v) { return (Vector3){ (float)v.x, (float)v.y, (float)v.z }; }

// ============================================================================
// ESTADO GLOBAL DA SIMULAÇÃO
// ============================================================================

const double dt = 0.01; 
double tempoAcumulado = 0.0;
MetodoIntegracao metodoAtual = METODO_RK4;
CenarioID cenarioAtual = CENARIO_ORBITA;
Estado3D estadoFisico;

// Parâmetros Físicos Comuns
const double massaCubo = 1.0;
const Vector3D gravidade = { 0.0, -9.81, 0.0 };

// Parâmetros Específicos
// - Órbita
const double massaCentralG = 500.0; // G * M
// - Arrasto
const double kArrasto = 0.15;
// - Pêndulo
Vector3D penduloAncora = { 0.0, 15.0, 0.0 };
double penduloComprimento = 10.0;

// Sistema de Rastro (Trail)
#define MAX_TRAIL 200
Vector3 trail[MAX_TRAIL];
int trailCount = 0;
int trailTimer = 0;

// ============================================================================
// CÁLCULO DE ACELERAÇÃO CENTRALIZADO
// ============================================================================

Vector3D CalcularAceleracao(Vector3D pos, Vector3D vel, CenarioID cenario)
{
    Vector3D acel = { 0, 0, 0 };

    switch (cenario)
    {
        case CENARIO_ORBITA: {
            // Lei da Gravitação: a = - (G*M / r^2) * normalize(r)
            double distSq = V3D_Dot(pos, pos);
            double dist = sqrt(distSq);
            if (dist > 0.1) {
                Vector3D direcao = V3D_Scale(pos, -1.0 / dist);
                double magnitude = massaCentralG / distSq;
                acel = V3D_Scale(direcao, magnitude);
            }
        } break;

        case CENARIO_PENDULO: {
            // Pêndulo Rígido 3D via Forças de Restrição
            Vector3D p = V3D_Sub(pos, penduloAncora);
            double dist = V3D_Length(p);
            
            if (dist > 0.001) {
                Vector3D dir = V3D_Scale(p, 1.0 / dist);
                double v2 = V3D_Dot(vel, vel);
                
                // Força de Tensão que cancela a gravidade radial e fornece força centrípeta
                Vector3D tracaoGrav = V3D_Scale(dir, -V3D_Dot(gravidade, dir));
                Vector3D tracaoCentripeta = V3D_Scale(dir, -v2 / dist);
                
                Vector3D forcaTotal = V3D_Add(gravidade, V3D_Add(tracaoGrav, tracaoCentripeta));
                acel = forcaTotal;
            }
        } break;

        case CENARIO_QUEDA: {
            // Queda Livre + Arrasto: a = g - (k/m)*|v|*v
            double moduloVel = V3D_Length(vel);
            Vector3D a_arrasto = {0,0,0};
            if (moduloVel > 0.001) {
                a_arrasto = V3D_Scale(vel, -(kArrasto / massaCubo) * moduloVel);
            }
            acel = V3D_Add(gravidade, a_arrasto);
        } break;
    }
    return acel;
}

// ============================================================================
// SETUPS (ResetSimulation e Cenários)
// ============================================================================

void ClearTrails() {
    trailCount = 0;
    trailTimer = 0;
}

void SetupOrbit() {
    // Coloca a 10m de distância, com velocidade tangencial perfeita para órbita circular
    estadoFisico.pos = (Vector3D){ 10.0, 0.0, 0.0 };
    double vTangencial = sqrt(massaCentralG / 10.0);
    estadoFisico.vel = (Vector3D){ 0.0, 0.0, -vTangencial };
}

void SetupPendulum() {
    // Solta o pêndulo a 90 graus (horizontal)
    estadoFisico.pos = (Vector3D){ penduloComprimento, penduloAncora.y, 0.0 };
    estadoFisico.vel = (Vector3D){ 0.0, 0.0, 0.0 };
}

void SetupFreeFall() {
    estadoFisico.pos = (Vector3D){ 0.0, 30.0, 0.0 };
    estadoFisico.vel = (Vector3D){ 0.0, 0.0, 0.0 };
}

void ResetSimulation() {
    ClearTrails();
    tempoAcumulado = 0.0;
    
    switch(cenarioAtual) {
        case CENARIO_ORBITA: SetupOrbit(); break;
        case CENARIO_PENDULO: SetupPendulum(); break;
        case CENARIO_QUEDA: SetupFreeFall(); break;
    }
}

// ============================================================================
// LÓGICA PRINCIPAL E RENDERIZAÇÃO
// ============================================================================

double tempoSimulacao = 0.0;

int main(void)
{
    InitWindow(1000, 720, "Teste Acido - Integradores Fisicos");
    SetTargetFPS(60);

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 20.0f, 20.0f, 30.0f };
    camera.target = (Vector3){ 0.0f, 5.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };         
    camera.fovy = 45.0f;                                
    camera.projection = CAMERA_PERSPECTIVE;            

    float anguloHorizontal = 0.6f;
    float anguloVertical = 0.4f;
    float raioCamera = 35.0f;

    ResetSimulation();
    tempoSimulacao = 0.0; // Zera o cronômetro no início

    while (!WindowShouldClose())
    {
        // 1. CHAVEAMENTO DINÂMICO (Teclas E e R)
        if (IsKeyPressed(KEY_E) && metodoAtual != METODO_EULER) {
            metodoAtual = METODO_EULER;
            ResetSimulation();
            tempoSimulacao = 0.0;
        }
        if (IsKeyPressed(KEY_R) && metodoAtual != METODO_RK4) {
            metodoAtual = METODO_RK4;
            ResetSimulation();
            tempoSimulacao = 0.0;
        }
        
        // Troca de cenários
        if (IsKeyPressed(KEY_ENTER)) {
            cenarioAtual = (cenarioAtual + 1) % 3;
            ResetSimulation();
            tempoSimulacao = 0.0;
        }

        // 2. LOOP DE TEMPO FIXO (Fixed Timestep)
        double frameTime = GetFrameTime();
        if (frameTime > 0.25) frameTime = 0.25; 
        tempoAcumulado += frameTime;

        while (tempoAcumulado >= dt)
        {
            if (metodoAtual == METODO_EULER) {
                Vector3D a = CalcularAceleracao(estadoFisico.pos, estadoFisico.vel, cenarioAtual);
                estadoFisico.vel = V3D_Add(estadoFisico.vel, V3D_Scale(a, dt));
                estadoFisico.pos = V3D_Add(estadoFisico.pos, V3D_Scale(estadoFisico.vel, dt));
            } else {
                // ... (Mantenha o bloco do RK4 igual ao código anterior)
                Vector3D p0 = estadoFisico.pos;
                Vector3D v0 = estadoFisico.vel;
                
                Vector3D a1 = CalcularAceleracao(p0, v0, cenarioAtual);
                Vector3D dp1 = V3D_Scale(v0, dt);
                Vector3D dv1 = V3D_Scale(a1, dt);

                Vector3D p2 = V3D_Add(p0, V3D_Scale(dp1, 0.5));
                Vector3D v2 = V3D_Add(v0, V3D_Scale(dv1, 0.5));
                Vector3D a2 = CalcularAceleracao(p2, v2, cenarioAtual);
                Vector3D dp2 = V3D_Scale(v2, dt);
                Vector3D dv2 = V3D_Scale(a2, dt);

                Vector3D p3 = V3D_Add(p0, V3D_Scale(dp2, 0.5));
                Vector3D v3 = V3D_Add(v0, V3D_Scale(dv2, 0.5));
                Vector3D a3 = CalcularAceleracao(p3, v3, cenarioAtual);
                Vector3D dp3 = V3D_Scale(v3, dt);
                Vector3D dv3 = V3D_Scale(a3, dt);

                Vector3D p4 = V3D_Add(p0, dp3);
                Vector3D v4 = V3D_Add(v0, dv3);
                Vector3D a4 = CalcularAceleracao(p4, v4, cenarioAtual);
                Vector3D dp4 = V3D_Scale(v4, dt);
                Vector3D dv4 = V3D_Scale(a4, dt);

                Vector3D somaP = V3D_Add(dp1, V3D_Add(V3D_Scale(dp2, 2.0), V3D_Add(V3D_Scale(dp3, 2.0), dp4)));
                Vector3D somaV = V3D_Add(dv1, V3D_Add(V3D_Scale(dv2, 2.0), V3D_Add(V3D_Scale(dv3, 2.0), dv4)));

                estadoFisico.pos = V3D_Add(estadoFisico.pos, V3D_Scale(somaP, 1.0/6.0));
                estadoFisico.vel = V3D_Add(estadoFisico.vel, V3D_Scale(somaV, 1.0/6.0));
            }
            
            // Incrementa o tempo real de simulação da física
            tempoSimulacao += dt;

            // Rastro (Trail)
            trailTimer++;
            if (trailTimer >= 5) {
                trailTimer = 0;
                if (trailCount < MAX_TRAIL) {
                    trail[trailCount] = V3D_ToRaylib(estadoFisico.pos);
                    trailCount++;
                } else {
                    memmove(trail, trail + 1, (MAX_TRAIL - 1) * sizeof(Vector3));
                    trail[MAX_TRAIL - 1] = V3D_ToRaylib(estadoFisico.pos);
                }
            }
            tempoAcumulado -= dt;
        }

        // Câmera Orbital (mantenha igual)
        raioCamera -= GetMouseWheelMove() * 2.0f;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 deltaMouse = GetMouseDelta();
            anguloHorizontal -= deltaMouse.x * 0.005f;
            anguloVertical += deltaMouse.y * 0.005f;
            if (anguloVertical > 1.5f) anguloVertical = 1.5f;
            if (anguloVertical < -1.5f) anguloVertical = -1.5f;
        }
        if (cenarioAtual == CENARIO_ORBITA) camera.target = (Vector3){ 0, 0, 0 };
        if (cenarioAtual == CENARIO_PENDULO) camera.target = (Vector3){ 0, 5, 0 };
        if (cenarioAtual == CENARIO_QUEDA) camera.target = (Vector3){ 0, 10, 0 };
        camera.position.x = camera.target.x + raioCamera * cosf(anguloVertical) * sinf(anguloHorizontal);
        camera.position.y = camera.target.y + raioCamera * sinf(anguloVertical);
        camera.position.z = camera.target.z + raioCamera * cosf(anguloVertical) * cosf(anguloHorizontal);

        // Cálculos de Energia
        double vEscalar = V3D_Length(estadoFisico.vel);
        double energiaCineticaLinear = 0.5 * massaCubo * (vEscalar * vEscalar);
        // FIXME: Implementar energia rotacional no próximo sprint
        double energiaCineticaRotacional = 0.0; 
        double energiaCineticaTotal = energiaCineticaLinear + energiaCineticaRotacional;

        // Renderização
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                
                // Desenha rastros
                for (int i = 0; i < trailCount - 1; i++) {
                    DrawLine3D(trail[i], trail[i+1], Fade(RED, (float)i / MAX_TRAIL));
                }

                // Desenha cenário e cubo
                if (cenarioAtual == CENARIO_ORBITA) DrawSphere((Vector3){0,0,0}, 2.0f, ORANGE);
                else if (cenarioAtual == CENARIO_PENDULO) {
                    DrawSphere(V3D_ToRaylib(penduloAncora), 0.5f, DARKGRAY);
                    DrawLine3D(V3D_ToRaylib(penduloAncora), V3D_ToRaylib(estadoFisico.pos), GRAY);
                }
                Vector3 posCube = V3D_ToRaylib(estadoFisico.pos);
                DrawCube(posCube, 1.2f, 1.2f, 1.2f, BLUE);
                DrawCubeWires(posCube, 1.2f, 1.2f, 1.2f, BLACK);
                
                DrawGrid(20, 2.0f);
            EndMode3D();

            // 4. INTERFACE DE MÉTRICAS (HUD)
            DrawRectangle(10, 10, 480, 200, Fade(BLACK, 0.8f));
            DrawRectangleLines(10, 10, 480, 200, GREEN);

            const char* nomeMetodo = (metodoAtual == METODO_EULER) ? "EULER" : "RK4";
            Color corMetodo = (metodoAtual == METODO_EULER) ? RED : LIME;
            DrawText(TextFormat("INTEGRADOR: %s", nomeMetodo), 20, 20, 22, corMetodo);
            
            DrawText(TextFormat("FPS: %i | Tempo: %.1fs", GetFPS(), tempoSimulacao), 20, 50, 18, LIME);
            
            DrawText("ENERGIA CINÉTICA", 20, 90, 16, GRAY);
            DrawText(TextFormat("Linear:      %.2f J", energiaCineticaLinear), 20, 110, 18, WHITE);
            DrawText(TextFormat("Rotacional:  %.2f J (Pendente)", energiaCineticaRotacional), 20, 130, 18, DARKGRAY);
            DrawText(TextFormat("Total:       %.2f J", energiaCineticaTotal), 20, 150, 18, YELLOW);

            DrawText("Controles: [E] Euler | [R] RK4 | [ENTER] Cenario", 20, 180, 14, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
