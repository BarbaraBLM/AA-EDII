#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "empregado.h"
#include "dependente.h"
#include "invertidodep.h"

InvertidoDep* criaInvertidoDep(){

	InvertidoDep *novo = (InvertidoDep *)malloc(sizeof(InvertidoDep));
	novo->prox_nome = -1;
	novo->prox_idade = -1;
	novo->PT = -1;
	novo->QTD = 1;

	return novo;
}

void qsort_nomeDep(InvertidoDep *regs[], int qtd_registros){

	if(qtd_registros>1){

		InvertidoDep *x = regs[0];
		int a = 1;
		int b = qtd_registros-1;

		do{
			while(a<qtd_registros && strcmp(regs[a]->dep->nome,x->dep->nome)<=0) a++;
			while(strcmp(regs[b]->dep->nome,x->dep->nome)>0) b--;
			if(a<b){
				InvertidoDep *temp = regs[a];
				regs[a] = regs[b];
				regs[b] = temp;
				a++;
				b--;
			}
		}while(a<=b);

		regs[0] = regs[b];
		regs[b] = x;

		qsort_nomeDep(regs,b);
		qsort_nomeDep(&regs[a], qtd_registros-a);
	}
}

void qsort_idadeDep(InvertidoDep *regs[], int qtd_registros){

	if(qtd_registros>1){

		InvertidoDep *x = regs[0];
		int a = 1;
		int b = qtd_registros-1;

		do{
			while(a<qtd_registros && regs[a]->dep->idade <= x->dep->idade) a++;
			while(regs[b]->dep->idade > x->dep->idade) b--;
			if(a<b){
				InvertidoDep *temp = regs[a];
				regs[a] = regs[b];
				regs[b] = temp;
				a++;
				b--;
			}
		}while(a<=b);

		regs[0] = regs[b];
		regs[b] = x;

		qsort_idadeDep(regs,b);
		qsort_idadeDep(&regs[a], qtd_registros-a);
	}
}

void qsort_cpDep(InvertidoDep *regs[], int qtd_registros){

	if(qtd_registros>1){

		InvertidoDep *x = regs[0];
		int a = 1;
		int b = qtd_registros-1;

		do{
			while(a<qtd_registros && regs[a]->ED <= x->ED) a++;
			while(regs[b]->ED > x->ED) b--;
			if(a<b){
				InvertidoDep *temp = regs[a];
				regs[a] = regs[b];
				regs[b] = temp;
				a++;
				b--;
			}
		}while(a<=b);

		regs[0] = regs[b];
		regs[b] = x;

		qsort_cpDep(regs,b);
		qsort_cpDep(&regs[a], qtd_registros-a);
	}
}

FILE** arquivo_invertido_dep(FILE* dados, int qtd_registros){

	FILE** ret = malloc(3*sizeof(FILE*));

	for(int i=0; i< 3; i++){
		ret[i] =  malloc(sizeof(FILE*));
	}

	ret[0] = fopen("A8-dep.dat","w+b");
	ret[1] = fopen("A5-Nome-dep.dat","w+b");
	ret[2] = fopen("A5-Idade-dep.dat","w+b");

	int ant, j;

	InvertidoDep* regs[qtd_registros];

	rewind(dados);//registros

	for (int i = 0; i < qtd_registros; ++i){ //lê todos os dados para a memoria principal
		regs[i] = criaInvertidoDep();
		regs[i]->ED = i;
		regs[i]->dep = le_dep(dados);
		if(regs[i]->dep->status = 1){
			imprime_dep(regs[i]->dep);
		}
	}
	
	//NOME 1-----------------------------------------
	qsort_nomeDep(regs, qtd_registros); //Ordena por nome

	for(int i=0; i<qtd_registros; i++){

		if(i < qtd_registros-1 && regs[i]->dep->status == 1){
			ant=i;
			j=i+1;
			while(j<qtd_registros && strcmp(regs[i]->dep->nome, regs[j]->dep->nome) == 0){
				if(regs[ant]->dep->status==1 && regs[j]->dep->status==1){
					regs[ant]->prox_nome = regs[j]->ED;
					regs[i]->QTD += 1;
					ant = j;
				}
				else{
					i=j;
					break;
				}
				j++;
			}
			fwrite(&regs[i]->dep->nome, sizeof(char), sizeof(regs[i]->dep->nome), ret[1]);
			fwrite(&regs[i]->ED, sizeof(int), 1, ret[1]);
			fwrite(&regs[i]->QTD, sizeof(int), 1, ret[1]);
			regs[i]->QTD = 1;
			i=j;
		}
		if(i<qtd_registros){
			if(regs[i]->dep->status==1){
				fwrite(&regs[i]->dep->nome, sizeof(char), sizeof(regs[i]->dep->nome), ret[1]);
				fwrite(&regs[i]->ED, sizeof(int), 1, ret[1]);
				fwrite(&regs[i]->QTD, sizeof(int), 1, ret[1]);
				regs[i]->QTD = 1;
			}
		}
	}
	
	//IDADE 2----------------------------------------
	qsort_idadeDep(regs, qtd_registros); //Ordena por idade

	for(int i=0; i<qtd_registros; i++){
		if(i < qtd_registros-1 && regs[i]->dep->status == 1){
			ant=i;
			j=i+1;
			while(j<qtd_registros && regs[i]->dep->idade == regs[j]->dep->idade){
				if(regs[ant]->dep->status==1 && regs[j]->dep->status==1){
					regs[ant]->prox_idade = regs[j]->ED;
					regs[i]->QTD += 1;
					ant = j;
				}
				else{
					i=j;
					break;
				}
				j++;
			}
			fwrite(&regs[i]->dep->idade, sizeof(int), 1, ret[2]);
			fwrite(&regs[i]->ED, sizeof(int), 1, ret[2]);
			fwrite(&regs[i]->QTD, sizeof(int), 1, ret[2]);
			regs[i]->QTD = 1;
			i=j;
		}

		if(i<qtd_registros){
			if(regs[i]->dep->status==1){
				fwrite(&regs[i]->dep->idade, sizeof(int), 1, ret[2]);
				fwrite(&regs[i]->ED, sizeof(int), 1, ret[2]);
				fwrite(&regs[i]->QTD, sizeof(int), 1, ret[2]);
				regs[i]->QTD = 1;
			}
		}
	}

	qsort_cpDep(regs, qtd_registros);
	
	//A8
	for (int i = 0; i < qtd_registros; ++i){ 
		fwrite(&regs[i]->dep->cod, sizeof(int), 1, ret[0]);
		fwrite(regs[i]->dep->nome, sizeof(char), sizeof(regs[i]->dep->nome), ret[0]);
		fwrite(&regs[i]->dep->idade, sizeof(int), 1, ret[0]);
		fwrite(&regs[i]->prox_nome, sizeof(int), 1, ret[0]);//ponteiro p/ prox nome
		fwrite(&regs[i]->prox_idade, sizeof(int), 1, ret[0]);//ponteiro p/ prox idad	
	}

    return ret;
}
