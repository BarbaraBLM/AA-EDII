#ifndef INVERTIDODEP_H
#define INVERTIDODEP_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "dependente.h"
#include "empregado.h"

typedef struct invertidoDep{

    int ED;
    //A8
    Dependente *dep;
    int prox_nome;
    int prox_idade;
    //A5
    int PT;
    int QTD;
}InvertidoDep;

InvertidoDep* criaInvertidoDep();
void qsort_nomeDep(InvertidoDep *regs[], int qtd_registros);
void qsort_idadeDep(InvertidoDep *regs[], int qtd_registros);
void qsort_cpDep(InvertidoDep *regs[], int qtd_registros);
FILE** arquivo_invertido_depDep(FILE* dados, int qtd_registros);

#endif
