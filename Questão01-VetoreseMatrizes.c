#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *vetor;
    int tamanho = 10;

    vetor = (int *) malloc(tamanho * sizeof(int));

    if (vetor == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    for (int i = 0; i < tamanho; i++) {
        vetor[i] = i;
    }

    printf("Vetor:\n");
    for (int i = 0; i < tamanho; i++) {
        printf("vetor[%d] = %d\n", i, vetor[i]);
    }

    free(vetor);

    return 0;
}