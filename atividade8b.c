
#include <stdlib.h>
#include "raylib.h"
#include "entidade.h"

int main(void) {
    InitWindow(800, 600, "Atividade 8b - Reuso do modulo entidade");
    SetTargetFPS(60);

    Entidade *jogador = entidadeCriar(ENTIDADE_JOGADOR, (Vector2){ 200, 300 });
    Entidade *inimigo = entidadeCriar(ENTIDADE_INIMIGO, (Vector2){ 400, 300 });
    Entidade *item    = entidadeCriar(ENTIDADE_ITEM,    (Vector2){ 600, 300 });

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        entidadeDesenhar(jogador);
        entidadeDesenhar(inimigo);
        entidadeDesenhar(item);

        DrawText("Jogador", 160, 340, 20, DARKGRAY);
        DrawText("Inimigo", 360, 340, 20, DARKGRAY);
        DrawText("Item",    575, 340, 20, DARKGRAY);

        EndDrawing();
    }

    free(jogador);
    free(inimigo);
    free(item);

    CloseWindow();
    return 0;
}