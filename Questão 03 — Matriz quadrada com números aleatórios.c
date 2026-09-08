#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    int n;
    int **matriz;

    printf("Digite a ordem da matriz quadrada: ");
    scanf("%d", &n);

    srand(time(NULL));

    matriz = (int **) malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        matriz[i] = (int *) malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matriz[i][j] = (rand() % 100) + 1; // numeros de 1 a 100
        }
    }

    printf("\nMatriz gerada:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d\t", matriz[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < n; i++) {
        free(matriz[i]);
    }
    free(matriz);

    return 0;
}