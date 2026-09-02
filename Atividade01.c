#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define LARGURA_JANELA 800
#define ALTURA_JANELA  600
#define LINHAS_MATRIZ  20
#define COLUNAS_MATRIZ 26
#define QUANTIDADE_BOLAS 12                              

typedef struct {
    Vector2 pos;
    Vector2 vel;
    float   raio;
    Color   cor;
} Bola;

static Color paleta[] = {RED, ORANGE, YELLOW, GREEN, SKYBLUE,
                          BLUE, VIOLET, PINK, LIME, GOLD};
static const int tamanhoPaleta = sizeof(paleta) / sizeof(paleta[0]);



int **criarMatriz(int linhas, int colunas) {
    int **matriz = (int **)malloc(linhas * sizeof(int *));
    for (int i = 0; i < linhas; i++) {
        matriz[i] = (int *)malloc(colunas * sizeof(int));
        for (int j = 0; j < colunas; j++) {
            matriz[i][j] = 0;
        }
    }
    return matriz;
}

void liberarMatriz(int **matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

void desenharMatriz(int **matriz, int linhas, int colunas) {
    int larguraCelula = LARGURA_JANELA / colunas;
    int alturaCelula  = ALTURA_JANELA / linhas;

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            Color cor = (matriz[i][j] == 1) ? (Color){90, 160, 200, 255}
                                             : (Color){20, 20, 28, 255};
            DrawRectangle(j * larguraCelula, i * alturaCelula,
                           larguraCelula, alturaCelula, cor);
        }
    }

    for (int j = 0; j <= colunas; j++) {
        DrawLine(j * larguraCelula, 0, j * larguraCelula, ALTURA_JANELA,
                  (Color){50, 50, 60, 120});
    }
    for (int i = 0; i <= linhas; i++) {
        DrawLine(0, i * alturaCelula, LARGURA_JANELA, i * alturaCelula,
                  (Color){50, 50, 60, 120});
    }
}

void marcarCelulaVisitada(int **matriz, int linhas, int colunas,
                           Vector2 pos, int *contadorVisitadas) {
    int larguraCelula = LARGURA_JANELA / colunas;
    int alturaCelula  = ALTURA_JANELA / linhas;

    int coluna = (int)(pos.x / larguraCelula);
    int linha  = (int)(pos.y / alturaCelula);

    if (linha < 0 || linha >= linhas || coluna < 0 || coluna >= colunas) {
        return;
    }

    if (matriz[linha][coluna] == 0) {
        matriz[linha][coluna] = 1;
        (*contadorVisitadas)++;
    }
}

void inicializarBola(Bola *b, int indice) {
    b->pos = (Vector2){ (float)GetRandomValue(50, LARGURA_JANELA - 50),
                         (float)GetRandomValue(50, ALTURA_JANELA - 50) };

    b->vel = (Vector2){ (float)GetRandomValue(-4, 4),
                         (float)GetRandomValue(-4, 4) };
    if (b->vel.x == 0) b->vel.x = 2;
    if (b->vel.y == 0) b->vel.y = 2;

    b->raio = (float)GetRandomValue(10, 25);
    b->cor  = paleta[indice % tamanhoPaleta];
}

Bola *criarBolas(int quantidade) {
    Bola *bolas = (Bola *)malloc(quantidade * sizeof(Bola));

    for (int i = 0; i < quantidade; i++) {
        inicializarBola(bolas + i, i);
    }

    return bolas;
}

void adicionarBola(Bola **bolas, int *quantidade) {
    int novaQuantidade = *quantidade + 1;

    Bola *novoBloco = (Bola *)realloc(*bolas, novaQuantidade * sizeof(Bola));
    if (novoBloco == NULL) {
        fprintf(stderr, "Falha ao alocar memoria para nova bola.\n");
        return;
    }

    *bolas = novoBloco;
    inicializarBola(*bolas + (*quantidade), *quantidade); 
    *quantidade = novaQuantidade;
}

void removerUltimaBola(Bola **bolas, int *quantidade) {
    if (*quantidade <= 0) return; 

    int novaQuantidade = *quantidade - 1;

    if (novaQuantidade == 0) {
        free(*bolas);
        *bolas = NULL;
        *quantidade = 0;
        return;
    }

    Bola *novoBloco = (Bola *)realloc(*bolas, novaQuantidade * sizeof(Bola));
    if (novoBloco == NULL) {
        fprintf(stderr, "Falha ao realocar memoria ao remover bola.\n");
        return;
    }

    *bolas = novoBloco;
    *quantidade = novaQuantidade;
}

void atualizarBola(Bola *b) {
    b->pos.x += b->vel.x;
    b->pos.y += b->vel.y;

    if (b->pos.x - b->raio < 0 || b->pos.x + b->raio > LARGURA_JANELA)
        b->vel.x *= -1;
    if (b->pos.y - b->raio < 0 || b->pos.y + b->raio > ALTURA_JANELA)
        b->vel.y *= -1;
}

void desenharBola(const Bola *b) {
    DrawCircleV(b->pos, b->raio, b->cor);
    DrawCircleLines((int)b->pos.x, (int)b->pos.y, b->raio, BLACK);
}

int main(void) {
    InitWindow(LARGURA_JANELA, ALTURA_JANELA,
               "Atividade 1 - Ponteiros, Matrizes e Structs em C (raylib)");
    SetTargetFPS(60);

    int **matriz = criarMatriz(LINHAS_MATRIZ, COLUNAS_MATRIZ);
    Bola *bolas  = criarBolas(QUANTIDADE_BOLAS);
    int quantidadeBolas = QUANTIDADE_BOLAS;

    int celulasVisitadas = 0;
    const int totalCelulas = LINHAS_MATRIZ * COLUNAS_MATRIZ;

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_SPACE)) {
            adicionarBola(&bolas, &quantidadeBolas);
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            removerUltimaBola(&bolas, &quantidadeBolas);
        }

        for (int i = 0; i < quantidadeBolas; i++) {
            atualizarBola(bolas + i); 
            marcarCelulaVisitada(matriz, LINHAS_MATRIZ, COLUNAS_MATRIZ,
                                  bolas[i].pos, &celulasVisitadas);
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            desenharMatriz(matriz, LINHAS_MATRIZ, COLUNAS_MATRIZ);

            for (int i = 0; i < quantidadeBolas; i++) {
                desenharBola(bolas + i);
            }

            DrawText("Alocacao dinamica: matriz (int**) + vetor de structs (Bola*)",
                      10, 10, 18, RAYWHITE);
            DrawText("ESPACO: adicionar bola | BACKSPACE: remover ultima bola",
                      10, 34, 18, RAYWHITE);
            DrawText(TextFormat("Bolas: %d", quantidadeBolas),
                      10, 58, 18, RAYWHITE);
            DrawText(TextFormat("Celulas visitadas: %d / %d", celulasVisitadas, totalCelulas),
                      10, 82, 18, RAYWHITE);
            DrawFPS(LARGURA_JANELA - 90, 10);

        EndDrawing();
    }

    liberarMatriz(matriz, LINHAS_MATRIZ);
    free(bolas); 

    CloseWindow();
    return 0;
}