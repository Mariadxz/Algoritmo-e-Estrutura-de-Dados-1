#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LARGURA        800
#define ALTURA         600
#define MAX_ENTIDADES  64
#define ARQUIVO_PLACAR "placar.txt"
#define ARQUIVO_SAVE   "save.bin"
#define NOME_MAX       15

typedef enum {
    ENT_JOGADOR = 0,
    ENT_INIMIGO,
    ENT_ITEM
} TipoEntidade;

typedef union {
    struct { int vida; int pontos; float invulneravel; } jogador;
    struct { float vx; float vy; int dano; } inimigo;
    struct { int valor; } item;
} DadosEspecificos;

typedef struct {
    int              id;
    TipoEntidade     tipo;
    float            x, y;
    float            raio;
    Color            cor;
    DadosEspecificos dados;
} Entidade;

static Entidade *vetorEntidades[MAX_ENTIDADES];
static int       totalEntidades = 0;
static int       proximoId = 1;

static char  mensagem[128] = "";
static float tempoMensagem = 0.0f;
static Color corMensagem   = DARKGREEN;

static void mostrarMensagem(const char *texto, Color cor)
{
    snprintf(mensagem, sizeof(mensagem), "%s", texto);
    corMensagem   = cor;
    tempoMensagem = 3.0f;
}

static bool adicionarEntidade(Entidade *e)
{
    if (e == NULL || totalEntidades >= MAX_ENTIDADES) return false;
    vetorEntidades[totalEntidades++] = e;
    return true;
}

static void liberarTodasEntidades(void)
{
    for (int i = 0; i < totalEntidades; i++) {
        free(vetorEntidades[i]);
        vetorEntidades[i] = NULL;
    }
    totalEntidades = 0;
}

static Entidade *criarEntidadeBase(TipoEntidade tipo, float x, float y, float raio, Color cor)
{
    Entidade *e = (Entidade *)calloc(1, sizeof(Entidade));
    if (e == NULL) return NULL;
    e->id   = proximoId++;
    e->tipo = tipo;
    e->x = x;
    e->y = y;
    e->raio = raio;
    e->cor  = cor;
    return e;
}

static Entidade *criarJogador(void)
{
    Entidade *e = criarEntidadeBase(ENT_JOGADOR, LARGURA / 2.0f, ALTURA / 2.0f, 16, BLUE);
    if (e) {
        e->dados.jogador.vida = 5;
        e->dados.jogador.pontos = 0;
        e->dados.jogador.invulneravel = 0.0f;
    }
    return e;
}

static Entidade *criarInimigo(void)
{
    Entidade *e = criarEntidadeBase(ENT_INIMIGO,
                                    (float)GetRandomValue(40, LARGURA - 40),
                                    (float)GetRandomValue(40, ALTURA - 40), 20, RED);
    if (e) {
        e->dados.inimigo.vx = (float)GetRandomValue(-140, 140);
        e->dados.inimigo.vy = (float)GetRandomValue(-140, 140);
        if (e->dados.inimigo.vx == 0) e->dados.inimigo.vx = 90;
        if (e->dados.inimigo.vy == 0) e->dados.inimigo.vy = -90;
        e->dados.inimigo.dano = 1;
    }
    return e;
}

static Entidade *criarItem(void)
{
    Entidade *e = criarEntidadeBase(ENT_ITEM,
                                    (float)GetRandomValue(30, LARGURA - 30),
                                    (float)GetRandomValue(30, ALTURA - 30), 10, GOLD);
    if (e) e->dados.item.valor = 10;
    return e;
}

static Entidade *encontrarJogador(void)
{
    for (int i = 0; i < totalEntidades; i++)
        if (vetorEntidades[i]->tipo == ENT_JOGADOR) return vetorEntidades[i];
    return NULL;
}

static void iniciarJogo(void)
{
    liberarTodasEntidades();
    proximoId = 1;

    adicionarEntidade(criarJogador());
    for (int i = 0; i < 4; i++) adicionarEntidade(criarInimigo());
    for (int i = 0; i < 6; i++) adicionarEntidade(criarItem());
}

static bool salvarPlacarTexto(const char *nome, int pontuacao)
{
    FILE *arquivo = fopen(ARQUIVO_PLACAR, "a");
    if (arquivo == NULL) return false;

    fprintf(arquivo, "%s %d\n", nome, pontuacao);
    if (fclose(arquivo) != 0) return false;
    return true;
}

static int lerMelhorPontuacao(char *nomeMelhor, size_t tam)
{
    nomeMelhor[0] = '\0';

    FILE *arquivo = fopen(ARQUIVO_PLACAR, "r");
    if (arquivo == NULL) return 0;

    char nomeLido[NOME_MAX + 1];
    int melhor = 0, valor = 0;

    while (fscanf(arquivo, "%15s %d", nomeLido, &valor) == 2) {
        if (valor > melhor) {
            melhor = valor;
            snprintf(nomeMelhor, tam, "%s", nomeLido);
        }
    }
    fclose(arquivo);
    return melhor;
}

