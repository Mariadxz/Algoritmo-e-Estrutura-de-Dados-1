#include "raylib.h"
#include "entidade.h"
#include <stdlib.h>
#include <time.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA  600
#define TOTAL_ENTIDADES 3

int main(void) {
    srand((unsigned int)time(NULL));

    InitWindow(LARGURA_JANELA, ALTURA_JANELA, "Atividade 8b - Reuso do modulo");
    SetTargetFPS(60);

    Entidade *entidades[TOTAL_ENTIDADES];

    entidades[0] = entidadeCriar(ENTIDADE_JOGADOR, (Vector2){ 200, 300 });
    entidades[1] = entidadeCriar(ENTIDADE_INIMIGO, (Vector2){ 400, 300 });
    entidades[2] = entidadeCriar(ENTIDADE_ITEM, (Vector2){ 600, 300 });

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);

            for (int i = 0; i < TOTAL_ENTIDADES; i++) {
                if (entidades[i] != NULL) entidadeDesenhar(entidades[i]);
            }

            DrawText("Jogador", 165, 350, 20, DARKGRAY);
            DrawText("Inimigo", 365, 350, 20, DARKGRAY);
            DrawText("Item", 580, 350, 20, DARKGRAY);
            DrawText("ESC sai", 10, ALTURA_JANELA - 25, 16, GRAY);
        EndDrawing();
    }

    for (int i = 0; i < TOTAL_ENTIDADES; i++) free(entidades[i]);

    CloseWindow();
    return 0;
}
