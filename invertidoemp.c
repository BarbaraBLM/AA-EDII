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
		fwrite(&regs[i]->status, sizeof(int), 1, ret[0]);//status
	}

    return ret;
}

Empregado** buscaNome(FILE** arqsInv, char* nome){
    //arqsInv[1] --> A5-Nome.dat    nome, ED, QTD
    int ed=0, qtd=0, prox_nome=0;
    char n[50];
    rewind(arqsInv[1]);
    
    if(fread(n, sizeof(char), 50, arqsInv[1]) > 0){
        printf("n = %s", n);
        while(strcmp(n, nome) != 0){    //n são iguais
            fseek(arqsInv[1], 2*sizeof(int), SEEK_CUR);    //pulando o ED e QTD
            fread(n, 50*sizeof(char), 1, arqsInv[1]);
        }
        // Achou ou acabou    ???    ???
        fread(&ed, sizeof(int), 1, arqsInv[1]);    //lendo ed
        fread(&qtd, sizeof(int), 1, arqsInv[1]);    //lendo qtd
        fseek(arqsInv[0], (ed-1)*(tamanhoEmpregado() + 4*sizeof(int)), SEEK_SET);

        Empregado** e = (Empregado**) malloc(qtd*sizeof(Empregado*));
        for(int i=0;i<qtd;i++){
            fread(&e[i]->cod, sizeof(int), 1, arqsInv[0]);
            fread(e[i]->nome, sizeof(char), 50, arqsInv[0]);
            fread(&e[i]->idade, sizeof(int), 1, arqsInv[0]);
            fread(&e[i]->salario, sizeof(float), 1, arqsInv[0]);
            fread(&e[i]->n_dependentes, sizeof(int), 1, arqsInv[0]);
            fread(&prox_nome, sizeof(int), 1, arqsInv[0]);
            fseek(arqsInv[0], 3*sizeof(int), SEEK_CUR);    //pulando prox_idade, prox_salario, prox_dependentes
            fseek(arqsInv[0], (prox_nome-1)*(tamanhoEmpregado() + 4*sizeof(int)), SEEK_SET);    //Indo para o prox nome
        }
        printf("Antes do return    -----    -----\n\n");
        return e;
    }
    return NULL;
}
