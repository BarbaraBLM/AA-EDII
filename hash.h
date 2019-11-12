#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include "dependente.h"
#include "empregado.h"

void initHash(FILE *h, int tam);
int hash(int n, int tam, int l);
void imprimeHash(FILE *hash);
void inserirHash(FILE *h, FILE *r, FILE *exclusao, Empregado *emp, int tam, int p, int l, int *qtd_registros);
void expandHash(FILE *h, FILE *r, int tam, int* p, int l);
int buscarCod(FILE *h, FILE* regts, int cod, int tam, int p, int l);
void excluirHash(FILE *h, FILE *r, FILE *exclusao, int end, int tam, int p, int l);

#endif
