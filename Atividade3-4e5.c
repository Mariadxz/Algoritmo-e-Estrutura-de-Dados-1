#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"

#define LARGURA_JANELA     800
#define ALTURA_JANELA      600
#define QUANTIDADE_ITENS   30
#define RAIO_JOGADOR       18.0f
#define VELOCIDADE_JOGADOR 4.5f
#define VIDA_MAXIMA_INICIAL 100
#define DANO_INICIAL        5.0f

typedef struct {
    float dano;
    int   alcance;
} DadosArma;

typedef struct {
    int cura;
} DadosPocao;

typedef struct {
    int absorcao;
} DadosEscudo;

typedef union {
    DadosArma   arma;
    DadosPocao  pocao;
    DadosEscudo escudo;
} DadosItem;

typedef enum {
    ITEM_ARMA,
    ITEM_POCAO,
    ITEM_ESCUDO
} TipoItem;

typedef struct {
    Vector2   pos;
    float     raio;
    TipoItem  tipo;
    DadosItem dados;
    bool      coletado;
} Item;

typedef struct {
    Vector2 pos;
    float   raio;
    int     vida;
    int     vidaMaxima;
    float   dano;
    int     armadura;
} Jogador;

Item *criarItens(int quantidade) {
    Item *itens = (Item *)malloc(quantidade * sizeof(Item));

    for (int i = 0; i < quantidade; i++) {
        Item *it = (itens + i);
        it->pos = (Vector2){ (float)GetRandomValue(30, LARGURA_JANELA - 30),
                              (float)GetRandomValue(30, ALTURA_JANELA - 30) };
        it->raio = 12.0f;
        it->coletado = false;
        it->tipo = (TipoItem)GetRandomValue(ITEM_ARMA, ITEM_ESCUDO);

        if (it->tipo == ITEM_ARMA) {
            it->dados.arma.dano = (float)GetRandomValue(2, 8);
            it->dados.arma.alcance = GetRandomValue(1, 3);
        } else if (it->tipo == ITEM_POCAO) {
            it->dados.pocao.cura = GetRandomValue(10, 30);

            int sorteioVeneno = GetRandomValue(0, 9);
            if (sorteioVeneno < 3) {
                it->dados.pocao.cura = -it->dados.pocao.cura;
            }
        } else {
            it->dados.escudo.absorcao = GetRandomValue(5, 15);
        }
    }
    return itens;
}

int contarPocoesEnvenenadas(const Item *itens, int quantidade) {
    int total = 0;
    for (int i = 0; i < quantidade; i++) {
        if (itens[i].tipo == ITEM_POCAO && itens[i].dados.pocao.cura < 0) {
            total++;
        }
    }
    return total;
}

void aplicarItem(Jogador *j, Item *item) {
    switch (item->tipo) {
        case ITEM_ARMA:
            j->dano += item->dados.arma.dano;
            break;
        case ITEM_POCAO:
            j->vida += item->dados.pocao.cura; 
            if (j->vida > j->vidaMaxima) j->vida = j->vidaMaxima; 
            if (j->vida < 0) j->vida = 0;                        
            break;
        case ITEM_ESCUDO:
            j->armadura += item->dados.escudo.absorcao;
            break;
    }
    item->coletado = true;
}

bool tentarColetarItem(Jogador *j, Item *item) {
    if (item->coletado) return false;

    float dx = item->pos.x - j->pos.x;
    float dy = item->pos.y - j->pos.y;
    float distancia = (dx * dx + dy * dy);
    float somaRaios = (item->raio + j->raio) * (item->raio + j->raio);

    if (distancia <= somaRaios) {
        aplicarItem(j, item);
        return true;
    }
    return false;
}

