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
    int prox_cod_emp;
    //A5
    int PT;
    int QTD;
}InvertidoDep;

InvertidoDep* criaInvertidoDep();
void qsort_nomeDep(InvertidoDep *regs[], int qtd_registros);
void qsort_idadeDep(InvertidoDep *regs[], int qtd_registros);
qsort_CodEmpDep(InvertidoDep *regs[], int qtd_registros);
void qsort_cpDep(InvertidoDep *regs[], int qtd_registros);
FILE** arquivo_invertido_depDep(FILE* dados, int qtd_registros);
void imprimeA8Dep(FILE** arqsInv, int qtd_registros);
Dependente** buscaIdadeMaiorQueXDep(FILE** arqsInv, int x, int *qtdTotal);
Dependente** buscaDependentesEmpDep(FILE** arqsInv, int x, int *qtdTotal);

#endif
