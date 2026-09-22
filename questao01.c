#include <stdio.h>
#include <stdlib.h>

int main(){
    int id;
    char nome[50];
    float salario;

    printf("Digite o ID do funcionario: ");
    scanf("%d", &id);
    printf("Digite o nome do funcionário: ");
    scanf("%s", nome);
    printf("Digite o salário do funcionário: ");
    scanf("%f", &salario);

    FILE * arq = fopen("entrada.txt", "w");
    if(arq == NULL){
        printf("Erro ao abrir o Arquivo de Texto! \n");
    } else {
        fprintf(arq, "%d %s %.2f \n", id, nome, salario);
    } fclose(arq);

    printf("Dados gravados com sucesso! \n");

    return 0;

    }