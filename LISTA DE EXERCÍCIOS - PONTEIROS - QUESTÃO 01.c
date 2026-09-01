//LISTA DE EXERCÍCIOS - PONTEIROS - QUESTÃO 01
#include <stdio.h>

int main(void) {
    int x, y, *p;

    y = 0;
    p = &y;
    x = *p;
    x = 4;
    (*p)++;
    --x;
    (*p) += x;

    printf("x = %d\n", x);
    printf("y = %d\n", y);
    printf("p aponta para o endereco: %p\n", (void *)p);
    printf("*p = %d\n", *p);

    return 0;
}

//Resultado final: x = 3, y = 4, p = endereço de y (ou seja, p continua apontando para y, e *p vale 4).



