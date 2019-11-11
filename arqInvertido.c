#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "dependente.c"
#include "empregado.c"

typedef struct invertido{
    int ED;
    //A8
    Empregado *emp;
    int prox_nome;
    int prox_idade;
    int prox_salario;
    int prox_n_dependentes;
    //A5
    int PT;
    int QTD;
}Invertido;

int cmp_nome(const void * a, const void * b){

	Invertido **a1 = (Invertido **) a;
	Invertido **b1 = (Invertido **) b;
	if(strcmp((*a1)->emp->nome, (*b1)->emp->nome) < 0)
		return -1;
	else if(strcmp((*a1)->emp->nome, (*b1)->emp->nome) > 0)
		return 1;
	else
		return 0;
}

Invertido* criaInvertido(){
	Invertido *novo = (Invertido *)malloc(sizeof(Invertido));
	novo->prox_nome = -1;
	novo->prox_idade = -1;
	novo->prox_salario = 1;
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

		qsort_ndependentes(regs,b);
		qsort_ndependentes(&regs[a], qtd_registros-a);
	}
}

FILE** arquivo_invert(FILE* dados, int qtd_registros){

	FILE *ret[5];
	ret[0] = fopen("A8.dat","w+b");
	ret[1] = fopen("A5-Nome.dat","w+b");
	ret[2] = fopen("A5-Idade.dat","w+b");
	ret[3] = fopen("A5-Salario.dat","w+b");
	ret[4] = fopen("A5-Ndependts.dat","w+b");

	Invertido* regs[qtd_registros];

	rewind(dados);//registros

	printf("Registros lidos...\n");
	for (int i = 0; i < qtd_registros; ++i){ //lê todos os dados para a memoria principal
		regs[i] = criaInvertido();
		regs[i]->ED = i;
		regs[i]->emp = le_empreg(dados);
	}
	
	//NOME 1-----------------------------------------
	qsort_nome(regs, qtd_registros); //Organiza por idade

	for (int i = 0; i < qtd_registros; ++i){ //Organiza os ponteiros e cria o A5-Nome
		if(i < (qtd_registros-1) && strcmp(regs[i]->emp->nome, regs[i+1]->emp->nome) == 0 && regs[i]->emp->status == 1 && regs[i+1]->emp->status == 1){
			for (int j = i+1; j < qtd_registros; ++j){
				if(strcmp(regs[i]->emp->nome, regs[j]->emp->nome) == 0 && regs[i]->emp->status == 1 && regs[j]->emp->status == 1){
					regs[i]->prox_nome = regs[j]->ED;
					regs[i]->QTD += 1;
				}
				else{
					i = j;
					break;
				}
			}
		}
		if(regs[i]->emp->status == 1){
			fwrite(regs[i]->emp->nome, sizeof(char), sizeof(regs[i]->emp->nome), ret[1]);
			fwrite(&regs[i]->ED, sizeof(int), 1, ret[1]);
			fwrite(&regs[i]->QTD, sizeof(int), 1, ret[1]);
		}
	}

	rewind(ret[1]);//A5-idade
	
	//PARA VERIFICAR RESULTADO
	char nome[50];
	int idade, ed, qtd, n_dependentes;

	printf("A5-NOME\n");
    while(fread(&nome, sizeof(char), sizeof(char)*50, ret[1]) != 0){

        fread(&ed, sizeof(int), 1, ret[1]);
        fread(&qtd, sizeof(int), 1, ret[1]);
        printf("IDADE:%s ED:%d Q:%d\n", nome, ed, qtd);
    }
    printf("\n\n");
    
	//IDADE 2----------------------------------------
	qsort_idade(regs, qtd_registros); //Organiza por idade

	for (int i = 0; i < qtd_registros; ++i){ //Organiza os ponteiros e cria o A5-idade
		if(i < (qtd_registros-1) && regs[i]->emp->idade == regs[i+1]->emp->idade && regs[i]->emp->status == 1 && regs[i+1]->emp->status == 1){
			for (int j = i+1; j < qtd_registros; ++j){
				if(regs[i]->emp->idade == regs[j]->emp->idade && regs[i]->emp->status == 1 && regs[j]->emp->status == 1){
					regs[i]->prox_idade = regs[j]->ED;
					regs[i]->QTD += 1;
				}
				else{
					if(regs[i]->emp->status==1){
						fwrite(&regs[i]->emp->idade, sizeof(int), 1, ret[2]);
						fwrite(&regs[i]->ED, sizeof(int), 1, ret[2]);
						fwrite(&regs[i]->QTD, sizeof(int), 1, ret[2]);
					}
					i = j;
					break;
				}
			}
		}
		if(regs[i]->emp->status==1){
			fwrite(&regs[i]->emp->idade, sizeof(int), 1, ret[2]);
			fwrite(&regs[i]->ED, sizeof(int), 1, ret[2]);
			fwrite(&regs[i]->QTD, sizeof(int), 1, ret[2]);
		}
	}

	rewind(ret[2]);//A5-idade

	//PARA VERIFICAR RESULTADO
	printf("A5-IDADE\n");
    while(fread(&idade, sizeof(int), 1, ret[2]) != 0){

        fread(&ed, sizeof(int), 1, ret[2]);
        fread(&qtd, sizeof(int), 1, ret[2]);
        printf("IDADE:%d ED:%d Q:%d\n", idade, ed, qtd);
    }
    printf("\n\n");

    //SALARIOOOOOOOOO 3----------------------------------
    qsort_salario(regs, qtd_registros); //Organiza por Salario

	for (int i = 0; i < qtd_registros; ++i){ //Organiza os ponteiros e cria o A5-SALARIO
		if(i < (qtd_registros-1) && regs[i]->emp->salario == regs[i+1]->emp->salario && regs[i]->emp->status == 1 && regs[i+1]->emp->status == 1){
			for (int j = i+1; j < qtd_registros; ++j){
				if(regs[i]->emp->salario == regs[j]->emp->salario && regs[i]->emp->status == 1 && regs[j]->emp->status == 1){
					regs[i]->prox_salario = regs[j]->ED;
					regs[i]->QTD += 1;
				}
				else{
					i = j;
					break;
				}
			}
		}
		if(regs[i]->emp->status==1){
			fwrite(&regs[i]->emp->salario, sizeof(double), 1, ret[3]);
			fwrite(&regs[i]->ED, sizeof(int), 1, ret[3]);
			fwrite(&regs[i]->QTD, sizeof(int), 1, ret[3]);
		}
	}

	rewind(ret[3]);//A5-salario

	//PARA VERIFICAR RESULTADO
	double salario;
	printf("A5-SALARIO\n");
    while(fread(&salario, sizeof(double), 1, ret[3]) != 0){

        fread(&ed, sizeof(int), 1, ret[3]);
        fread(&qtd, sizeof(int), 1, ret[3]);
        printf("SALARIO:%lf ED:%d Q:%d\n", salario, ed, qtd);
    }
    printf("\n\n");

    //DEPENDENTES 4-----------------------------------
    qsort_ndependentes(regs, qtd_registros); //Organiza por Salario

	for (int i = 0; i < qtd_registros; ++i){ //Organiza os ponteiros e cria o A5-SALARIO
		if(i < (qtd_registros-1) && regs[i]->emp->n_dependentes == regs[i+1]->emp->n_dependentes && regs[i]->emp->status == 1 && regs[i+1]->emp->status == 1){
			for (int j = i+1; j < qtd_registros; ++j){
				if(regs[i]->emp->n_dependentes == regs[j]->emp->n_dependentes && regs[i]->emp->status == 1 && regs[j]->emp->status == 1){
					regs[i]->prox_n_dependentes = regs[j]->ED;
					regs[i]->QTD += 1;
				}
				else {
					i = j;
					break;
				}
			}
		}
		if(regs[i]->emp->status==1){
			fwrite(&regs[i]->emp->n_dependentes, sizeof(int), 1, ret[4]);
			fwrite(&regs[i]->ED, sizeof(int), 1, ret[4]);
			fwrite(&regs[i]->QTD, sizeof(int), 1, ret[4]);
		}
	}

	rewind(ret[3]);//A5-salario

	//PARA VERIFICAR RESULTADO
	printf("A5-Ndependts\n");
    while(fread(&n_dependentes, sizeof(int), 1, ret[3]) != 0){

        fread(&ed, sizeof(int), 1, ret[3]);
        fread(&qtd, sizeof(int), 1, ret[3]);
        printf("NDEPENDENTS:%d ED:%d Q:%d\n", n_dependentes, ed, qtd);
    }
    printf("\n\n");


	qsort_cp(regs, qtd_registros);
	
	//A8
	for (int i = 0; i < qtd_registros; ++i){ //lê todos os dados para a memoria principal
		fwrite(&regs[i]->emp->cod, sizeof(int), 1, ret[0]);
		fwrite(regs[i]->emp->nome, sizeof(char), sizeof(regs[i]->emp->nome), ret[0]);
		fwrite(&regs[i]->emp->idade, sizeof(int), 1, ret[0]);
		fwrite(&regs[i]->emp->salario, sizeof(double), 1, ret[0]);
		fwrite(&regs[i]->emp->n_dependentes, sizeof(int), 1, ret[0]);
		fwrite(&regs[i]->prox_nome, sizeof(int), 1, ret[0]);//ponteiro p/ prox nome
		fwrite(&regs[i]->prox_idade, sizeof(int), 1, ret[0]);//ponteiro p/ prox idade
		fwrite(&regs[i]->prox_salario, sizeof(int), 1, ret[0]);//ponteiro p/ prox salario
		fwrite(&regs[i]->prox_n_dependentes, sizeof(int), 1, ret[0]);//ponteiro p/ prox n_dependentes
	}

    for(int i=0; i<5; i++){
    	fclose(ret[i]);
    }
}

int main(){

	int qtsRegistros;
	FILE *regs;

	if((regs = fopen("registros.dat","rb")) == NULL) //abre somente para leitura
	{
		printf("Erro ao abrir arquivo: registros.dat\n");
		exit(1);
	}

	printf("Informe a quantidade de registros no arquivo de dados:");
	scanf("%d",&qtsRegistros);

	arquivo_invert(regs, qtsRegistros);

	fclose(regs);

	return 0;
}
