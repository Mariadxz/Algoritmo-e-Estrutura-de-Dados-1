#include <stdio.h>

typedef enum {
    MASCULINO,
    FEMININO
} Genero;

typedef struct {
    char nome[81];
    int idade;
    Genero genero;
} Pessoa;

int main(void) {
    Pessoa pessoa;
    int opcao_genero;

    printf("Digite o nome: ");
    scanf(" %80[^\n]", pessoa.nome);

    printf("Digite a idade: ");
    scanf("%d", &pessoa.idade);

    printf("Digite o genero (0 - Masculino, 1 - Feminino): ");
    scanf("%d", &opcao_genero);
    pessoa.genero = (Genero) opcao_genero;

    printf("\n--- Dados da Pessoa ---\n");
    printf("Nome: %s\n", pessoa.nome);
    printf("Idade: %d\n", pessoa.idade);
    printf("Genero: %s\n", (pessoa.genero == MASCULINO) ? "Masculino" : "Feminino");

    return 0;
}