#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PESSOAS 100
#define TAM_NOME 100

int main(void) {
    int quantidade;
    char **nomes;
    int *idades;

    printf("Digite o numero de pessoas (max %d): ", MAX_PESSOAS);
    scanf("%d", &quantidade);

    if (quantidade > MAX_PESSOAS || quantidade <= 0) {
        printf("Quantidade invalida!\n");
        return 1;
    }

    nomes = (char **) malloc(quantidade * sizeof(char *));
    idades = (int *) malloc(quantidade * sizeof(int));

    if (nomes == NULL || idades == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    for (int i = 0; i < quantidade; i++) {
        nomes[i] = (char *) malloc(TAM_NOME * sizeof(char));

        if (nomes[i] == NULL) {
            printf("Erro ao alocar memoria!\n");
            return 1;
        }

        printf("\nPessoa %d:\n", i + 1);

        printf("Nome completo: ");
        scanf(" %99[^\n]", nomes[i]);

        printf("Idade: ");
        scanf("%d", &idades[i]);
    }

    printf("\n--- Nomes armazenados ---\n");
    for (int i = 0; i < quantidade; i++) {
        printf("%s - %d anos\n", nomes[i], idades[i]);
    }

    for (int i = 0; i < quantidade; i++) {
        free(nomes[i]);
    }
    free(nomes);
    free(idades);

    return 0;
}