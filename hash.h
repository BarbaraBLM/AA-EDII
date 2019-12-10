#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include "dependente.h"
#include "empregado.h"

void initHash(FILE *h, int tam, int c);
int hash(int n, int tam, int l);
void imprimeHash(FILE *hash);
void inserirHashEmp(FILE *h, FILE *r, FILE *exclusao, Empregado *emp, int tam, int p, int l, int *qtd_registros);
void inserirHashDep(FILE *h, FILE *r, FILE *exclusao, Dependente *dep, int tam, int p, int l, int *qtd_registros)
void expandHashEmp(FILE *h, FILE *r, int tam, int* p, int l);
void expandHashDep(FILE *h, FILE *r, int tam, int* p, int l);
int buscarCodEmp(FILE *h, FILE* regts, int cod, int tam, int p, int l);
int buscarCodDep(FILE *h, FILE* regts, int cod, int tam, int p, int l);
void excluirHashEmp(FILE *h, FILE *r, FILE *exclusao, int end, int tam, int p, int l, int *qtd_registros);
void excluirHashDep(FILE *h, FILE *r, FILE *exclusao, int end, int tam, int p, int l, int *qtd_registros);
void incrementa_ndepEmp(FILE* h, FILE* r, int cod, int tam, int l);
void decrementa_ndepEmp(FILE* h, FILE* r, int cod, int tam, int l);
alteraNomeEmp(FILE* h, FILE* r, int cod, int tam, int l, char* n);
void alteraIdadeEmp(FILE* h, FILE* r, int cod, int tam, int l, int idade);
void alteraSalarioEmp(FILE* hash, FILE* regts, int cod, inttamHash, int l, double salario);
//Remove o '\n' que o fgets salva ao ler string
void clean_fgets_error(char str[]);


#endif
