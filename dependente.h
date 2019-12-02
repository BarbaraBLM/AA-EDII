#ifndef DEPENDENTE_H
#define DEPENDENTE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct dependente{
    int cod;
    char* nome;
    int idade;
    int status;
    int cod_emp;
    int prox;
}Dependente;

Dependente* criarDependente(char* n, int idade, int cod_emp);
void salva_dep(Dependente *e, FILE *out);
Dependente* le_dep(FILE *in);
void imprime_dep(Dependente *e);
int tamanhoDependente();

#endif