static bool salvarJogoBinario(void)
{
    FILE *arquivo = fopen(ARQUIVO_SAVE, "wb");
    if (arquivo == NULL) return false;

    bool ok = (fwrite(&totalEntidades, sizeof(int), 1, arquivo) == 1);
    for (int i = 0; ok && i < totalEntidades; i++) {

        ok = (fwrite(vetorEntidades[i], sizeof(Entidade), 1, arquivo) == 1);
    }

    if (fclose(arquivo) != 0) ok = false;
    return ok;
}

static bool carregarJogoBinario(void)
{
    FILE *arquivo = fopen(ARQUIVO_SAVE, "rb");
    if (arquivo == NULL) return false;

    int totalSalvo = 0;
    if (fread(&totalSalvo, sizeof(int), 1, arquivo) != 1
        || totalSalvo <= 0 || totalSalvo > MAX_ENTIDADES) {
        fclose(arquivo);
        return false;
    }

    Entidade *novas[MAX_ENTIDADES];
    int lidas = 0;

    while (lidas < totalSalvo) {
        Entidade *e = (Entidade *)malloc(sizeof(Entidade));
        if (e == NULL) break;
        if (fread(e, sizeof(Entidade), 1, arquivo) != 1) {
            free(e);
            break;
        }
        novas[lidas++] = e;
    }
    fclose(arquivo);

    if (lidas != totalSalvo || novas[0]->tipo != ENT_JOGADOR) {
        for (int i = 0; i < lidas; i++) free(novas[i]);
        return false;
    }

    liberarTodasEntidades();
    proximoId = 1;
    for (int i = 0; i < totalSalvo; i++) {
        adicionarEntidade(novas[i]);
        if (novas[i]->id >= proximoId) proximoId = novas[i]->id + 1;
    }
    return true;
}

static void atualizar(Entidade *jogador, float dt)
{

    float vel = 240.0f;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) jogador->x += vel * dt;
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) jogador->x -= vel * dt;
    if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) jogador->y += vel * dt;
    if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) jogador->y -= vel * dt;

    if (jogador->x < jogador->raio) jogador->x = jogador->raio;
    if (jogador->y < jogador->raio) jogador->y = jogador->raio;
    if (jogador->x > LARGURA - jogador->raio) jogador->x = LARGURA - jogador->raio;
    if (jogador->y > ALTURA  - jogador->raio) jogador->y = ALTURA  - jogador->raio;

    if (jogador->dados.jogador.invulneravel > 0)
        jogador->dados.jogador.invulneravel -= dt;

    for (int i = 0; i < totalEntidades; i++) {
        Entidade *e = vetorEntidades[i];
        Vector2 pe = { e->x, e->y };
        Vector2 pj = { jogador->x, jogador->y };

        switch (e->tipo) {
        case ENT_INIMIGO:
            e->x += e->dados.inimigo.vx * dt;
            e->y += e->dados.inimigo.vy * dt;
            if (e->x < e->raio || e->x > LARGURA - e->raio) e->dados.inimigo.vx *= -1;
            if (e->y < e->raio || e->y > ALTURA  - e->raio) e->dados.inimigo.vy *= -1;

            if (jogador->dados.jogador.invulneravel <= 0
                && CheckCollisionCircles(pe, e->raio, pj, jogador->raio)) {
                jogador->dados.jogador.vida -= e->dados.inimigo.dano;
                jogador->dados.jogador.invulneravel = 1.5f;
            }
            break;

        case ENT_ITEM:
            if (CheckCollisionCircles(pe, e->raio, pj, jogador->raio)) {
                jogador->dados.jogador.pontos += e->dados.item.valor;

                e->x = (float)GetRandomValue(30, LARGURA - 30);
                e->y = (float)GetRandomValue(30, ALTURA - 30);
            }
            break;

        case ENT_JOGADOR:
        default:
            break;
        }
    }
}

