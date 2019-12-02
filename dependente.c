#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dependente.h"

Dependente* criarDependente(char* n, int idade, int cod_emp){
	int static cod = 1;
    Dependente* e = (Dependente *) malloc(sizeof(Dependente));
    if (e) memset(e, 0, sizeof(Dependente));
    e->cod = cod;
    cod++;
    strcpy(e->nome, n);
    e->idade = idade;
    e->cod_emp = cod_emp;
    e->status = 1;
    e->prox =  -1;
    return e;
}

void salva_dep(Dependente *e, FILE *out){
	if(out != NULL){
        fwrite(&e->cod, sizeof(int), 1, out);
        fwrite(e->nome, sizeof(char), sizeof(e->nome), out);
        fwrite(&e->idade, sizeof(int), 1, out);
        fwrite(&e->cod_emp, sizeof(int), 1, out);
        fwrite(&e->status, sizeof(int), 1, out);
        fwrite(&e->prox, sizeof(int), 1, out);
    }
    else    printf("File passado é null");
    
    free(e);
}

Dependente* le_dep(FILE *in){
	Dependente *e = (Dependente*) malloc(sizeof(Dependente));
    if (0 >= fread(&e->cod, sizeof(int), 1, in))
    {
        free(e);
        return NULL;
    }
    fread(e->nome, sizeof(char), sizeof(e->nome), in);
    fread(&e->idade, sizeof(int), 1, in);
    fread(&e->cod_emp, sizeof(int), 1, in);
    fread(&e->status, sizeof(int), 1, in);
    fread(&e->prox, sizeof(int), 1, in);
    return e;
}

void imprime_dep(Dependente *e){
    printf("\n\nDependente:\nCod: %d\nNome: %s\nIdade:%d\nCod Empregado:%d\nStatus:%d\n", (e->cod), e->nome, e->idade, e->cod_emp, e->status);
}

int tamanhoDependente(){
	return sizeof(int)         //cod
         + sizeof(char) * 50   //nome
         + sizeof(int)         //idade
         + sizeof(int)         //status
         + sizeof(int)         //cod_emp
         + sizeof(int);        //prox
}
