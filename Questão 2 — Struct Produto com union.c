#include <stdio.h>

typedef enum {
    ALIMENTO,
    BEBIDA,
    ELETRONICO
} TipoProduto;

typedef union {
    int validade_dias;      
    float volume_litros;     
    int garantia_meses;      
} DadoExtra;

typedef struct {
    char nome[81];
    float preco;
    TipoProduto tipo;
    DadoExtra extra;
} Produto;

int main(void) {
    Produto produto;
    int opcao_tipo;

    printf("Digite o nome do produto: ");
    scanf(" %80[^\n]", produto.nome);

    printf("Digite o preco: ");
    scanf("%f", &produto.preco);

    printf("Digite o tipo (0 - Alimento, 1 - Bebida, 2 - Eletronico): ");
    scanf("%d", &opcao_tipo);
    produto.tipo = (TipoProduto) opcao_tipo;

    switch (produto.tipo) {
        case ALIMENTO:
            printf("Digite a validade (em dias): ");
            scanf("%d", &produto.extra.validade_dias);
            break;

        case BEBIDA:
            printf("Digite o volume (em litros): ");
            scanf("%f", &produto.extra.volume_litros);
            break;

        case ELETRONICO:
            printf("Digite a garantia (em meses): ");
            scanf("%d", &produto.extra.garantia_meses);
            break;
    }

    printf("\n--- Dados do Produto ---\n");
    printf("Nome: %s\n", produto.nome);
    printf("Preco: R$ %.2f\n", produto.preco);

    switch (produto.tipo) {
        case ALIMENTO:
            printf("Tipo: Alimento\n");
            printf("Validade: %d dias\n", produto.extra.validade_dias);
            break;

        case BEBIDA:
            printf("Tipo: Bebida\n");
            printf("Volume: %.2f litros\n", produto.extra.volume_litros);
            break;

        case ELETRONICO:
            printf("Tipo: Eletronico\n");
            printf("Garantia: %d meses\n", produto.extra.garantia_meses);
            break;
    }

    return 0;
}