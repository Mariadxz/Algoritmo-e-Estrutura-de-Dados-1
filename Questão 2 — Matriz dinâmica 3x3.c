#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int linhas = 3, colunas = 3;
    int **matriz;

    matriz = (int **) malloc(linhas * sizeof(int *));

    if (matriz == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    for (int i = 0; i < linhas; i++) {
        matriz[i] = (int *) malloc(colunas * sizeof(int));

        if (matriz[i] == NULL) {
            printf("Erro ao alocar memoria!\n");
            return 1;
        }
    }

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            matriz[i][j] = i * 3 + j;
        }
    }

    printf("Matriz:\n");
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%d\t", matriz[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);

    return 0;
}