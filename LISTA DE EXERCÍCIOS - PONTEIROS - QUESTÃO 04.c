//LISTA DE EXERCÍCIOS - PONTEIROS - QUESTÂO 04

#include <stdio.h>
#include <math.h>

void calcula_hexagono(float l, float *area, float *perimetro) {
    *area = (3.0 * pow(l, 2) * sqrt(3.0)) / 2.0;
    *perimetro = 6.0 * l;
}

int main(void) {
    float l, area, perimetro;

    printf("Digite o valor do lado do hexagono: \n");
    scanf("%f", &l);

    calcula_hexagono(l, &area, &perimetro);

    printf("Area do hexagono = %.2f\n", area);
    printf("Perimetro do hexagono = %.2f\n", perimetro);

    return 0;
}