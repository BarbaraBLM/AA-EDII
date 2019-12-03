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
	novo->prox_cod_emp = -1;
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

void qsort_CodEmpDep(InvertidoDep *regs[], int qtd_registros){

	if(qtd_registros>1){

		InvertidoDep *x = regs[0];
		int a = 1;
		int b = qtd_registros-1;

		do{
			while(a<qtd_registros && regs[a]->dep->cod_emp <= x->dep->cod_emp) a++;
			while(regs[b]->dep->cod_emp > x->dep->cod_emp) b--;
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

		qsort_CodEmpDep(regs,b);
		qsort_CodEmpDep(&regs[a], qtd_registros-a);
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

	FILE** ret = malloc(4*sizeof(FILE*));

	for(int i=0; i< 4; i++){
		ret[i] =  malloc(sizeof(FILE*));
	}

	ret[0] = fopen("A8-dep.dat","w+b");
	ret[1] = fopen("A5-Nome-dep.dat","w+b");
	ret[2] = fopen("A5-Idade-dep.dat","w+b");
	ret[3] = fopen("A5-Cod_emp-dep.dat","w+b");

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
			fwrite(&regs[i]->dep->nome, sizeof(char), 50, ret[1]);
			fwrite(&regs[i]->ED, sizeof(int), 1, ret[1]);
			fwrite(&regs[i]->QTD, sizeof(int), 1, ret[1]);
			regs[i]->QTD = 1;
			i=j;
		}
		if(i<qtd_registros){
			if(regs[i]->dep->status==1){
				fwrite(&regs[i]->dep->nome, sizeof(char), 50, ret[1]);
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

	//Codido empregado 3----------------------------------------
	qsort_CodEmpDep(regs, qtd_registros); //Ordena por cod_emp

	for(int i=0; i<qtd_registros; i++){
		if(i < qtd_registros-1 && regs[i]->dep->status == 1){
			ant=i;
			j=i+1;
			while(j<qtd_registros && regs[i]->dep->cod_emp == regs[j]->dep->cod_emp){
				if(regs[ant]->dep->status==1 && regs[j]->dep->status==1){
					regs[ant]->prox_cod_emp = regs[j]->ED;
					regs[i]->QTD += 1;
					ant = j;
				}
				else{
					i=j;
					break;
				}
				j++;
			}
			fwrite(&regs[i]->dep->cod_emp, sizeof(int), 1, ret[3]);
			fwrite(&regs[i]->ED, sizeof(int), 1, ret[3]);
			fwrite(&regs[i]->QTD, sizeof(int), 1, ret[3]);
			regs[i]->QTD = 1;
			i=j;
		}

		if(i<qtd_registros){
			if(regs[i]->dep->status==1){
				fwrite(&regs[i]->dep->cod_emp, sizeof(int), 1, ret[3]);
				fwrite(&regs[i]->ED, sizeof(int), 1, ret[3]);
				fwrite(&regs[i]->QTD, sizeof(int), 1, ret[3]);
				regs[i]->QTD = 1;
			}
		}
	}

	qsort_cpDep(regs, qtd_registros);
	
	//A8 cod - nome - idade - cod_emp - prox_nome - prox_idade - prox_cod_emp
	for (int i = 0; i < qtd_registros; ++i){ 
		fwrite(&regs[i]->dep->cod, sizeof(int), 1, ret[0]);
		fwrite(regs[i]->dep->nome, sizeof(char), 50, ret[0]);
		fwrite(&regs[i]->dep->idade, sizeof(int), 1, ret[0]);
		fwrite(&regs[i]->dep->cod_emp, sizeof(int), 1, ret[0]);
		fwrite(&regs[i]->prox_nome, sizeof(int), 1, ret[0]);//ponteiro p/ prox nome
		fwrite(&regs[i]->prox_idade, sizeof(int), 1, ret[0]);//ponteiro p/ prox idad
		fwrite(&regs[i]->prox_cod_emp, sizeof(int), 1, ret[0]);//ponteiro p/ prox cod_emp
	}

    return ret;
}


void imprimeA8Dep(FILE** arqsInv, int qtd_registros){

	int cod;
	char nome[50];
	int idade = 0;
	int prox_nome, prox_idade, prox_cod_emp, cod_emp;
	int ed;

	rewind(arqsInv[0]);//A8
    
    printf("A8 DEPENDENTE:\n");
	for (int i = 0; i < qtd_registros; ++i){ 

		fread(&cod, sizeof(int), 1, arqsInv[0]);
		fread(nome, sizeof(char), 50, arqsInv[0]);
		fread(&idade, sizeof(int), 1, arqsInv[0]);
		fread(&cod_emp, sizeof(int), 1, arqsInv[0]);
		fread(&prox_nome, sizeof(int), 1, arqsInv[0]);//ponteiro p/ prox nome
		fread(&prox_idade, sizeof(int), 1, arqsInv[0]);//ponteiro p/ prox idad
		fread(&prox_cod_emp, sizeof(int), 1, arqsInv[0]);//ponteiro p/ prox cod_emp

		printf("cod %d nome %s idade %d ndep %d cod_emp %d prox_idade %d prox_cod_emp %d\n", cod, nome, idade, cod_emp, prox_nome, prox_idade, prox_cod_emp);
	}
}


Dependente** buscaIdadeMaiorQueXDep(FILE** arqsInv, int x, int *qtdTotal){

	//qtdTotal: quantidade de registros relacionados à busca
	//arqsInv[2] --> A5-Idade.dat:   IDADE, PT, QTD
	printf("BUSCA IDADE DEPENDENTE\n");

    int pt=0, qtd=0, prox_idade=0, /*qtdTotal=0, */pos=0;
    int idade=0;
    rewind(arqsInv[0]);//A8
    rewind(arqsInv[2]);//A5-IDADE

    //printf("while 1\n");
    while(fread(&idade, sizeof(int), 1, arqsInv[2]) > 0){

    	fread(&pt, sizeof(int), 1, arqsInv[2]);
    	fread(&qtd, sizeof(int), 1, arqsInv[2]);

    	if(idade>x){
    		//quatidade total de resultados para o tamanho do vetor de resultados
    		*qtdTotal += qtd;
    	}
    }

    Dependente** dependentes = malloc((*qtdTotal)*sizeof(Dependente*));
	for(int i=0; i< *qtdTotal; i++){
		dependentes[i] = (Dependente *) malloc(sizeof(Dependente)); //malloc(sizeof(Empregado*));
	}

    rewind(arqsInv[2]);//A5-IDADE

    while(fread(&idade, sizeof(int), 1, arqsInv[2]) > 0){

    	fread(&pt, sizeof(int), 1, arqsInv[2]);
    	fread(&qtd, sizeof(int), 1, arqsInv[2]);

    	if(idade>x){

    		fseek(arqsInv[0], (pt)*(tamanhoDependente() + sizeof(int)) , SEEK_SET);

	    	//lendo todos os registros que atendem ao critério da busca
	    	for(int i=0; i<qtd; i++){
	    		fread(&dependentes[pos]->cod, sizeof(int), 1, arqsInv[0]);
	            fread(dependentes[pos]->nome, sizeof(char), 50, arqsInv[0]);
	            fread(&dependentes[pos]->idade, sizeof(int), 1, arqsInv[0]);
	            fread(&dependentes[pos]->cod_emp, sizeof(int), 1, arqsInv[0]);

	            //pula o prox_nome
	            fseek(arqsInv[0],sizeof(int), SEEK_CUR);

	            fread(&prox_idade, sizeof(int), 1, arqsInv[0]);
	            //pula o prox_cod_emp
	            fseek(arqsInv[0], sizeof(int), SEEK_CUR); 
	          	fseek(arqsInv[0], (prox_idade)*(tamanhoDependente() + sizeof(int)), SEEK_SET);    //Indo para a prox idade
	            //posição do vetor de empregados que será retornado
	            pos++;
	    	}
    	}
    }
    printf("FIM BUSCA IDADE DEPENDENTE\n");

    if(*qtdTotal!=0){

    	return dependentes;
    }

    return NULL;
}


//retorna um vetor com os dependentes de uma chave
Dependente** buscaDependentesEmpDep(FILE** arqsInv, int x, int *qtdTotal){

	//qtdTotal: quantidade de registros relacionados à busca
	//arqsInv[2] --> A5-Idade.dat:   IDADE, PT, QTD
	printf("BUSCA DEPENDENTES DE UM EMPREGADO\n");

    int pt=0, qtd=0, prox_cod_emp=0, /*qtdTotal=0, */pos=0;
    int cod_emp=0;
    rewind(arqsInv[0]);//A8
    rewind(arqsInv[3]);//A5-IDADE

    //printf("while 1\n");
    while(fread(&cod_emp, sizeof(int), 1, arqsInv[3]) > 0){

    	fread(&pt, sizeof(int), 1, arqsInv[3]);
    	fread(&qtd, sizeof(int), 1, arqsInv[3]);

    	if(cod_emp==x){

    		//quatidade total de resultados para o tamanho do vetor de resultados
    		*qtdTotal = qtd;
    		break;
    	}
    }

    Dependente** dependentes = malloc((*qtdTotal)*sizeof(Dependente*));
    for(int i=0; i< *qtdTotal; i++){
		dependentes[i] = (Dependente *) malloc(sizeof(Dependente)); //malloc(sizeof(Empregado*));
	}

	fseek(arqsInv[0], (pt)*(tamanhoDependente() + sizeof(int)) , SEEK_SET);

	//lendo todos os registros que atendem ao critério da busca
	for(int i=0; i<qtd; i++){
	    fread(&dependentes[pos]->cod, sizeof(int), 1, arqsInv[0]);
	    fread(dependentes[pos]->nome, sizeof(char), 50, arqsInv[0]);
	    fread(&dependentes[pos]->idade, sizeof(int), 1, arqsInv[0]);
	    fread(&dependentes[pos]->cod_emp, sizeof(int), 1, arqsInv[0]);

	    //pula o prox_nome, prox_idade
	    fseek(arqsInv[0],2*sizeof(int), SEEK_CUR);

	    fread(&prox_cod_emp, sizeof(int), 1, arqsInv[0]);

	    fseek(arqsInv[0], (prox_cod_emp)*(tamanhoDependente() + sizeof(int)), SEEK_SET);    //Indo para a prox idade
	    //posição do vetor de empregados que será retornado
	    pos++;
	}

    printf("FIM BUSCA DEPENDENTES DE UM EMPREGADO\n");

    if(*qtdTotal!=0){

    	return dependentes;
    }

    return NULL;
}
