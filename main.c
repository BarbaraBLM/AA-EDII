#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "empregado.h"
#include "dependente.h"
#include "hash.h"

int main(int argc, char *argv[]){
    int tamHash = atoi(argv[1]);
    int l = 0;
    int qtd_registros = 0;
    int p = 0;
    int aux = -1;
    int run = 1;
    int op;
    Empregado* emp_aux;

	FILE *hash = fopen("hash.dat","w+b");
	FILE *regts = fopen("registros.dat","w+b");
	FILE *excls = fopen("exclusao.dat","w+b");
	FILE *arq_invert_emp[5];

	initHash(hash, tamHash);
	fwrite(&aux, sizeof(int), 1, excls);

	while(run == 1){
		//Status
		int cod;
		char nome[50];
		int idade;
		double salario;
		int end;

		printf("1. Cadastrar Empregado\n2. Cadastrar Dependente\n3. Buscar\n4. Modificar\n5. Sair\nDigite uma opção: ");
		scanf("%d", &op);
		fflush(stdin);
		switch (op){
			case 1 :
				printf("Digite o nome do Empregado: ");
				scanf("%[^\n]", nome);
				fflush(stdin);
				printf("Idade do Empregado: ");
				scanf("%d", &idade);
				fflush(stdin);
				printf("Salario do Empregado: ");
				scanf("%lf", &salario);
				fflush(stdin);
				Empregado *emp = criarEmpregado(nome, idade, salario);
				inserirHash(hash, regts, excls, emp, tamHash, l, &qtd_registros);
				break;
			case 2 :
				printf("Não implementado ainda!\n");
				break;
			case 3 :
				printf("1. Buscar Empregado\n2. Buscar Dependente\n3. Voltar\nDigite uma opção: ");
				scanf("%d", &op);
				fflush(stdin);
				switch (op){
					case 1 :
						printf("1. Buscar Codigo\n2. Buscar Nome\n3. Buscar Idade\n4. Buscar Salario\n5. Buscar Número de Dependente\n6. Voltar\nDigite uma opção: ");
						scanf("%d", &op);
						fflush(stdin);
						switch (op){
							case 1 :
								printf("Digite o codigo do Empregado: ");
								scanf("%d", &cod);
								fflush(stdin);
								if((end = buscarCod(hash, regts, cod, tamHash, p, l)) != -1){
									printf("Empregado encotrado!\n1. Excluir\n2. Modificar\n3. Imprimir\n4. Voltar\nDigite uma opção: ");
									scanf("%d", &op);
									fflush(stdin);
									switch (op){
										case 1 :
											excluirHash(hash, regts, excls, end, tamHash, p, l);
											printf("Empregado Excluido!\n");
											break;
										case 2 :
											printf("Não implementado ainda!\n");
											break;
										case 3 :
											fseek(regts, end*tamanhoEmpregado(), SEEK_SET);
											emp_aux = le_empreg(regts);
											imprime_empreg(emp_aux);
											free(emp_aux);
											break;
										case 4 :
											break;
										default :
											printf("Opção invalida!\n");
									}
								}
								else
									printf("Empregado NÃO encontrado!\n");
								break;
							case 2 :
								printf("Não implementado ainda!\n");
								break;
							case 3 :
								printf("Não implementado ainda!\n");
								break;
							case 4 :
								printf("Não implementado ainda!\n");
								break;
							case 5 :
								printf("Não implementado ainda!\n");
								break;
							case 6 :
								break;
							default :
								printf("Opção invalida!\n");
						}
						break;
					case 2 :
						break;
					case 3 :
						break;
					default :
						printf("Opção invalida!\n");
				}
				break;
			case 4 :
				printf("Não implementado ainda!\n");
				break;
			case 5 :
				break;
			default :
				printf("Opção invalida!\n");
		}
	}

    return 0;
}
