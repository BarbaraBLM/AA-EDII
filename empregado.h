#ifndef EMPREGADO_H
#define EMPREGADO_H

#include<stdio.h>
#include<stdlib.h>

typedef struct empregado{
    int cod;
    char nome[50];
    int idade;
    double salario;
    int n_dependentes;
    // Atributos para a hash extensível com encadeamento exterior
    int prox;
    int dependentes;
    int status;
}Empregado;

Empregado* criarEmpregado(char* n, int idade, double sal);
void salva_empreg(Empregado *e, FILE *out);
Empregado* le_empreg(FILE *in);
void imprime_empreg(Empregado *e);
int tamanhoEmpregado();

#endif