static void desenhar(const Entidade *jogador, const char *nome, int melhor, const char *nomeMelhor)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int i = 0; i < totalEntidades; i++) {
        const Entidade *e = vetorEntidades[i];
        Color c = e->cor;
        if (e->tipo == ENT_JOGADOR && e->dados.jogador.invulneravel > 0
            && ((int)(GetTime() * 10) % 2 == 0))
            c = Fade(c, 0.4f);
        DrawCircle((int)e->x, (int)e->y, e->raio, c);
    }

    DrawText(TextFormat("Jogador: %s", nome),                         10, 10, 20, DARKBLUE);
    DrawText(TextFormat("Pontos: %d", jogador->dados.jogador.pontos), 10, 34, 20, DARKGRAY);
    DrawText(TextFormat("Vida: %d",   jogador->dados.jogador.vida),   10, 58, 20, DARKGRAY);
    if (melhor > 0)
        DrawText(TextFormat("Recorde: %d (%s)", melhor, nomeMelhor),  10, 82, 20, DARKGRAY);
    else
        DrawText("Recorde: --",                                       10, 82, 20, DARKGRAY);
    DrawText(TextFormat("Entidades: %d", totalEntidades),             10, 106, 20, DARKGRAY);

    DrawText("F5: placar (texto) | F6: salvar | F9: carregar | R: reiniciar",
             10, ALTURA - 26, 16, GRAY);

    if (tempoMensagem > 0)
        DrawText(mensagem, 10, 136, 20, corMensagem);

    if (jogador->dados.jogador.vida <= 0) {
        DrawRectangle(0, 0, LARGURA, ALTURA, Fade(BLACK, 0.6f));
        DrawText("FIM DE JOGO", LARGURA / 2 - 110, ALTURA / 2 - 30, 36, WHITE);
        DrawText("R: reiniciar  |  F9: carregar save", LARGURA / 2 - 160, ALTURA / 2 + 15, 20, LIGHTGRAY);
    }

    EndDrawing();
}

static bool pedirNome(char *nome, int tam)
{
    int len = 0;
    nome[0] = '\0';

    while (!WindowShouldClose()) {
        int c;
        while ((c = GetCharPressed()) > 0) {
            if (c > 32 && c < 127 && len < tam - 1) {
                nome[len++] = (char)c;
                nome[len] = '\0';
            }
        }
        if (IsKeyPressed(KEY_BACKSPACE) && len > 0) nome[--len] = '\0';

        if (IsKeyPressed(KEY_ENTER)) {
            if (len == 0) snprintf(nome, tam, "Jogador");
            return true;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Digite seu nome e pressione ENTER", 160, 220, 28, DARKGRAY);
        DrawRectangle(250, 280, 300, 44, LIGHTGRAY);
        DrawRectangleLines(250, 280, 300, 44, DARKGRAY);
        DrawText(nome, 260, 290, 26, BLACK);
        if ((int)(GetTime() * 2) % 2 == 0)
            DrawText("_", 260 + MeasureText(nome, 26), 290, 26, BLACK);
        DrawText(TextFormat("%d/%d caracteres (sem espacos)", len, tam - 1), 250, 335, 16, GRAY);
        EndDrawing();
    }
    return false;
}

int main(void)
{
    InitWindow(LARGURA, ALTURA, "Atividade 6 - Arquivos em C");
    SetTargetFPS(60);

    char nomeJogador[NOME_MAX + 1];
    if (!pedirNome(nomeJogador, sizeof(nomeJogador))) {
        CloseWindow();
        return 0;
    }

    iniciarJogo();
    Entidade *jogador = vetorEntidades[0];
    char nomeMelhor[NOME_MAX + 1];
    int melhor = lerMelhorPontuacao(nomeMelhor, sizeof(nomeMelhor));

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (tempoMensagem > 0) tempoMensagem -= dt;

        if (IsKeyPressed(KEY_R)) {
            iniciarJogo();
            jogador = vetorEntidades[0];
            mostrarMensagem("Jogo reiniciado.", DARKGRAY);
        }

        if (IsKeyPressed(KEY_F5)) {
            if (salvarPlacarTexto(nomeJogador, jogador->dados.jogador.pontos)) {
                melhor = lerMelhorPontuacao(nomeMelhor, sizeof(nomeMelhor));
                mostrarMensagem("Pontuacao anexada em placar.txt", DARKGREEN);
            } else {
                mostrarMensagem("ERRO: nao foi possivel abrir placar.txt", RED);
            }
        }

        if (IsKeyPressed(KEY_F6)) {
            if (salvarJogoBinario())
                mostrarMensagem("Jogo salvo em save.bin", DARKGREEN);
            else
                mostrarMensagem("ERRO: falha ao salvar save.bin", RED);
        }

        if (IsKeyPressed(KEY_F9)) {
            if (carregarJogoBinario()) {
                jogador = encontrarJogador();
                mostrarMensagem("Jogo carregado de save.bin", DARKGREEN);
            } else {
                mostrarMensagem("ERRO: save.bin ausente ou invalido", RED);
            }
        }

        if (jogador->dados.jogador.vida > 0)
            atualizar(jogador, dt);

        desenhar(jogador, nomeJogador, melhor, nomeMelhor);
    }

    liberarTodasEntidades();
    CloseWindow();
    return 0;
}