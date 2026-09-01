//LISTA DE EXERCÍCIOS - PONTEIROS - QUESTÃO 02

#include <stdio.h>

int main(void) {
    int x, *p;

    x = 100;
    p = &x;  // agora p recebe corretamente o ENDERECO de x

    printf("Valor de p = %p\tValor de *p = %d\n", (void *)p, *p);

    return 0;
}

//a) Erro ou advertência?
//É uma advertência (warning), não um erro. O código compila normalmente (a menos que se
// use uma flag como -Werror, que transforma warnings em erros).

//b) Por que o compilador emite essa mensagem?
//Porque p é do tipo int * (ponteiro para inteiro) e x é do tipo int (um inteiro comum).
//A atribuição p = x está colocando um valor inteiro diretamente dentro de uma variável
//ponteiro, sem conversão explícita (cast) e sem que esse número represente de fato um 
//endereço válido. O compilador avisa algo como: "assignment makes pointer from integer
//without a cast".

//c) Compile e execute. Foi bem-sucedida?
//Não. Como p recebe o valor 100 (não o endereço de nada), quando o programa tenta acessar
//*p no printf, ele está tentando ler o conteúdo do endereço de memória 0x64 (100 em hexade-
//cimal), que muito provavelmente não pertence ao seu programa. Isso resulta em falha de seg-
//mentação (segmentation fault) ou comportamento indefinido.

//d) Correção (sem mensagens do compilador):

#include <stdio.h>

int main(void) {
    int x, *p;
    x = 100;
    p = &x;   // agora p recebe o ENDEREÇO de x
    printf("Valor de p = %p\tValor de *p = %d\n", (void *)p, *p);
    return 0;
}
//e)A execução foi bem sucedida?
//Foi sim, Agora p armazena corretamente o endereço de x, então *p acessa o valor 100 sem problemas,
