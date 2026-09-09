#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"

#define LARGURA_JANELA   800
#define ALTURA_JANELA    600
#define QUANTIDADE_MOEDAS 15
#define RAIO_JOGADOR     18.0f
#define VELOCIDADE_JOGADOR 4.5f


typedef enum {
    MOEDA_BRONZE,
    MOEDA_PRATA,
    MOEDA_OURO,
    MOEDA_DIAMANTE
} TipoMoeda;

typedef struct {
    Vector2   pos;
    float     raio;
    TipoMoeda tipo;
    int       valor;
    bool      coletada;
    float     tempoColeta;
} Moeda;

Color corDaMoeda(TipoMoeda tipo) {
    switch (tipo) {
        case MOEDA_BRONZE:   return (Color){160, 90, 40, 255};
        case MOEDA_PRATA:    return (Color){190, 190, 190, 255};
        case MOEDA_OURO:     return GOLD;
        case MOEDA_DIAMANTE: return (Color){130, 220, 255, 255}; 
        default:             return WHITE;
    }
}

int valorDaMoeda(TipoMoeda tipo) {
    switch (tipo) {
        case MOEDA_BRONZE:   return 5;
        case MOEDA_PRATA:    return 10;
        case MOEDA_OURO:     return 25;
        case MOEDA_DIAMANTE: return 50;
        default:             return 0;
    }
}


Moeda *criarMoedas(int quantidade) {
    Moeda *moedas = (Moeda *)malloc(quantidade * sizeof(Moeda));

    for (int i = 0; i < quantidade; i++) {
        Moeda *m = (moedas + i);
        m->pos = (Vector2){ (float)GetRandomValue(30, LARGURA_JANELA - 30),
                             (float)GetRandomValue(30, ALTURA_JANELA - 30) };
        m->raio = 10.0f;

        int sorteioRaridade = GetRandomValue(0, 9);
        if (sorteioRaridade == 0) {
            m->tipo = MOEDA_DIAMANTE;
            m->raio = 13.0f; 
        } else {
            m->tipo = (TipoMoeda)GetRandomValue(MOEDA_BRONZE, MOEDA_OURO);
        }

        m->valor = valorDaMoeda(m->tipo);
        m->coletada = false;
        m->tempoColeta = 0.0f; 
    }
    return moedas;
}

bool tentarColetar(Moeda *m, Vector2 posJogador, float raioJogador) {
    if (m->coletada) return false;

    float dx = m->pos.x - posJogador.x;
    float dy = m->pos.y - posJogador.y;
    float distancia = (dx * dx + dy * dy);
    float somaRaios = (m->raio + raioJogador) * (m->raio + raioJogador);

    if (distancia <= somaRaios) {
        m->coletada = true;
        m->tempoColeta = (float)GetTime(); 
        return true;
    }
    return false;
}

#define TEMPO_REAPARECIMENTO 3.0f

void verificarReaparecimento(Moeda *moedas, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        Moeda *m = (moedas + i);

        if (m->coletada && (float)GetTime() - m->tempoColeta >= TEMPO_REAPARECIMENTO) {
            m->pos = (Vector2){ (float)GetRandomValue(30, LARGURA_JANELA - 30),
                                 (float)GetRandomValue(30, ALTURA_JANELA - 30) };

            int sorteioRaridade = GetRandomValue(0, 9);
            if (sorteioRaridade == 0) {
                m->tipo = MOEDA_DIAMANTE;
                m->raio = 13.0f;
            } else {
                m->tipo = (TipoMoeda)GetRandomValue(MOEDA_BRONZE, MOEDA_OURO);
                m->raio = 10.0f; 
            }
            m->valor = valorDaMoeda(m->tipo);

            m->coletada = false;
        }
    }
}

void desenharMoeda(const Moeda *m) {
    if (m->coletada) return; 

    Color cor = corDaMoeda(m->tipo);
    DrawCircleV(m->pos, m->raio, cor);
    DrawCircleLines((int)m->pos.x, (int)m->pos.y, m->raio, BLACK);
}

void desenharJogador(Vector2 pos, float raio) {
    DrawCircleV(pos, raio, BLUE);
    DrawCircleLines((int)pos.x, (int)pos.y, raio, DARKBLUE);
}
void moverJogador(Vector2 *pos, float raio, float velocidade) {
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) pos->x += velocidade;
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) pos->x -= velocidade;
    if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) pos->y += velocidade;
    if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) pos->y -= velocidade;

    if (pos->x - raio < 0) pos->x = raio;
    if (pos->x + raio > LARGURA_JANELA) pos->x = LARGURA_JANELA - raio;
    if (pos->y - raio < 0) pos->y = raio;
    if (pos->y + raio > ALTURA_JANELA) pos->y = ALTURA_JANELA - raio;
}

int main(void) {
    InitWindow(LARGURA_JANELA, ALTURA_JANELA,
               "Atividade 2 - Enum, Struct e Ponteiro para Struct em C (raylib)");
    SetTargetFPS(60);

    Moeda *moedas = criarMoedas(QUANTIDADE_MOEDAS);
    int quantidadeMoedas = QUANTIDADE_MOEDAS;

    Vector2 posJogador = { LARGURA_JANELA / 2.0f, ALTURA_JANELA / 2.0f };

    int pontuacao = 0;
    int moedasColetadas = 0;

    while (!WindowShouldClose()) {

        moverJogador(&posJogador, RAIO_JOGADOR, VELOCIDADE_JOGADOR);

        for (int i = 0; i < quantidadeMoedas; i++) {
            if (tentarColetar(moedas + i, posJogador, RAIO_JOGADOR)) {
                pontuacao += moedas[i].valor;
                moedasColetadas++;
            }
        }

        verificarReaparecimento(moedas, quantidadeMoedas);

        BeginDrawing();
            ClearBackground((Color){24, 24, 32, 255});

            for (int i = 0; i < quantidadeMoedas; i++) {
                desenharMoeda(moedas + i);
            }

            desenharJogador(posJogador, RAIO_JOGADOR);

            DrawText("Setas/WASD: mover | Colete as moedas!",
                      10, 10, 18, RAYWHITE);
            DrawText(TextFormat("Pontuacao: %d", pontuacao),
                      10, 34, 20, GOLD);
            DrawText(TextFormat("Coletas totais: %d", moedasColetadas),
                      10, 60, 18, RAYWHITE);

            DrawFPS(LARGURA_JANELA - 90, 10);

        EndDrawing();
    }

    free(moedas);

    CloseWindow();
    return 0;
}