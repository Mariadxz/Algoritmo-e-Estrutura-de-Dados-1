#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int tamanho;
    int *vetor;

    printf("Digite o tamanho do vetor: ");
    scanf("%d", &tamanho);

    vetor = (int *) malloc(tamanho * sizeof(int));

    if (vetor == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    for (int i = 0; i < tamanho; i++) {
        printf("Digite o elemento [%d]: ", i);
        scanf("%d", &vetor[i]);
    }

    printf("\nVetor em ordem inversa:\n");
    for (int i = tamanho - 1; i >= 0; i--) {
        printf("%d ", vetor[i]);
    }
    printf("\n");

    free(vetor);

    return 0;
}