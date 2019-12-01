#ifndef INVERTIDODEP_H
#define INVERTIDODEP_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "dependente.h"

typedef struct invertido{

    int ED;
    //A8
    Dependente *dep;
    int prox_nome;
    int prox_idade;
    //A5
    int PT;
    int QTD;
}Invertido;

Invertido* criaInvertido();
void qsort_nome(Invertido *regs[], int qtd_registros);
void qsort_idade(Invertido *regs[], int qtd_registros);
void qsort_cp(Invertido *regs[], int qtd_registros);
FILE** arquivo_invertido_dep(FILE* dados, int qtd_registros);

#endif