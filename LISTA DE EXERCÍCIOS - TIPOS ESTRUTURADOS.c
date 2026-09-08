#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TURMAS 10
#define MAX_VAGAS 3

struct aluno {
    int mat;
    char nome[81];
    float notas[3];
    float media;
};
typedef struct aluno Aluno;

struct turma {
    char id;      
    int vagas;
    Aluno* alunos[MAX_VAGAS];
};
typedef struct turma Turma;

Turma* turmas[MAX_TURMAS];
int num_turmas = 0;

Turma* cria_turma(char id) {
    Turma* nova = (Turma*) malloc(sizeof(Turma));

    if (nova == NULL) {
        printf("Erro ao alocar memoria para a turma!\n");
        return NULL;
    }

    nova->id = id;
    nova->vagas = MAX_VAGAS;

    for (int i = 0; i < MAX_VAGAS; i++) {
        nova->alunos[i] = NULL;
    }

    return nova;
}

void matricula_aluno(Turma* turma, int mat, char* nome) {
    if (turma->vagas <= 0) {
        printf("Nao ha vagas disponiveis nesta turma!\n");
        return;
    }

    for (int i = 0; i < MAX_VAGAS; i++) {
        if (turma->alunos[i] == NULL) {
            Aluno* novo_aluno = (Aluno*) malloc(sizeof(Aluno));

            if (novo_aluno == NULL) {
                printf("Erro ao alocar memoria para o aluno!\n");
                return;
            }

            novo_aluno->mat = mat;
            strcpy(novo_aluno->nome, nome);

            for (int j = 0; j < 3; j++) {
                novo_aluno->notas[j] = 0.0;
            }
            novo_aluno->media = 0.0;

            turma->alunos[i] = novo_aluno;
            turma->vagas--;

            printf("Aluno matriculado com sucesso!\n");
            return;
        }
    }
}

void lanca_notas(Turma* turma) {
    for (int i = 0; i < MAX_VAGAS; i++) {
        if (turma->alunos[i] != NULL) {
            Aluno* aluno = turma->alunos[i];

            printf("Matricula: %d\n", aluno->mat);
            printf("Aluno: %s\n", aluno->nome);

            float soma = 0.0;
            for (int j = 0; j < 3; j++) {
                printf("Digite a nota %d: ", j + 1);
                scanf("%f", &aluno->notas[j]);
                soma += aluno->notas[j];
            }

            aluno->media = soma / 3.0;
        }
    }
}

void imprime_alunos(Turma* turma) {
    int existe_aluno = 0;

    for (int i = 0; i < MAX_VAGAS; i++) {
        if (turma->alunos[i] != NULL) {
            existe_aluno = 1;
            Aluno* aluno = turma->alunos[i];

            printf("Matricula: %d\n", aluno->mat);
            printf("Nome: %s\n", aluno->nome);
            printf("Media: %.1f\n", aluno->media);
        }
    }

    if (!existe_aluno) {
        printf("Nenhum aluno matriculado nesta turma.\n");
    }
}

void imprime_turmas(Turma** turmas, int n) {
    if (n == 0) {
        printf("Nenhuma turma cadastrada.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        printf("Turma %c - %d vagas disponiveis\n", turmas[i]->id, turmas[i]->vagas);
    }
}

Turma* procura_turma(Turma** turmas, int n, char id) {
    for (int i = 0; i < n; i++) {
        if (turmas[i]->id == id) {
            return turmas[i];
        }
    }
    return NULL;
}

void libera_memoria(void) {
    for (int i = 0; i < num_turmas; i++) {
        for (int j = 0; j < MAX_VAGAS; j++) {
            if (turmas[i]->alunos[j] != NULL) {
                free(turmas[i]->alunos[j]);
            }
        }
        free(turmas[i]);
    }
}

int main(void) {
    int opcao;
    char id;
    int mat;
    char nome[81];
    Turma* turma_encontrada;

    printf("Bem-vindo ao Programa de Gerenciamento de Turmas!\n");
    printf("Este programa gerencia as turmas ofertadas, fornecendo as\n");
    printf("funcionalidades de matricula, lancamento de notas e listagem de alunos.\n");
    printf("Autor: Ada Lovelace\n");
    printf("Versao: 1.0\n\n");

    do {
        printf("MENU:\n");
        printf("1 - Criar turma\n");
        printf("2 - Listar turmas\n");
        printf("3 - Matricular aluno\n");
        printf("4 - Lancar notas\n");
        printf("5 - Listar alunos\n");
        printf("6 - Sair\n");
        printf("Digite sua opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {

            case 1:
                printf("\nCriando nova turma...\n");

                if (num_turmas >= MAX_TURMAS) {
                    printf("Numero maximo de turmas atingido!\n");
                    break;
                }

                printf("Digite um id: ");
                scanf(" %c", &id);

                turmas[num_turmas] = cria_turma(id);

                if (turmas[num_turmas] != NULL) {
                    printf("Turma %c criada com sucesso!\n", id);
                    num_turmas++;
                }
                break;

            case 2:
                printf("\nListando turmas...\n");
                imprime_turmas(turmas, num_turmas);
                break;

            case 3:
                printf("\nMatriculando aluno...\n");
                printf("Digite o id da turma: ");
                scanf(" %c", &id);

                turma_encontrada = procura_turma(turmas, num_turmas, id);

                if (turma_encontrada == NULL) {
                    printf("Turma inexistente!\n");
                    break;
                }

                printf("Digite a matricula: ");
                scanf("%d", &mat);

                printf("Digite o nome: ");
                scanf(" %80[^\n]", nome);

                matricula_aluno(turma_encontrada, mat, nome);
                break;

            case 4:
                printf("\nLancando notas...\n");
                printf("Digite o id da turma: ");
                scanf(" %c", &id);

                turma_encontrada = procura_turma(turmas, num_turmas, id);

                if (turma_encontrada == NULL) {
                    printf("Turma inexistente!\n");
                    break;
                }

                lanca_notas(turma_encontrada);
                break;

            case 5:
                printf("\nListando alunos...\n");
                printf("Digite o id da turma: ");
                scanf(" %c", &id);

                turma_encontrada = procura_turma(turmas, num_turmas, id);

                if (turma_encontrada == NULL) {
                    printf("Turma inexistente!\n");
                    break;
                }

                imprime_alunos(turma_encontrada);
                break;

            case 6:
                printf("Obrigado por usar este programa!\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

        printf("\n");

    } while (opcao != 6);

    libera_memoria();

    return 0;
}