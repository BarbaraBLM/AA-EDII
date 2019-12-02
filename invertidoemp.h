#ifndef INVERTIDOEMP_H
#define INVERTIDOEMP_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "dependente.h"
#include "empregado.h"

typedef struct invertido{
    int ED;
    //A8
    Empregado *emp;
    int prox_nome;
    int prox_idade;
    int prox_salario;
    int prox_n_dependentes;
    //A5
    int PT;
    int QTD;
}Invertido;

Invertido* criaInvertido();
void qsort_nome(Invertido *regs[], int qtd_registros);
void qsort_idade(Invertido *regs[], int qtd_registros);
void qsort_salario(Invertido *regs[], int qtd_registros);
void qsort_ndependentes(Invertido *regs[], int qtd_registros);
void qsort_cp(Invertido *regs[], int qtd_registros);
FILE** arquivo_invertido_emp(FILE* dados, int qtd_registros);
Empregado** buscaNome(FILE** arqsInv, char* nome);

#endif
