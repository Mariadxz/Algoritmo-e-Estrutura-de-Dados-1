#include <stdio.h>

typedef enum {
    JANEIRO = 1,
    FEVEREIRO,
    MARCO,
    ABRIL,
    MAIO,
    JUNHO,
    JULHO,
    AGOSTO,
    SETEMBRO,
    OUTUBRO,
    NOVEMBRO,
    DEZEMBRO
} Mes;

typedef struct {
    int dia;
    Mes mes;
    int ano;
} Data;

int main(void) {
    Data data;
    int opcao_mes;

    printf("Digite o dia: ");
    scanf("%d", &data.dia);

    printf("Digite o mes (1 - Janeiro, 2 - Fevereiro, ..., 12 - Dezembro): ");
    scanf("%d", &opcao_mes);
    data.mes = (Mes) opcao_mes;

    printf("Digite o ano: ");
    scanf("%d", &data.ano);

    printf("\nData: %02d/%02d/%04d\n", data.dia, data.mes, data.ano);

    return 0;
}