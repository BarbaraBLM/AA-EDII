#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "empregado.h"
#include "dependente.h"
#include "invertidoemp.h"

Invertido* criaInvertido(){
	Invertido *novo = (Invertido *)malloc(sizeof(Invertido));
	novo->prox_nome = -1;
	novo->prox_idade = -1;
	novo->prox_salario = -1;
	novo->prox_n_dependentes = -1;
	novo->PT = -1;
	novo->QTD = 1;

	return novo;
}

void qsort_nome(Invertido *regs[], int qtd_registros){

	if(qtd_registros>1){

		Invertido *x = regs[0];
		int a = 1;
		int b = qtd_registros-1;

		do{
			while(a<qtd_registros && strcmp(regs[a]->emp->nome,x->emp->nome)<=0) a++;
			while(strcmp(regs[b]->emp->nome,x->emp->nome)>0) b--;
			if(a<b){
				Invertido *temp = regs[a];
				regs[a] = regs[b];
				regs[b] = temp;
				a++;
				b--;
			}
		}while(a<=b);

		regs[0] = regs[b];
		regs[b] = x;

		qsort_nome(regs,b);
		qsort_nome(&regs[a], qtd_registros-a);
	}
}

void qsort_idade(Invertido *regs[], int qtd_registros){

	if(qtd_registros>1){

		Invertido *x = regs[0];
		int a = 1;
		int b = qtd_registros-1;

		do{
			while(a<qtd_registros && regs[a]->emp->idade <= x->emp->idade) a++;
			while(regs[b]->emp->idade > x->emp->idade) b--;
			if(a<b){
				Invertido *temp = regs[a];
				regs[a] = regs[b];
				regs[b] = temp;
				a++;
				b--;
			}
		}while(a<=b);

		regs[0] = regs[b];
		regs[b] = x;

		qsort_idade(regs,b);
		qsort_idade(&regs[a], qtd_registros-a);
	}
}

void qsort_salario(Invertido *regs[], int qtd_registros){

	if(qtd_registros>1){

		Invertido *x = regs[0];
		int a = 1;
		int b = qtd_registros-1;

		do{
			while(a<qtd_registros && regs[a]->emp->salario <= x->emp->salario) a++;
			while(regs[b]->emp->salario > x->emp->salario) b--;
			if(a<b){
				Invertido *temp = regs[a];
				regs[a] = regs[b];
				regs[b] = temp;
				a++;
				b--;
			}
		}while(a<=b);

		regs[0] = regs[b];
		regs[b] = x;

		qsort_salario(regs,b);
		qsort_salario(&regs[a], qtd_registros-a);
	}
}

void qsort_ndependentes(Invertido *regs[], int qtd_registros){

	if(qtd_registros>1){

		Invertido *x = regs[0];
		int a = 1;
		int b = qtd_registros-1;

		do{
			while(a<qtd_registros && regs[a]->emp->n_dependentes <= x->emp->n_dependentes) a++;
			while(regs[b]->emp->n_dependentes > x->emp->n_dependentes) b--;
			if(a<b){
				Invertido *temp = regs[a];
				regs[a] = regs[b];
				regs[b] = temp;
				a++;
				b--;
			}
		}while(a<=b);

		regs[0] = regs[b];
		regs[b] = x;

		qsort_ndependentes(regs,b);
		qsort_ndependentes(&regs[a], qtd_registros-a);
	}
}

void qsort_cp(Invertido *regs[], int qtd_registros){

	if(qtd_registros>1){

		Invertido *x = regs[0];
		int a = 1;
		int b = qtd_registros-1;

		do{
			while(a<qtd_registros && regs[a]->ED <= x->ED) a++;
			while(regs[b]->ED > x->ED) b--;
			if(a<b){
				Invertido *temp = regs[a];
				regs[a] = regs[b];
				regs[b] = temp;
				a++;
				b--;
			}
		}while(a<=b);

		regs[0] = regs[b];
		regs[b] = x;

		qsort_cp(regs,b);
		qsort_cp(&regs[a], qtd_registros-a);
	}
}

