#include "raylib.h"
#include <stdlib.h>
#include <string.h>

#define LARGURA        1000
#define ALTURA         600
#define PONTUACAO_MAX  100
#define N_INICIAL      40
#define N_MIN          10
#define N_MAX          2560

typedef struct {
    char nome[16];
    int pontuacao;
} Placar;

Placar *criarPlacares(int quantidade) {
    Placar *placares = (Placar *)malloc(quantidade * sizeof(Placar));
    for (int i = 0; i < quantidade; i++) {
        Placar *p = (placares + i);
        TextCopy(p->nome, TextFormat("J%02d", i + 1));
        p->pontuacao = GetRandomValue(10, PONTUACAO_MAX);
    }
    return placares;
}

void ordenarBubbleSort(Placar *vetor, int n, long *comparacoes, long *trocas) {
    *comparacoes = 0;
    *trocas = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            (*comparacoes)++;
            if (vetor[j].pontuacao > vetor[j + 1].pontuacao) {
                Placar temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
                (*trocas)++;
            }
        }
    }
}

int buscaSequencial(Placar *vetor, int n, int alvo, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n; i++) {
        (*comparacoes)++;
        if (vetor[i].pontuacao == alvo) return i;
    }
    return -1;
}

int buscaBinaria(Placar *vetor, int n, int alvo, long *comparacoes) {
    *comparacoes = 0;
    int inicio = 0, fim = n - 1;
    while (inicio <= fim) {
        (*comparacoes)++;
        int meio = (inicio + fim) / 2;
        if (vetor[meio].pontuacao == alvo) return meio;
        if (vetor[meio].pontuacao < alvo) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

int main(void) {
    InitWindow(LARGURA, ALTURA, "Atividade 7 - Exercicio 2: Tempo de execucao");
    SetTargetFPS(60);

    int n = N_INICIAL;
    Placar *placares = criarPlacares(n);
    bool ordenado = false;
    int alvo = placares[GetRandomValue(0, n - 1)].pontuacao;
    int indiceEncontrado = -1;

    long compBubble = -1, trocasBubble = 0;
    double tempoBubbleMs = 0.0;
    long compSeq = -1, compBin = -1;
    const char *mensagem = "Pressione uma tecla para comecar.";

    while (!WindowShouldClose()) {

        bool novoVetor = IsKeyPressed(KEY_R);
        if (IsKeyPressed(KEY_UP) && n * 2 <= N_MAX) { n *= 2; novoVetor = true; }
        if (IsKeyPressed(KEY_DOWN) && n / 2 >= N_MIN) { n /= 2; novoVetor = true; }

        if (novoVetor) {
            free(placares);
            placares = criarPlacares(n);
            ordenado = false;
            alvo = placares[GetRandomValue(0, n - 1)].pontuacao;
            indiceEncontrado = -1;
            compBubble = compSeq = compBin = -1;
            mensagem = "Novo vetor gerado.";
        }

        if (IsKeyPressed(KEY_B)) {
            /* EXERCÍCIO 2: GetTime() antes e depois da chamada */
            double inicio = GetTime();
            ordenarBubbleSort(placares, n, &compBubble, &trocasBubble);
            double fim = GetTime();
            tempoBubbleMs = (fim - inicio) * 1000.0;

            ordenado = true;
            indiceEncontrado = -1;
            mensagem = "Vetor ordenado com bubble sort.";
        }

        if (IsKeyPressed(KEY_T)) {
            alvo = placares[GetRandomValue(0, n - 1)].pontuacao;
            indiceEncontrado = -1;
            mensagem = "Novo alvo sorteado.";
        }

        if (IsKeyPressed(KEY_S)) {
            indiceEncontrado = buscaSequencial(placares, n, alvo, &compSeq);
            mensagem = (indiceEncontrado >= 0) ? "Busca sequencial: encontrado."
                                               : "Busca sequencial: nao encontrado.";
        }

        if (IsKeyPressed(KEY_W)) {
            if (ordenado) {
                indiceEncontrado = buscaBinaria(placares, n, alvo, &compBin);
                mensagem = (indiceEncontrado >= 0) ? "Busca binaria: encontrado."
                                                   : "Busca binaria: nao encontrado.";
            } else {
                mensagem = "Busca binaria exige vetor ORDENADO! (use B)";
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        int topo = 200;
        int alturaMax = ALTURA - topo - 20;
        float larguraBarra = (float)(LARGURA - 20) / n;
        for (int i = 0; i < n; i++) {
            float h = (float)placares[i].pontuacao / PONTUACAO_MAX * alturaMax;
            Color cor = SKYBLUE;
            if (i == indiceEncontrado) cor = RED;
            else if (placares[i].pontuacao == alvo) cor = ORANGE;
            float w = larguraBarra > 3 ? larguraBarra - 1 : larguraBarra;
            DrawRectangleV((Vector2){10 + i * larguraBarra, ALTURA - 10 - h},
                           (Vector2){w, h}, cor);
        }

        DrawText(TextFormat("n = %d | alvo = %d | ordenado: %s", n, alvo, ordenado ? "SIM" : "NAO"),
                 10, 10, 20, DARKGRAY);
        DrawText("R novo | B bubble | T novo alvo | S sequencial | W binaria | CIMA/BAIXO muda n",
                 10, 36, 16, GRAY);
        DrawText(mensagem, 10, 60, 20, MAROON);

        if (compBubble >= 0) {
            DrawText(TextFormat("Bubble: %ld comparacoes | %ld trocas | %.4f ms",
                                compBubble, trocasBubble, tempoBubbleMs),
                     10, 100, 20, BLACK);
        } else {
            DrawText("Bubble: (nao executado)", 10, 100, 20, LIGHTGRAY);
        }
        DrawText(compSeq >= 0 ? TextFormat("Busca sequencial: %ld comparacoes", compSeq)
                              : "Busca sequencial: (nao executada)",
                 10, 130, 20, compSeq >= 0 ? BLACK : LIGHTGRAY);
        DrawText(compBin >= 0 ? TextFormat("Busca binaria: %ld comparacoes", compBin)
                              : "Busca binaria: (nao executada)",
                 10, 156, 20, compBin >= 0 ? BLACK : LIGHTGRAY);

        EndDrawing();
    }

    free(placares);
    CloseWindow();
    return 0;
}