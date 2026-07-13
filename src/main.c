#include "raylib.h"

// =================================================================
// CODIGO PARA TESTE DE RAYLIB 3D COM FISICA SIMPLES DE BOLA
// =================================================================

int main(void) {
    // Mantém o desvio de segurança do diretório
    ChangeDirectory("C:/"); 

    const int screenWidth = 800;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "Phoenix Physics Engine - 3D BALL CODES");
    SetTargetFPS(60);
    
    // --- CONFIGURAÇÃO DA CÂMERA 3D (MÉTODO SEGURO) ---
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 8.0f, 15.0f }; // Posicionada um pouco mais alta e distante
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    // --- PROPRIEDADES DA BOLA (FÍSICA) ---
    Vector3 ballPosition = { 0.0f, 5.0f, 0.0f };
    Vector3 ballVelocity = { 0.0f, 0.0f, 0.0f };
    float ballRadius = 0.4f;
    
    // Variáveis do ambiente físico
    float gravity = -0.015f;      // Força constante para baixo (eixo Y)
    float bounce = 0.75f;        // Elasticidade (perde 25% da força ao colidir)
    float airResistance = 0.99f; // Atrito do ar para desacelerar a bola gradualmente
    float pushForce = 0.01f;     // Força aplicada ao pressionar as teclas de movimento

    // --- PROPRIEDADES DA PLATAFORMA ---
    Vector3 platformPosition = { 0.0f, -0.1f, 0.0f };
    Vector3 platformSize = { 12.0f, 0.2f, 12.0f }; // Uma grande mesa quadrada 3D

    while (!WindowShouldClose()) {
        
        // 1. CONTROLE DA CÂMERA (Setas do Teclado)
        Vector3 camMove = { 0 };
        Vector3 camRot = { 0 };
        
        if (IsKeyDown(KEY_RIGHT)) camRot.x = 1.5f;
        if (IsKeyDown(KEY_LEFT))  camRot.x = -1.5f;
        if (IsKeyDown(KEY_UP))    camRot.y = -1.5f;
        if (IsKeyDown(KEY_DOWN))  camRot.y = 1.5f;
        
        UpdateCameraPro(&camera, camMove, camRot, 0.0f);

        // 2. CONTROLE DA BOLA (Teclas W, A, S, D)
        // Aplica impulsos nos eixos X (esquerda/direita) e Z (frente/trás)
        if (IsKeyDown(KEY_D)) ballVelocity.x += pushForce;
        if (IsKeyDown(KEY_A)) ballVelocity.x -= pushForce;
        if (IsKeyDown(KEY_W)) ballVelocity.z -= pushForce;
        if (IsKeyDown(KEY_S)) ballVelocity.z += pushForce;
        if (IsKeyPressed(KEY_SPACE) && ballPosition.y <= (platformPosition.y + platformSize.y/2 + ballRadius + 0.05f)) {
            ballVelocity.y = 0.4f; // Pulo! Só funciona se estiver perto do chão
        }

        // 3. PROCESSAMENTO DA FÍSICA
        ballVelocity.y += gravity;             // Aplica gravidade no eixo vertical
        ballVelocity.x *= airResistance;       // Amortece velocidade no eixo X
        ballVelocity.z *= airResistance;       // Amortece velocidade no eixo Z
        
        // Atualiza a posição 3D baseada nas velocidades
        ballPosition.x += ballVelocity.x;
        ballPosition.y += ballVelocity.y;
        ballPosition.z += ballVelocity.z;

        // 4. DETECÇÃO DE COLISÃO (Bola vs Plataforma)
        float halfWidth = platformSize.x / 2.0f;
        float halfDepth = platformSize.z / 2.0f;
        float platformTop = platformPosition.y + (platformSize.y / 2.0f);

        // Verifica se a bola está dentro dos limites horizontais da mesa/plataforma
        if ((ballPosition.x >= -halfWidth && ballPosition.x <= halfWidth) &&
            (ballPosition.z >= -halfDepth && ballPosition.z <= halfDepth)) {
            
            // Verifica se a base da bola cruzou a superfície superior da mesa
            if (ballPosition.y - ballRadius <= platformTop) {
                ballPosition.y = platformTop + ballRadius; // Impede a bola de afundar
                ballVelocity.y = -ballVelocity.y * bounce; // Inverte o eixo Y aplicando o quique
            }
        }

        // Sistema de segurança: Se a bola cair para fora da plataforma, ela ressurge no alto
        if (ballPosition.y < -10.0f) {
            ballPosition = (Vector3){ 0.0f, 6.0f, 0.0f };
            ballVelocity = (Vector3){ 0.0f, 0.0f, 0.0f };
        }

        // 5. RENDERIZAÇÃO
        BeginDrawing();
            ClearBackground(BLACK);
            
            BeginMode3D(camera);
                
                // Linhas de grade para dar noção de profundidade e espaço no cenário
                DrawGrid(20, 1.0f);
                
                // Desenha a mesa de testes (Plataforma Cinza)
                DrawCubeV(platformPosition, platformSize, GRAY);
                DrawCubeWiresV(platformPosition, platformSize, DARKGRAY);
                
                // Desenha a Bola Física (Esfera Vermelha com linhas guias)
                DrawSphere(ballPosition, ballRadius, RED);
                DrawSphereWires(ballPosition, ballRadius, 10, 10, MAROON);
                
            EndMode3D();
            
            // Interface Textual 2D por cima do 3D
            DrawText("ENGINE DE FISICA 3D - ONLINE", 10, 10, 20, GREEN);
            DrawText("Controles da Bola: W, A, S, D [ESPACO para pular]", 10, 40, 16, LIGHTGRAY);
            DrawText("Controles da Camera: Setas do Teclado", 10, 60, 16, LIGHTGRAY);
            
            // Telemetria da Física
            DrawText(TextFormat("Posicao Y: %.2f", ballPosition.y), 10, 95, 14, WHITE);
            DrawText(TextFormat("Velocidade X: %.3f", ballVelocity.x), 10, 115, 14, BLUE);
            DrawText(TextFormat("Velocidade Z: %.3f", ballVelocity.z), 10, 135, 14, ORANGE);
            
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}