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
    int l = 0, lD = 0;
    int qtd_registros = 0, qtd_registrosDep = 0, qtdTotal = 0;
    int p = 0, pD = 0;
    int aux = -1;
    int run = 1;
    int op, busca = 0, resul = 0, i = 0;
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
    
    FILE *hashDep = fopen("hashDep.dat","w+b");
	FILE *regtsDep = fopen("registrosDep.dat","w+b");
	FILE *exclsDep = fopen("exclusaoDep.dat","w+b");
    
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
                emps = buscaNome(arqsInv, nome2, &qtdTotal);
                if(emps != NULL){
                    if(qtdTotal > 1){    //Precisa escolher um empregado
                        printf("Mais de um empregado com esse nome. Por favor, digite o código do empregado desejado:\n");
                        for(int i=0;i<resul;i++)
                            printf("\n\nEmpregado:\nCod: %d\nNome: %s\nIdade:%d\nSalario:%f\nNº dependentes:%d\n", emps[i]->cod, emps[i]->nome, emps[i]->idade, emps[i]->salario, emps[i]->n_dependentes);
                        scanf(" %d", &cod);
                         for(int i=0;i<resul;i++){
                             if(cod == emps[i]->cod){
                                 d = criarDependente(nome, idade, emps[i]->cod);
                                inserirHashDep(hashDep, regtsDep, exclsDep, d, tamHash, pD, lD, &qtd_registrosDep);
                             }
                         }
                    }
                    else{
                        printf("qtdTotal = 1\n");
                        for(i=0;i<qtdTotal;i++){
                            d = criarDependente(nome, idade, emps[i]->cod);    //-->    A SEG FAULT ESTÁ AQUI    (acho q é emps[0]) 
                            //printf("TESTE    TESTE\n");
                            inserirHashDep(hashDep, regtsDep, exclsDep, d, tamHash, pD, lD, &qtd_registrosDep);
                        }
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
                                emps = buscaNome(arqsInv, nome, &qtdTotal);
                                printf("Dps de arqsInv    ---    -    -    -\n\n");
                                if(emps != NULL){
                                    printf("NO IF                    ----------------\n");
                                    resul = sizeof(emps)/sizeof(FILE*);
                                    printf("resul = %d            sizeof(emps) = %ld\n-----                    --", resul, sizeof(emps));
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
                                printf("Buscar empregados com idade maior que: ");
                                scanf(" %d", &idade);
                                while(idade < 0){
                                    printf("Idade negativa. Tente novamente.\n");
                                    scanf(" %d", &idade);
                                }
                                emps = buscaIdadeMaiorQueX(arqsInv, idade, &qtdTotal);
                                if(emps != NULL){
                                    resul = sizeof(emps)/sizeof(Empregado*);
                                    for(int i=0;i<resul;i++){
                                        printf("\n\nEmpregado:\nCod: %d\nNome: %s\nIdade:%d\nSalario:%f\nNº dependentes:%d\n", emps[i]->cod, emps[i]->nome, emps[i]->idade, emps[i]->salario, emps[i]->n_dependentes);
                                    }
                                    if(resul > 1){
                                        printf("Empregados encontrados!\nDeseja alterar algum desses registros?\n1. Sim\n2. Não\n");
                                        printf("Empregado encontrado!\n\n1. Excluir\n2. Modificar\n3. Imprimir\n4. Voltar\nDigite uma opção: ");
                                        scanf(" %d", &op);
                                        switch (op){
                                            case 1:
                                                printf("Digite o código do registro: ");
                                                scanf(" %d", &cod);
                                                do{
                                                    for(int i=0;i<resul;i++){
                                                    if(cod == emps[i]->cod)
                                                        break;
                                                    }
                                                    if(i==resul-1)    printf("Código não encontrado.\n");
                                                }while(i == resul-1);
                                                printf("1. Excluir\n2. Modificar\n3. Imprimir\n4. Voltar\nDigite uma opção: ");
                                                scanf(" %d", &op);
                                                switch(op){
                                                    case 1:    //excluir
                                                        rewind(arqsInv[0]);
                                                        int i=0, aux = 0;
                                                        while(fread(&aux, sizeof(int), 1, arqsInv[0]) > 0){
                                                            i++;
                                                            if(aux = cod){
                                                                break;
                                                            }
                                                            fseek(arqsInv[0], 50*sizeof(char), SEEK_CUR);//pula nome
                                                            fseek(arqsInv[0], sizeof(int), SEEK_CUR);//pula idade
                                                            fseek(arqsInv[0], sizeof(double), SEEK_CUR);//pula salario
                                                            fseek(arqsInv[0], 5*sizeof(int), SEEK_CUR);//pula n_dependentes, prox_
                                                        }
                                                         excluirHashEmp(hash, regts, excls, i, tamHash, p, l, &qtd_registros);
                                                        break;
                                                    case 2:
                                                        break;
                                                    case 3:
                                                        break;
                                                    case 4:
                                                        break;
                                                    default:
                                                        printf("Opção invalida!\n");
                                                }
                                                
                                        }
                                        
                                    }
                                }
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
					case 2 :    //buscar Dependente
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
					case 1 :    //imprimir empregado
						printf("1. Buscar Codigo\n2. Buscar Nome\n3. Buscar Idade\n4. Buscar Salario\n5. Buscar Número de Dependente\n6. Voltar\nDigite uma opção: ");
						scanf("%d", &op);
						fflush(stdin);
						switch (op){
							case 1 :    //por codigo
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
							case 2 :    //por no
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
					case 2 :    //imprimir dependente
                        printf("1. Buscar Codigo\n2. Buscar Nome\n3. Buscar Idade\n4. Buscar Codido Empregado\n5. Voltar\nDigite uma opção: ");
						scanf("%d", &op);
						fflush(stdin);
						switch (op){
							case 1 :    //por codigo
								printf("Digite o codigo do Dependente: ");
								scanf("%d", &cod);
								fflush(stdin);
								if((end = buscarCodDep(hashDep, regtsDep, cod, tamHash, pD, lD)) != -1){
									fseek(regtsDep, end*tamanhoDependente(), SEEK_SET);
                                    d = le_dep(regtsDep);
                                    imprime_dep(d);
                                    printf("\n");
								}
								else
									printf("Dependentegado NÃO encontrado!\n\n");
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
								break;
							default :
								printf("Opção invalida!\n");
						}
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
