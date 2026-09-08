#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    int quantidade;
    char **nomes;
    float *precos;

    printf("Quantas frutas deseja cadastrar? ");
    scanf("%d", &quantidade);

    nomes = (char **) malloc(quantidade * sizeof(char *));
    precos = (float *) malloc(quantidade * sizeof(float));

    if (nomes == NULL || precos == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    for (int i = 0; i < quantidade; i++) {
        nomes[i] = (char *) malloc(50 * sizeof(char));

        if (nomes[i] == NULL) {
            printf("Erro ao alocar memoria!\n");
            return 1;
        }

        printf("\nFruta %d:\n", i + 1);

        printf("Nome: ");
        scanf(" %49[^\n]", nomes[i]);

        printf("Preco: ");
        scanf("%f", &precos[i]);
    }

    quantidade++;

    char **temp_nomes = (char **) realloc(nomes, quantidade * sizeof(char *));
    float *temp_precos = (float *) realloc(precos, quantidade * sizeof(float));

    if (temp_nomes == NULL || temp_precos == NULL) {
        printf("Erro ao realocar memoria!\n");
        free(nomes);
        free(precos);
        return 1;
    }

    nomes = temp_nomes;
    precos = temp_precos;

    nomes[quantidade - 1] = (char *) malloc(50 * sizeof(char));

    if (nomes[quantidade - 1] == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    printf("\nDeseja cadastrar mais uma fruta (a ultima)?\n");
    printf("Nome: ");
    scanf(" %49[^\n]", nomes[quantidade - 1]);
    printf("Preco: ");
    scanf("%f", &precos[quantidade - 1]);

    printf("\n--- Lista de frutas cadastradas ---\n");
    for (int i = 0; i < quantidade; i++) {
        printf("Fruta: %-20s Preco: R$ %.2f\n", nomes[i], precos[i]);
    }

    for (int i = 0; i < quantidade; i++) {
        free(nomes[i]);
    }
    free(nomes);
    free(precos);

    return 0;
}