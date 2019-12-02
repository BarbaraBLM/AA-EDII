//    ATENÇÃO : argumento de linha de comando = tamanho da Hash. Rode: .\exe tam
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "empregado.c"
#include "dependente.c"
#include "hash.c"
#include "invertidoemp.c"

int main(int argc, char *argv[]){
    int tamHash = atoi(argv[1]);
    int l = 0;
    int qtd_registros = 0;
    int p = 0;
    int aux = -1;
    int run = 1;
    int op, busca = 0;
    Empregado* emp_aux;
    Empregado** emps;
    Dependente* d;
    
    int cod;
	char nome[50], nome2[50];
	int idade = 0;
	double salario;
	int end;
    
	FILE *hash = fopen("hash.dat","w+b");
	FILE *regts = fopen("registros.dat","w+b");
	FILE *excls = fopen("exclusao.dat","w+b");
	FILE *arq_invert_emp[5];
    
    FILE** arqsInv = (FILE**) malloc(5*sizeof(FILE*));
    
	initHash(hash, tamHash);
	fwrite(&aux, sizeof(int), 1, excls);
	while(run == 1){
		//Status

		printf("1. Cadastrar Empregado\n2. Cadastrar Dependente\n3. Buscar\n4. Imprimir\n5. Sair\nDigite uma opção: ");
		scanf("%d", &op);
		fflush(stdin);
		switch (op){
			case 1 :    //Cadastrar empregado
				printf("Digite o nome do Empregado: ");
                scanf("%s[^\n]", nome);                
                fflush(stdin);
                printf("Idade do Empregado: ");
                scanf(" %d", &idade);
                fflush(stdin);
                printf("Salario do Empregado: ");
                scanf(" %lf", &salario);
                fflush(stdin);
                Empregado *emp = criarEmpregado(nome, idade, salario);
                inserirHashEmp(hash, regts, excls, emp, tamHash, p, l, &qtd_registros);
                printf("Empregado inserido!\n\n");       
				break;
			case 2 :    //Cadastrar dependente
                printf("Digite o nome do Dependente: ");
				scanf("%s[^\n]", nome);
				fflush(stdin);
				printf("Idade do Dependente: ");
				scanf(" %d", &idade);
				fflush(stdin);
                printf("Digite o nome do Empregado do qual é dependente: ");
                scanf("%s[^\n]", nome2);
				fflush(stdin);
                arqsInv = arquivo_invertido_emp(regts, qtd_registros);
                emps = buscaNome(arqsInv, nome2);
                if(emps != NULL){
                    // -----    -----    -----    SEG FAULT EM emps[i]    !!
                    int resul = sizeof(emps)/sizeof(Empregado*);
                    printf("Resul :        %d registros   ----    -----\n\nImprimindo:\n", resul);
                    //Imprimindo aki só p/ teste
                    for(int i=0;i<resul;i++)
                            imprime_empreg(emps[i]);    //n imprime
                    
                    if(resul > 1){    //Precisa escolher um empregado
                        printf("Mais de um empregado com esse nome. Por favor, digite o código do empregado desejado:\n");
                        for(int i=0;i<resul;i++)
                            imprime_empreg(emps[i]);
                        scanf(" %d", &cod);
                         for(int i=0;i<resul;i++){
                             if(cod == emps[i]->cod){
                                 d = criarDependente(nome, idade, emps[i]->cod);
                                inserirHashDep(hash, regts, excls, d, tamHash, p, l, &qtd_registros);
                             }
                         }
                    }
                    else{
                        printf("No else    ---    -----    ------\n");
                        d = criarDependente(nome, idade, emps[0]->cod);    //-->    A SEG FAULT ESTÁ AQUI    (acho q é emps[0]) 
                        printf("TESTE    TESTE\n");
                        inserirHashDep(hash, regts, excls, d, tamHash, p, l, &qtd_registros);
                    }
                }
                else    printf("Não foi possível encontrar empregado correspondente.\n\n");
				break;
			case 3 :    //Buscar
				printf("1. Buscar Empregado\n2. Buscar Dependente\n3. Voltar\nDigite uma opção: ");
				scanf("%d", &op);
				fflush(stdin);
				switch (op){
					case 1 :    //buscar Empregado
						printf("1. Buscar Codigo\n2. Buscar Nome\n3. Buscar Idade\n4. Buscar Salario\n5. Buscar Número de Dependente\n6. Voltar\nDigite uma opção: ");
						scanf("%d", &op);
						fflush(stdin);
						switch (op){
							case 1 :    //por codigo
								printf("Digite o codigo do Empregado: ");
								scanf("%d", &cod);
								fflush(stdin);
								if((end = buscarCodEmp(hash, regts, cod, tamHash, p, l)) != -1){
									printf("Empregado encontrado!\n\n1. Excluir\n2. Modificar\n3. Imprimir\n4. Voltar\nDigite uma opção: ");
									scanf("%d", &op);
									fflush(stdin);
									switch (op){
										case 1 :
                                            printf("Não implementado ainda\n");
											excluirHashEmp(hash, regts, excls, end, tamHash, p, l, &qtd_registros);
											printf("Empregado Excluido!\n");    //SEG FAULT*/
											break;
										case 2 :
											printf("Não implementado ainda!\n\n");
											break;
										case 3 :
											fseek(regts, end*tamanhoEmpregado(), SEEK_SET);
											emp_aux = le_empreg(regts);
											imprime_empreg(emp_aux);
											free(emp_aux);
                                            printf("\n");
											break;
										case 4 :
											break;
										default :
											printf("Opção invalida!\n");
									}
								}
								else
									printf("Empregado NÃO encontrado!\n\n");
								break;
							case 2 :    //por nome
                                printf("Digite o nome do empregado: ");
                                scanf("%s[^\n]", nome);
                                printf("Dps do scanf                                                -------\n");
                                emps = buscaNome(arqsInv, nome);
                                printf("Dps de arqsInv    ---    -    -    -\n\n");
                                if(emps != NULL){
                                    printf("NO IF                    ----------------\n");
                                    int resul = sizeof(emps)/sizeof(FILE*);
                                    printf("reusl = %d            -----                    --", resul);
                                    if(resul == 1){
                                        printf("Empregado encontrado!\n\n1. Excluir\n2. Modificar\n3. Imprimir\n4. Voltar\nDigite uma opção: ");
                                        scanf("%d", &op);
                                        fflush(stdin);
                                        switch (op){
                                            case 1 :
                                                printf("Não implementado ainda\n");
                                                /*excluirHash(hash, regts, excls, end, tamHash, p, l);
                                                printf("Empregado Excluido!\n");    SEG FAULT*/
                                                break;
                                            case 2 :
                                                printf("Não implementado ainda!\n\n");
                                                break;
                                            case 3 :
                                                imprime_empreg(emps[0]);
                                                printf("\n");
                                                break;
                                            case 4 :
                                                break;
                                            default :
                                                printf("Opção invalida!\n");
                                        }
                                    }
                                }
								break;
							case 3 :    //por idade
								printf("Não implementado ainda!\n\n");
								break;
							case 4 :
								printf("Não implementado ainda!\n\n");
								break;
							case 5 :
								printf("Não implementado ainda!\n\n");
								break;
							case 6 :
								break;
							default :
								printf("Opção invalida!\n");
						}
						break;
					case 2 :
                        printf("Não implementado ainda!\n\n");
						break;
					case 3 :
						break;
					default :
						printf("Opção invalida!\n");
				}
				break;
			case 4 :    //Imprimir
                printf("Deseja imprimir\n1. Empregado\n2. Dependente\n3. Voltar\nDigite uma opção: ");
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
								if((end = buscarCodEmp(hash, regts, cod, tamHash, p, l)) != -1){
									fseek(regts, end*tamanhoEmpregado(), SEEK_SET);
                                    emp_aux = le_empreg(regts);
                                    imprime_empreg(emp_aux);
                                    printf("\n");
								}
								else
									printf("Empregado NÃO encontrado!\n\n");
								break;
							case 2 :
								printf("Não implementado ainda!\n\n");
								break;
							case 3 :
								printf("Não implementado ainda!\n\n");
								break;
							case 4 :
								printf("Não implementado ainda!\n\n");
								break;
							case 5 :
								printf("Não implementado ainda!\n\n");
								break;
							case 6 :
								break;
							default :
								printf("Opção invalida!\n");
						}
						break;
					case 2 :
                        printf("Não implementado ainda!\n\n");
						break;
					case 3 :
						break;
					default :
						printf("Opção invalida!\n");
				}
				break;
			case 5 :    //Sair
                run = 0;
				break;
			default :
				printf("Opção invalida!\n");
		}
	}

    return 0;
}