void desenharItem(const Item *item) {
    if (item->coletado) return; 

    switch (item->tipo) {
        case ITEM_ARMA:
            DrawPoly(item->pos, 4, item->raio, 45.0f, (Color){230, 120, 40, 255});
            DrawPolyLines(item->pos, 4, item->raio, 45.0f, BLACK);
            break;
        case ITEM_POCAO: {
            bool envenenada = (item->dados.pocao.cura < 0);
            Color corPocao = envenenada ? (Color){140, 60, 200, 255}   
                                         : (Color){230, 60, 150, 255}; 
            DrawCircleV(item->pos, item->raio, corPocao);
            DrawCircleLines((int)item->pos.x, (int)item->pos.y, item->raio, BLACK);
            break;
        }
        case ITEM_ESCUDO:
            DrawPoly(item->pos, 5, item->raio, -90.0f, (Color){110, 150, 190, 255});
            DrawPolyLines(item->pos, 5, item->raio, -90.0f, BLACK);
            break;
    }
}

void desenharJogador(const Jogador *j) {
    DrawCircleV(j->pos, j->raio, BLUE);
    DrawCircleLines((int)j->pos.x, (int)j->pos.y, j->raio, DARKBLUE);
}

void moverJogador(Jogador *j, float velocidade) {
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) j->pos.x += velocidade;
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) j->pos.x -= velocidade;
    if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) j->pos.y += velocidade;
    if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) j->pos.y -= velocidade;

    if (j->pos.x - j->raio < 0) j->pos.x = j->raio;
    if (j->pos.x + j->raio > LARGURA_JANELA) j->pos.x = LARGURA_JANELA - j->raio;
    if (j->pos.y - j->raio < 0) j->pos.y = j->raio;
    if (j->pos.y + j->raio > ALTURA_JANELA) j->pos.y = ALTURA_JANELA - j->raio;
}

int main(void) {
    InitWindow(LARGURA_JANELA, ALTURA_JANELA,
               "Atividade 3 - Union, Enum-Tag e Struct em C (raylib)");
    SetTargetFPS(60);

    Item *itens = criarItens(QUANTIDADE_ITENS);
    int quantidadeItens = QUANTIDADE_ITENS;

    int totalPocoesEnvenenadas = contarPocoesEnvenenadas(itens, quantidadeItens);

    Jogador jogador;
    jogador.pos = (Vector2){ LARGURA_JANELA / 2.0f, ALTURA_JANELA / 2.0f };
    jogador.raio = RAIO_JOGADOR;
    jogador.vida = VIDA_MAXIMA_INICIAL;
    jogador.vidaMaxima = VIDA_MAXIMA_INICIAL;
    jogador.dano = DANO_INICIAL;
    jogador.armadura = 0;

    while (!WindowShouldClose()) {

        moverJogador(&jogador, VELOCIDADE_JOGADOR);

        for (int i = 0; i < quantidadeItens; i++) {
            tentarColetarItem(&jogador, itens + i);
        }

        BeginDrawing();
            ClearBackground((Color){24, 24, 32, 255});

            for (int i = 0; i < quantidadeItens; i++) {
                desenharItem(itens + i);
            }

            desenharJogador(&jogador);

            DrawText("Setas/WASD: mover | Armas (laranja), pocoes boas (rosa), venenosas (roxo), escudos (azul)",
                      10, 10, 18, RAYWHITE);
            DrawText(TextFormat("Vida: %d / %d", jogador.vida, jogador.vidaMaxima),
                      10, 34, 20, (Color){230, 60, 150, 255});
            DrawText(TextFormat("Dano: %.1f", jogador.dano),
                      10, 60, 20, (Color){230, 120, 40, 255});
            DrawText(TextFormat("Armadura: %d", jogador.armadura),
                      10, 86, 20, (Color){110, 150, 190, 255});
            DrawText(TextFormat("Pocoes envenenadas nesta partida: %d", totalPocoesEnvenenadas),
                      10, 112, 16, (Color){140, 60, 200, 255});

            DrawFPS(LARGURA_JANELA - 90, 10);

        EndDrawing();
    }

    free(itens);

    CloseWindow();
    return 0;
}