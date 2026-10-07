
#include "raylib.h"
#include <stdlib.h>
#include <string.h>

#define LARGURA        1000
#define ALTURA         600
#define PONTUACAO_MAX  100
#define N              40

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

void ordenarInsertionSort(Placar *vetor, int n, long *comparacoes, long *trocas) {
    *comparacoes = 0;
    *trocas = 0;
    for (int i = 1; i < n; i++) {
        Placar chave = vetor[i];
        int j = i - 1;
        while (j >= 0) {
            (*comparacoes)++;
            if (vetor[j].pontuacao > chave.pontuacao) {
                vetor[j + 1] = vetor[j];
                (*trocas)++;
                j--;
            } else {
                break;
            }
        }
        vetor[j + 1] = chave;
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
    InitWindow(LARGURA, ALTURA, "Atividade 7 - Exercicio 1: Insertion sort");
    SetTargetFPS(60);

    Placar *placares = criarPlacares(N);
    Placar *original = (Placar *)malloc(N * sizeof(Placar));
    memcpy(original, placares, N * sizeof(Placar));

    bool ordenado = false;
    int alvo = placares[GetRandomValue(0, N - 1)].pontuacao;
    int indiceEncontrado = -1;

    long compBubble = -1, trocasBubble = 0;
    long compInsertion = -1, trocasInsertion = 0;
    long compSeq = -1, compBin = -1;
    const char *mensagem = "Pressione uma tecla para comecar.";

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_R)) {
            free(placares);
            free(original);
            placares = criarPlacares(N);
            original = (Placar *)malloc(N * sizeof(Placar));
            memcpy(original, placares, N * sizeof(Placar));
            ordenado = false;
            alvo = placares[GetRandomValue(0, N - 1)].pontuacao;
            indiceEncontrado = -1;
            compBubble = compInsertion = compSeq = compBin = -1;
            mensagem = "Novo vetor gerado.";
        }

        if (IsKeyPressed(KEY_B)) {
            memcpy(placares, original, N * sizeof(Placar));
            ordenarBubbleSort(placares, N, &compBubble, &trocasBubble);
            ordenado = true;
            indiceEncontrado = -1;
            mensagem = "Vetor ordenado com bubble sort.";
        }

        if (IsKeyPressed(KEY_I)) {
            memcpy(placares, original, N * sizeof(Placar));
            ordenarInsertionSort(placares, N, &compInsertion, &trocasInsertion);
            ordenado = true;
            indiceEncontrado = -1;
            mensagem = "Vetor ordenado com insertion sort.";
        }

        if (IsKeyPressed(KEY_T)) {
            alvo = placares[GetRandomValue(0, N - 1)].pontuacao;
            indiceEncontrado = -1;
            mensagem = "Novo alvo sorteado.";
        }

        if (IsKeyPressed(KEY_S)) {
            indiceEncontrado = buscaSequencial(placares, N, alvo, &compSeq);
            mensagem = (indiceEncontrado >= 0) ? "Busca sequencial: encontrado."
                                               : "Busca sequencial: nao encontrado.";
        }

        if (IsKeyPressed(KEY_W)) {
            if (ordenado) {
                indiceEncontrado = buscaBinaria(placares, N, alvo, &compBin);
                mensagem = (indiceEncontrado >= 0) ? "Busca binaria: encontrado."
                                                   : "Busca binaria: nao encontrado.";
            } else {
                mensagem = "Busca binaria exige vetor ORDENADO! (use B ou I)";
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        int topo = 230;
        int alturaMax = ALTURA - topo - 20;
        float larguraBarra = (float)(LARGURA - 20) / N;
        for (int i = 0; i < N; i++) {
            float h = (float)placares[i].pontuacao / PONTUACAO_MAX * alturaMax;
            Color cor = SKYBLUE;
            if (i == indiceEncontrado) cor = RED;
            else if (placares[i].pontuacao == alvo) cor = ORANGE;
            DrawRectangleV((Vector2){10 + i * larguraBarra, ALTURA - 10 - h},
                           (Vector2){larguraBarra - 1, h}, cor);
        }

        DrawText(TextFormat("n = %d | alvo = %d | ordenado: %s", N, alvo, ordenado ? "SIM" : "NAO"),
                 10, 10, 20, DARKGRAY);
        DrawText("R novo | B bubble | I insertion | T novo alvo | S sequencial | W binaria",
                 10, 36, 16, GRAY);
        DrawText(mensagem, 10, 60, 20, MAROON);

        DrawText(compBubble >= 0
                     ? TextFormat("Bubble:    %ld comparacoes | %ld trocas", compBubble, trocasBubble)
                     : "Bubble:    (nao executado)",
                 10, 100, 20, compBubble >= 0 ? BLACK : LIGHTGRAY);
        DrawText(compInsertion >= 0
                     ? TextFormat("Insertion: %ld comparacoes | %ld deslocamentos", compInsertion, trocasInsertion)
                     : "Insertion: (nao executado)",
                 10, 126, 20, compInsertion >= 0 ? BLACK : LIGHTGRAY);
        DrawText(compSeq >= 0 ? TextFormat("Busca sequencial: %ld comparacoes", compSeq)
                              : "Busca sequencial: (nao executada)",
                 10, 160, 20, compSeq >= 0 ? BLACK : LIGHTGRAY);
        DrawText(compBin >= 0 ? TextFormat("Busca binaria: %ld comparacoes", compBin)
                              : "Busca binaria: (nao executada)",
                 10, 186, 20, compBin >= 0 ? BLACK : LIGHTGRAY);

        EndDrawing();
    }

    free(placares);
    free(original);
    CloseWindow();
    return 0;
}