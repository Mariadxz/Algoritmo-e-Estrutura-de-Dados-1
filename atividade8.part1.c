#ifndef ENTIDADE_H
#define ENTIDADE_H

#include <stdbool.h>
#include "raylib.h"

typedef enum {
    ENTIDADE_JOGADOR,
    ENTIDADE_INIMIGO,
    ENTIDADE_ITEM
} TipoEntidade;

typedef union {
    int dano;   
    int valor;  
} ExtraEntidade;

typedef struct {
    TipoEntidade  tipo;
    Vector2       pos;
    float         raio;
    int           vida;
    Color         cor;
    ExtraEntidade extra;
} Entidade;

Entidade *entidadeCriar(TipoEntidade tipo, Vector2 pos);
bool      entidadeColidiu(Entidade *a, Entidade *b);
void      entidadeDesenhar(Entidade *e);
void      entidadeAplicarDano(Entidade *e, int dano);
bool      entidadeEstaViva(Entidade *e);   

#endif 

#include <stdlib.h>
#include "entidade.h"

Entidade *entidadeCriar(TipoEntidade tipo, Vector2 pos) {
    Entidade *e = (Entidade *) malloc(sizeof(Entidade));
    if (e == NULL) return NULL;

    e->tipo = tipo;
    e->pos  = pos;

    switch (tipo) {
        case ENTIDADE_JOGADOR:
            e->raio       = 15.0f;
            e->vida       = 100;
            e->cor        = BLUE;
            e->extra.dano = 0;
            break;
        case ENTIDADE_INIMIGO:
            e->raio       = 20.0f;
            e->vida       = 30;
            e->cor        = RED;
            e->extra.dano = 10;
            break;
        case ENTIDADE_ITEM:
            e->raio        = 10.0f;
            e->vida        = 1;
            e->cor         = GREEN;
            e->extra.valor = 10;
            break;
    }
    return e;
}

bool entidadeColidiu(Entidade *a, Entidade *b) {
    if (a == NULL || b == NULL) return false;
    return CheckCollisionCircles(a->pos, a->raio, b->pos, b->raio);
}

void entidadeDesenhar(Entidade *e) {
    if (e == NULL) return;
    DrawCircleV(e->pos, e->raio, e->cor);
    DrawCircleLines((int) e->pos.x, (int) e->pos.y, e->raio, BLACK);
}

void entidadeAplicarDano(Entidade *e, int dano) {
    if (e == NULL) return;

    e->vida -= dano;
    if (e->vida < 0) e->vida = 0;
}

bool entidadeEstaViva(Entidade *e) {
    if (e == NULL) return false;
    return e->vida > 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "entidade.h"

#define LARGURA        800
#define ALTURA         600
#define MAX_ENTIDADES  64

static Entidade *vetorEntidades[MAX_ENTIDADES];
static int       qtdEntidades = 0;

static bool adicionarEntidade(Entidade *e) {
    if (e == NULL) return false;
    if (qtdEntidades >= MAX_ENTIDADES) {
        free(e);
        return false;
    }
    vetorEntidades[qtdEntidades++] = e;
    return true;
}

static void removerEntidade(int i) {
    if (i < 0 || i >= qtdEntidades) return;
    free(vetorEntidades[i]);
    vetorEntidades[i] = vetorEntidades[qtdEntidades - 1];
    vetorEntidades[qtdEntidades - 1] = NULL;
    qtdEntidades--;
}

static void liberarTudo(void) {
    for (int i = 0; i < qtdEntidades; i++) {
        free(vetorEntidades[i]);
        vetorEntidades[i] = NULL;
    }
    qtdEntidades = 0;
}

static Vector2 posicaoNaBorda(void) {
    switch (GetRandomValue(0, 3)) {
        case 0:  return (Vector2){ (float) GetRandomValue(0, LARGURA), 0 };
        case 1:  return (Vector2){ (float) GetRandomValue(0, LARGURA), ALTURA };
        case 2:  return (Vector2){ 0, (float) GetRandomValue(0, ALTURA) };
        default: return (Vector2){ LARGURA, (float) GetRandomValue(0, ALTURA) };
    }
}

int main(void) {
    InitWindow(LARGURA, ALTURA, "Atividade 8 - Modulos em C");
    SetTargetFPS(60);

    Vector2 posInicial = { LARGURA / 2.0f, ALTURA / 2.0f };
    Entidade *jogador = entidadeCriar(ENTIDADE_JOGADOR, posInicial);
    adicionarEntidade(jogador);   

    int   pontos     = 0;
    float tempoSpawn = 0.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (entidadeEstaViva(jogador)) {
            float vel = 250.0f * dt;
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) jogador->pos.x += vel;
            if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) jogador->pos.x -= vel;
            if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) jogador->pos.y += vel;
            if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) jogador->pos.y -= vel;

            if (jogador->pos.x < jogador->raio) jogador->pos.x = jogador->raio;
            if (jogador->pos.y < jogador->raio) jogador->pos.y = jogador->raio;
            if (jogador->pos.x > LARGURA - jogador->raio) jogador->pos.x = LARGURA - jogador->raio;
            if (jogador->pos.y > ALTURA  - jogador->raio) jogador->pos.y = ALTURA  - jogador->raio;

            tempoSpawn += dt;
            if (tempoSpawn >= 1.0f) {
                tempoSpawn = 0.0f;
                TipoEntidade tipo = (GetRandomValue(0, 99) < 65) ? ENTIDADE_INIMIGO
                                                                  : ENTIDADE_ITEM;
                Entidade *novo = entidadeCriar(tipo, posicaoNaBorda());
                if (novo != NULL && tipo == ENTIDADE_ITEM) {
                    novo->pos = (Vector2){ (float) GetRandomValue(30, LARGURA - 30),
                                           (float) GetRandomValue(30, ALTURA  - 30) };
                }
                adicionarEntidade(novo);
            }

            for (int i = 1; i < qtdEntidades; i++) {
                Entidade *e = vetorEntidades[i];

                if (e->tipo == ENTIDADE_INIMIGO) {
                    float dx = jogador->pos.x - e->pos.x;
                    float dy = jogador->pos.y - e->pos.y;
                    float d  = sqrtf(dx * dx + dy * dy);
                    if (d > 0.001f) {
                        e->pos.x += (dx / d) * 70.0f * dt;
                        e->pos.y += (dy / d) * 70.0f * dt;
                    }
                }

                if (entidadeColidiu(jogador, e)) {
                    if (e->tipo == ENTIDADE_INIMIGO) {
                        entidadeAplicarDano(jogador, e->extra.dano);
                        entidadeAplicarDano(e, e->vida);  
                    } else if (e->tipo == ENTIDADE_ITEM) {
                        pontos += e->extra.valor;
                        entidadeAplicarDano(e, e->vida);   
                    }
                }
            }

            for (int i = qtdEntidades - 1; i >= 1; i--) {
                if (!entidadeEstaViva(vetorEntidades[i])) {
                    removerEntidade(i);
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < qtdEntidades; i++) {
            entidadeDesenhar(vetorEntidades[i]);
        }

        DrawText(TextFormat("Vida: %d", jogador->vida), 10, 10, 20, DARKGRAY);
        DrawText(TextFormat("Pontos: %d", pontos),      10, 35, 20, DARKGRAY);

        if (!entidadeEstaViva(jogador)) {
            DrawText("GAME OVER", LARGURA / 2 - 110, ALTURA / 2 - 20, 40, RED);
        }
        EndDrawing();
    }

    liberarTudo();
    CloseWindow();
    return 0;
}