FILE** arquivo_invertido_emp(FILE* dados, int qtd_registros){
    printf("Em arquivo_invertido    -----    -----\n\n");
    FILE** ret = malloc(5*sizeof(FILE*));

	for(int i=0; i< 5; i++){
		ret[i] =  malloc(sizeof(FILE*));
	}

	ret[0] = fopen("A8.dat","w+b");
	ret[1] = fopen("A5-Nome.dat","w+b");
	ret[2] = fopen("A5-Idade.dat","w+b");
	ret[3] = fopen("A5-Salario.dat","w+b");
	ret[4] = fopen("A5-Ndependts.dat","w+b");

	int ant, j;

	Invertido* regs[qtd_registros];

	rewind(dados);//registros

	for (int i = 0; i < qtd_registros; ++i){ //lê todos os dados para a memoria principal
		regs[i] = criaInvertido();
		regs[i]->ED = i;
		regs[i]->emp = le_empreg(dados);
	}
	
	//NOME 1-----------------------------------------
	qsort_nome(regs, qtd_registros); //Ordena por nome

	for(int i=0; i<qtd_registros; i++){

		if(i < qtd_registros-1 && regs[i]->emp->status == 1){
			ant=i;
			j=i+1;
			while(j<qtd_registros && strcmp(regs[i]->emp->nome, regs[j]->emp->nome) == 0){
				if(regs[ant]->emp->status==1 && regs[j]->emp->status==1){
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
			fwrite(&regs[i]->emp->nome, sizeof(char), sizeof(regs[i]->emp->nome), ret[1]);
			fwrite(&regs[i]->ED, sizeof(int), 1, ret[1]);
			fwrite(&regs[i]->QTD, sizeof(int), 1, ret[1]);
			regs[i]->QTD = 1;
			i=j;
		}
		if(i<qtd_registros){
			if(regs[i]->emp->status==1){
				fwrite(&regs[i]->emp->nome, sizeof(char), sizeof(regs[i]->emp->nome), ret[1]);
				fwrite(&regs[i]->ED, sizeof(int), 1, ret[1]);
				fwrite(&regs[i]->QTD, sizeof(int), 1, ret[1]);
				regs[i]->QTD = 1;
			}
		}
	}
	
	//IDADE 2----------------------------------------
	qsort_idade(regs, qtd_registros); //Ordena por idade

	for(int i=0; i<qtd_registros; i++){
		if(i < qtd_registros-1 && regs[i]->emp->status == 1){
			ant=i;
			j=i+1;
			while(j<qtd_registros && regs[i]->emp->idade == regs[j]->emp->idade){
				if(regs[ant]->emp->status==1 && regs[j]->emp->status==1){
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
			fwrite(&regs[i]->emp->idade, sizeof(int), 1, ret[2]);
			fwrite(&regs[i]->ED, sizeof(int), 1, ret[2]);
			fwrite(&regs[i]->QTD, sizeof(int), 1, ret[2]);
			regs[i]->QTD = 1;
			i=j;
		}

		if(i<qtd_registros){
			if(regs[i]->emp->status==1){
				fwrite(&regs[i]->emp->idade, sizeof(int), 1, ret[2]);
				fwrite(&regs[i]->ED, sizeof(int), 1, ret[2]);
				fwrite(&regs[i]->QTD, sizeof(int), 1, ret[2]);
				regs[i]->QTD = 1;
			}
		}
	}


    //SALARIO 3----------------------------------
    qsort_salario(regs, qtd_registros); //Ordena por Salario

	for(int i=0; i<qtd_registros; i++){
		if(i < qtd_registros-1 && regs[i]->emp->status == 1){
			ant=i;
			j=i+1;
			while(j<qtd_registros && regs[i]->emp->salario == regs[j]->emp->salario){
				if(regs[ant]->emp->status==1 && regs[j]->emp->status==1){
					regs[ant]->prox_salario = regs[j]->ED;
					regs[i]->QTD += 1;
					ant = j;
				}
				else{
					i=j;
					break;
				}
				j++;
			}
			fwrite(&regs[i]->emp->salario, sizeof(double), 1, ret[3]);
			fwrite(&regs[i]->ED, sizeof(int), 1, ret[3]);
			fwrite(&regs[i]->QTD, sizeof(int), 1, ret[3]);
			regs[i]->QTD = 1;
			i=j;
		}
		if(i<qtd_registros){
			if(regs[i]->emp->status==1){
				fwrite(&regs[i]->emp->salario, sizeof(double), 1, ret[3]);
				fwrite(&regs[i]->ED, sizeof(int), 1, ret[3]);
				fwrite(&regs[i]->QTD, sizeof(int), 1, ret[3]);
				regs[i]->QTD = 1;
			}
		}
	}

    //DEPENDENTES 4-----------------------------------
    qsort_ndependentes(regs, qtd_registros); //Organiza por dependentes

	for(int i=0; i<qtd_registros; i++){
		if(i < qtd_registros-1 && regs[i]->emp->status == 1){
			ant=i;
			j=i+1;
			while(j<qtd_registros && regs[i]->emp->n_dependentes == regs[j]->emp->n_dependentes){
				if(regs[ant]->emp->status==1 && regs[j]->emp->status==1){
					regs[ant]->prox_n_dependentes = regs[j]->ED;
					regs[i]->QTD += 1;
					ant = j;
				}
				else{
					i=j;
					break;
				}
				j++;
			}
			fwrite(&regs[i]->emp->n_dependentes, sizeof(int), 1, ret[4]);
			fwrite(&regs[i]->ED, sizeof(int), 1, ret[4]);
			fwrite(&regs[i]->QTD, sizeof(int), 1, ret[4]);
			regs[i]->QTD = 1;
			i=j;
		}
		if(i<qtd_registros){
			if(regs[i]->emp->status==1){
				fwrite(&regs[i]->emp->n_dependentes, sizeof(int), 1, ret[4]);
				fwrite(&regs[i]->ED, sizeof(int), 1, ret[4]);
				fwrite(&regs[i]->QTD, sizeof(int), 1, ret[4]);
				regs[i]->QTD = 1;
			}
		}
	}

	qsort_cp(regs, qtd_registros);
	
	//A8
	for (int i = 0; i < qtd_registros; ++i){ 
		fwrite(&regs[i]->emp->cod, sizeof(int), 1, ret[0]);
		fwrite(regs[i]->emp->nome, sizeof(char), sizeof(regs[i]->emp->nome), ret[0]);
		fwrite(&regs[i]->emp->idade, sizeof(int), 1, ret[0]);
		fwrite(&regs[i]->emp->salario, sizeof(double), 1, ret[0]);
		fwrite(&regs[i]->emp->n_dependentes, sizeof(int), 1, ret[0]);
		fwrite(&regs[i]->prox_nome, sizeof(int), 1, ret[0]);//ponteiro p/ prox nome
		fwrite(&regs[i]->prox_idade, sizeof(int), 1, ret[0]);//ponteiro p/ prox idad		
		fwrite(&regs[i]->prox_salario, sizeof(int), 1, ret[0]);//ponteiro p/ prox salario
		fwrite(&regs[i]->prox_n_dependentes, sizeof(int), 1, ret[0]);//ponteiro p/ prox n_dependentes
	}

    return ret;
}

void imprimeA8(FILE** arqsInv, int qtd_registros){

	int cod;
	char nome[50], nome2[50];
	int idade = 0;
	double salario;
	int n_dependentes, prox_nome, prox_idade, prox_salario, prox_n_dependentes;
	int end;
	int ed;
	int qtd;

	rewind(arqsInv[0]);//A8
    
	for (int i = 0; i < 5; ++i){ 

		fread(&cod, sizeof(int), 1, arqsInv[0]);
		fread(nome, sizeof(char), sizeof(char)*50, arqsInv[0]);
		fread(&idade, sizeof(int), 1, arqsInv[0]);
		fread(&salario, sizeof(double), 1, arqsInv[0]);
		fread(&n_dependentes, sizeof(int), 1, arqsInv[0]);
		fread(&prox_nome, sizeof(int), 1, arqsInv[0]);//ponteiro p/ prox nome
		fread(&prox_idade, sizeof(int), 1, arqsInv[0]);//ponteiro p/ prox idad		
		fread(&prox_salario, sizeof(int), 1, arqsInv[0]);//ponteiro p/ prox salario
		fread(&prox_n_dependentes, sizeof(int), 1, arqsInv[0]);//ponteiro p/ prox n_dependentes
		printf("cod %d nome %s idade %d sal %lf ndep %d pnome %d pidade %d psal %d pndep %d\n", cod, nome, idade, salario, n_dependentes, prox_nome, prox_idade, prox_salario, prox_n_dependentes);
	}
}

Empregado** buscaNome(FILE** arqsInv, char* nome, int *qtdTotal){
    //arqsInv[1] --> A5-Nome.dat    nome, ED, QTD
    printf("BUSCA NOME\n");

    int pt=0, qtd=0, prox_nome=0, /*qtdTotal=0, */pos=0;
    char n[50];
    rewind(arqsInv[0]);//A8
    rewind(arqsInv[1]);//A5-NOME

    //printf("while 1\n");
    while(fread(n, sizeof(char), 50, arqsInv[1]) > 0){
    	fread(&pt, sizeof(int), 1, arqsInv[1]);    //lendo ponteiro
    	fread(&qtd, sizeof(int), 1, arqsInv[1]);    //lendo qtd

    	if(strcmp(nome, n) == 0){
    		//quatidade total de resultados para o tamanho do vetor de resultados
    		*qtdTotal += qtd;
    	}
    }
    
    Empregado** empregados = malloc((*qtdTotal)*sizeof(Empregado*));
	for(int i=0; i< qtd; i++){
		empregados[i] = (Empregado *) malloc(sizeof(Empregado)); //malloc(sizeof(Empregado*));
	}

    rewind(arqsInv[1]);//A5-NOME

    while(fread(n, sizeof(char), 50, arqsInv[1]) > 0){
    	fread(&pt, sizeof(int), 1, arqsInv[1]);
    	fread(&qtd, sizeof(int), 1, arqsInv[1]);

    	if(strcmp(n, nome) == 0){
    		fseek(arqsInv[0], (pt)*(tamanhoEmpregado() + sizeof(int)) , SEEK_SET);

	    	//lendo todos os registros que atendem ao critério da busca
	    	for(int i=0; i<qtd; i++){
	    		fread(&empregados[pos]->cod, sizeof(int), 1, arqsInv[0]);
	            fread(empregados[pos]->nome, sizeof(char), 50, arqsInv[0]);
	            fread(&empregados[pos]->idade, sizeof(int), 1, arqsInv[0]);
	            fread(&empregados[pos]->salario, sizeof(double), 1, arqsInv[0]);
	            fread(&empregados[pos]->n_dependentes, sizeof(int), 1, arqsInv[0]);
                fread(&prox_nome, sizeof(int), 1, arqsInv[0]);
	            //pula o prox_idade, prox_salario, prox_n_dependentes
	            fseek(arqsInv[0],3*sizeof(int), SEEK_CUR);
	            
	          	fseek(arqsInv[0], (prox_nome)*(tamanhoEmpregado() + sizeof(int)), SEEK_SET);    //Indo para a prox nome
	            //posição do vetor de empregados que será retornado
	            pos++;
	    	}
    	}
    }
    printf("FIM BUSCA NOME\n");

    if(qtdTotal!=0){
    	return empregados;
    }
    return NULL;
}

Empregado** buscaIdadeMaiorQueX(FILE** arqsInv, int x, int *qtdTotal){
	//qtdTotal: quantidade de registros relacionados à busca
	//arqsInv[2] --> A5-Idade.dat:   IDADE, PT, QTD
	printf("BUSCA IDADE\n");

    int pt=0, qtd=0, prox_idade=0, /*qtdTotal=0, */pos=0;
    int idade=0;
    rewind(arqsInv[0]);//A8
    rewind(arqsInv[2]);//A5-IDADE

    //printf("while 1\n");
    while(fread(&idade, sizeof(int), 1, arqsInv[2]) != 0){

    	fread(&pt, sizeof(int), 1, arqsInv[2]);
    	fread(&qtd, sizeof(int), 1, arqsInv[2]);

    	if(idade>x){
    		//quatidade total de resultados para o tamanho do vetor de resultados
    		*qtdTotal += qtd;
    	}
    }


    Empregado** empregados = malloc((*qtdTotal)*sizeof(Empregado*));
	for(int i=0; i< 5; i++){
		empregados[i] = (Empregado *) malloc(sizeof(Empregado)); //malloc(sizeof(Empregado*));
	}

    rewind(arqsInv[2]);//A5-IDADE

    while(fread(&idade, sizeof(int), 1, arqsInv[2]) != 0){

    	fread(&pt, sizeof(int), 1, arqsInv[2]);
    	fread(&qtd, sizeof(int), 1, arqsInv[2]);

    	if(idade>x){

    		fseek(arqsInv[0], (pt)*(tamanhoEmpregado() + sizeof(int)) , SEEK_SET);

	    	//lendo todos os registros que atendem ao critério da busca
	    	for(int i=0; i<qtd; i++){
	    		fread(&empregados[pos]->cod, sizeof(int), 1, arqsInv[0]);
	            fread(empregados[pos]->nome, sizeof(char), 50, arqsInv[0]);
	            fread(&empregados[pos]->idade, sizeof(int), 1, arqsInv[0]);
	            fread(&empregados[pos]->salario, sizeof(double), 1, arqsInv[0]);
	            fread(&empregados[pos]->n_dependentes, sizeof(int), 1, arqsInv[0]);
	            //pula o prox_nome
	            fseek(arqsInv[0],sizeof(int), SEEK_CUR); 
	            fread(&prox_idade, sizeof(int), 1, arqsInv[0]);
	            //pula o prox_salario, prox_n_dependentes
	            fseek(arqsInv[0], 2*sizeof(int), SEEK_CUR); 
	          	fseek(arqsInv[0], (prox_idade)*(tamanhoEmpregado() + sizeof(int)), SEEK_SET);    //Indo para a prox idade
	            //posição do vetor de empregados que será retornado
	            pos++;
	    	}
    	}
    }
    printf("FIM BUSCA IDADE\n");

    if(*qtdTotal!=0){
    	printf("qtdTotal:%d\n", *qtdTotal);
    	return empregados;
    }

    printf("retornando NULL\n");
    return NULL;
}

//busca empregados que têm quantidade maior que X de dependentes
Empregado** buscaNumDepMaiorQueX(FILE** arqsInv, int x, int *qtdTotal){

	//arqsInv[4] --> A5-Ndependts.dat:   N_DEPENDENTS, PT, QTD
	printf("BUSCA N_DEPENDENTES\n");

    int pt=0, qtd=0, prox_n_dependentes=0, /*qtdTotal=0, */pos=0;
    int n_dependentes=0;
    rewind(arqsInv[0]);//A8
    rewind(arqsInv[4]);//A5-IDADE

    while(fread(&n_dependentes, sizeof(int), 1, arqsInv[4]) > 0){

    	fread(&pt, sizeof(int), 1, arqsInv[4]);
    	fread(&qtd, sizeof(int), 1, arqsInv[4]);

    	if(n_dependentes>x){
    		//quatidade total de resultados para o tamanho do vetor de resultados
    		*qtdTotal += qtd;
    	}
    }

    Empregado** empregados = malloc((*qtdTotal)*sizeof(Empregado*));
	for(int i=0; i< 5; i++){
		empregados[i] = (Empregado *) malloc(sizeof(Empregado)); //malloc(sizeof(Empregado*));
	}

    rewind(arqsInv[4]);//A5-N_DEPENDENTS

    while(fread(&n_dependentes, sizeof(int), 1, arqsInv[4]) > 0){

    	fread(&pt, sizeof(int), 1, arqsInv[4]);
    	fread(&qtd, sizeof(int), 1, arqsInv[4]);

    	if(n_dependentes>x){

    		fseek(arqsInv[0], (pt)*(tamanhoEmpregado() + sizeof(int)) , SEEK_SET);

	    	//lendo todos os registros que atendem ao critério da busca
	    	for(int i=0; i<qtd; i++){
	    		fread(&empregados[pos]->cod, sizeof(int), 1, arqsInv[0]);
	            fread(empregados[pos]->nome, sizeof(char), 50, arqsInv[0]);
	            fread(&empregados[pos]->idade, sizeof(int), 1, arqsInv[0]);
	            fread(&empregados[pos]->salario, sizeof(double), 1, arqsInv[0]);
	            fread(&empregados[pos]->n_dependentes, sizeof(int), 1, arqsInv[0]);

	            //pula o prox_nome, prox_salario, prox_n_dependentes
	            fseek(arqsInv[0], 3*sizeof(int), SEEK_CUR);

	            fread(&prox_n_dependentes, sizeof(int), 1, arqsInv[0]);

	            //pula o prox_salario, prox_n_dependentes
	            fseek(arqsInv[0], 2*sizeof(int), SEEK_CUR); 
	          	fseek(arqsInv[0], (prox_n_dependentes)*(tamanhoEmpregado() + sizeof(int)), SEEK_SET);    //Indo para a prox idade
	            //posição do vetor de empregados que será retornado
	            pos++;
	    	}
    	}
    }

    printf("FIM BUSCA N_DEPENDENTES\n");

    if(*qtdTotal!=0){

    	return empregados;
    }

    return NULL;
}


Empregado** buscaSalarioMaiorQueX(FILE** arqsInv, double x, int *qtdTotal){

	//arqsInv[3] --> A5-Salario.dat:   SALARIO, PT, QTD
	printf("BUSCA SALARIO\n");

    int pt=0, qtd=0, prox_salario=0, /*qtdTotal=0, */pos=0;
    double salario=0;
    rewind(arqsInv[0]);//A8
    rewind(arqsInv[3]);//A5-SALARIO

    while(fread(&salario, sizeof(double), 1, arqsInv[3]) > 0){

    	fread(&pt, sizeof(int), 1, arqsInv[3]);
    	fread(&qtd, sizeof(int), 1, arqsInv[3]);

    	if(salario>x){
    		//quatidade total de resultados para o tamanho do vetor de resultados
    		*qtdTotal += qtd;
    	}
    }

    Empregado** empregados = malloc((*qtdTotal)*sizeof(Empregado*));
	for(int i=0; i< 5; i++){
		empregados[i] = (Empregado *) malloc(sizeof(Empregado)); //malloc(sizeof(Empregado*));
	}

    rewind(arqsInv[3]);//A5-SALARIO

    while(fread(&salario, sizeof(double), 1, arqsInv[3]) > 0){

    	fread(&pt, sizeof(int), 1, arqsInv[3]);
    	fread(&qtd, sizeof(int), 1, arqsInv[3]);

    	if(salario>x){

    		fseek(arqsInv[0], (pt)*(tamanhoEmpregado() + sizeof(int)) , SEEK_SET);

	    	//lendo todos os registros que atendem ao critério da busca
	    	for(int i=0; i<qtd; i++){
	    		fread(&empregados[pos]->cod, sizeof(int), 1, arqsInv[0]);
	            fread(empregados[pos]->nome, sizeof(char), 50, arqsInv[0]);
	            fread(&empregados[pos]->idade, sizeof(int), 1, arqsInv[0]);
	            fread(&empregados[pos]->salario, sizeof(double), 1, arqsInv[0]);
	            fread(&empregados[pos]->n_dependentes, sizeof(int), 1, arqsInv[0]);

	            //pula o prox_nome, prox_idade
	            fseek(arqsInv[0], 2*sizeof(int), SEEK_CUR);

	            fread(&prox_salario, sizeof(int), 1, arqsInv[0]);

	            //pula o prox_n_dependentes
	            fseek(arqsInv[0], sizeof(int), SEEK_CUR); 
	          	fseek(arqsInv[0], (prox_salario)*(tamanhoEmpregado() + sizeof(int)), SEEK_SET);    //Indo para a prox idade
	            //posição do vetor de empregados que será retornado
	            pos++;
	    	}
    	}
    }

    printf("FIM BUSCA SALARIO\n");

    if(*qtdTotal!=0){

    	return empregados;
    }

    return NULL;
}
