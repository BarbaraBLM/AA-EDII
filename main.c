/*    ATENÇÃO	ATENÇÃO		ATENÇÃO
Rode: .\exe tamanhoHash		, sendo tamanhoHash um inteiro
*/

#include <stdlib.h>
#include <string.h>
#include "empregado.c"
#include "dependente.c"
#include "hash.c"
#include "invertidoemp.c"
#include "invertidodep.c"

/*
    para ler, preservando o conteúdo: r+b
    usar w+b apenas se o arquivo não existir
*/

int main(int argc, char *argv[]){
    int tamHash = atoi(argv[1]);
    int l = 0, lD = 0;
    int qtd_registros = 0, qtd_registrosDep = 0, qtdTotal = 0;
    int p = 0, pD = 0;
    int aux = -1;
    int run = 1;
    int op, busca = 0, resul = 0, i = 0;
    Empregado* emp_aux;
    Dependente* dep_aux;
    Empregado** emps;
    Dependente** deps;
    
    int cod;
    char nomeEmpregado[50], nomeDependente[50];
    int idade = 0;
    double salario;
    int end, n_dependentes;

    
    FILE *hash = fopen("hash.dat","w+b");
    FILE *regts = fopen("registros.dat","w+b");
    FILE *excls = fopen("exclusao.dat","w+b");
    
    FILE *hashDep = fopen("hashDep.dat","w+b");
    FILE *regtsDep = fopen("registrosDep.dat","w+b");
    FILE *exclsDep = fopen("exclusaoDep.dat","w+b");
    
    FILE** arqsInv = (FILE**) malloc(5*sizeof(FILE*));
    FILE** arqsInvDep = (FILE**) malloc(5*sizeof(FILE*));
    
    initHash(hash, tamHash);
    initHash(hashDep, tamHash);

    arqsInv = arquivo_invertido_emp(regts, qtd_registros);
    arqsInvDep = arquivo_invertido_dep(regtsDep, qtd_registrosDep);

    fwrite(&aux, sizeof(int), 1, excls);
    fwrite(&aux, sizeof(int), 1, exclsDep);
    while(run == 1){
        //Status
        printf("1. Cadastrar Empregado\n2. Cadastrar Dependente\n3. Buscar\n4. Imprimir\n5. Sair\nDigite uma opção: ");
        scanf("%d", &op);
        fflush(stdin);
        switch (op){
            case 1 :    //Cadastrar empregado
                printf("Digite o nome do Empregado: ");
                setbuf(stdin, NULL);
                fgets(nomeEmpregado,50,stdin);
                clean_fgets_error(nomeEmpregado);
                fflush(stdin);
                printf("Idade do Empregado: ");
                scanf(" %d", &idade);
                fflush(stdin);
                printf("Salario do Empregado: ");
                scanf(" %lf", &salario);
                fflush(stdin);
                Empregado *emp = criarEmpregado(nomeEmpregado, idade, salario);
                inserirHashEmp(hash, regts, excls, emp, tamHash, p, l, &qtd_registros);
                printf("Empregado inserido! %d\n\n", qtd_registros);
                arqsInv = arquivo_invertido_emp(regts, qtd_registros);       
                break;
            case 2 :    //Cadastrar dependente
                printf("Digite o nome do Dependente: ");
                setbuf(stdin, NULL);
                fgets(nomeDependente,50,stdin);
                clean_fgets_error(nomeDependente);
                fflush(stdin);

                printf("Idade do Dependente: ");
                scanf(" %d", &idade);
                fflush(stdin);
                printf("Digite o nome do Empregado do qual é dependente: ");
                setbuf(stdin, NULL);
                fgets(nomeEmpregado,50,stdin);
                clean_fgets_error(nomeEmpregado);

                fflush(stdin);
    
                emps = buscaNome(arqsInv, nomeEmpregado, &qtdTotal);
                if(emps != NULL){
                    if(qtdTotal > 1){    //Precisa escolher um empregado
                        printf("Mais de um empregado com esse nome. Por favor, digite o código do empregado desejado:\n");
                        for(int i=0;i<qtdTotal;i++)
                            printf("\n\nEmpregado:\nCod: %d\nNome: %s\nIdade:%d\nSalario:%f\nNº dependentes:%d\n", emps[i]->cod, emps[i]->nome, emps[i]->idade, emps[i]->salario, emps[i]->n_dependentes);
                        scanf(" %d", &cod);
                         for(int i=0;i<qtdTotal;i++){
                             if(cod == emps[i]->cod){
                                Dependente* dep = criarDependente(nomeDependente, idade, emps[i]->cod);
                                inserirHashDep(hashDep, regtsDep, exclsDep, dep, tamHash, pD, lD, &qtd_registrosDep);
                                incrementa_ndepEmp(hash, regts, emps[i]->cod, tamHash, l);
                                printf("Registros de dependentes\n");
                                arqsInvDep = arquivo_invertido_dep(regtsDep, qtd_registrosDep);
                                arqsInv = arquivo_invertido_emp(regts, qtd_registros);
                            }
                         }
                    }
                    else{
                        //printf("qtdTotal = 1\n");
                        for(i=0;i<qtdTotal;i++){
                            printf("Empregado encontrado:\n");
                            incrementa_ndepEmp(hash, regts, emps[i]->cod, tamHash, l);
                            printf("\n\nEmpregado:\nCod: %d\nNome: %s\nIdade:%d\nSalario:%f\nNº dependentes:%d\n", emps[i]->cod, emps[i]->nome, emps[i]->idade, emps[i]->salario, emps[i]->n_dependentes+1);
                            Dependente* dep = criarDependente(nomeDependente, idade, emps[i]->cod);
                            imprime_dep(dep);
                            inserirHashDep(hashDep, regtsDep, exclsDep, dep, tamHash, pD, lD, &qtd_registrosDep);
                            
                            arqsInvDep = arquivo_invertido_dep(regtsDep, qtd_registrosDep);
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
                                        case 1 :	//excluir
                                            excluirHashEmp(hash, regts, excls, end, tamHash, p, l, &qtd_registros);
                                            break;
                                        case 2 :	//modificar
                                            printf("O que deseja modificar:\n1.Nome\n2.Idade\n3.Salario\n4.Voltar\n(Para modificar n_dependetes, deve-se excluir o dependente)\nDigite uma opção: ");
                                            scanf(" %d", &op);
                                            fflush(stdin);
                                            switch (op){
                                            	case 1:	//nome
                                            		printf("Digite novo nome: ");
                                                    setbuf(stdin, NULL);
                                                    fgets(nomeEmpregado,50,stdin);
                                                    clean_fgets_error(nomeEmpregado);
                                                    fflush(stdin);

                                            		alteraNomeEmp(hash, regts, cod, tamHash, l, nomeEmpregado);
                                            		arqsInv = arquivo_invertido_emp(regts, qtd_registros);
                                            		break;
                                            	case 2:	//idade
                                            		printf("Digite nova idade: ");
                                            		scanf(" %d", &idade);
                                            		alteraIdadeEmp(hash, regts, cod, tamHash, l, idade);
                                            		arqsInv = arquivo_invertido_emp(regts, qtd_registros);
                                            		break;
                                            	case 3:	//salario
                                            		printf("Digite novo salário: ");
                                            		scanf(" %lf", &salario);
                                            		alteraSalarioEmp(hash, regts, cod, tamHash, l, salario);
                                            		arqsInv = arquivo_invertido_emp(regts, qtd_registros);
                                            		break;
                                            	case 4:
                                            		break;
                                           		default :
		                                            printf("Opção invalida!\n");
                                            }
                                            arqsInv = arquivo_invertido_emp(regts, qtd_registros);
                                        break;
                                        case 3 :	//imprimir
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
                                setbuf(stdin, NULL);
                                fgets(nomeEmpregado,50,stdin);
                                clean_fgets_error(nomeEmpregado);
                                fflush(stdin);
                                emps = buscaNome(arqsInv, nomeEmpregado, &qtdTotal);

                                if(emps != NULL){
                                
                                    if(qtdTotal == 1){
                                        printf("Empregado encontrado!\n\n1. Excluir\n2. Modificar\n3. Imprimir\n4. Voltar\nDigite uma opção: ");
                                        scanf("%d", &op);
                                        fflush(stdin);
                                        switch (op){
                                            case 1 :
                                                end = buscarCodEmp(hash, regts, emps[0]->cod, tamHash, p, l);
                                                excluirHashEmp(hash, regts, excls, end, tamHash, p, l, &qtd_registros);
                                                printf("Empregado Excluido!\n");
                                                arqsInv = arquivo_invertido_emp(regts, qtd_registros);
                                            break;
                                            case 2 :
                                                printf("O que deseja modificar:\n1.Nome\n2.Idade\n3.Salario\n4.Voltar\n(Para modificar n_dependetes, deve-se excluir o dependente)\nDigite uma opção: ");
                                                scanf(" %d", &op);
                                                switch (op){
                                                    case 1:    //modificar nome
                                                        printf("Digite novo nome: ");
                                                        setbuf(stdin, NULL);
                                                        fgets(nomeEmpregado,50,stdin);
                                                        clean_fgets_error(nomeEmpregado);
                                                        fflush(stdin);
                                                        alteraNomeEmp(hash, regts, emps[0]->cod, tamHash, l, nomeEmpregado);
                                                        arqsInv = arquivo_invertido_emp(regts, qtd_registros);
                                                        break;
                                                case 2:	//idade
                                            		printf("Digite nova idade: ");
                                            		scanf(" %d", &idade);
                                            		alteraIdadeEmp(hash, regts, cod, tamHash, l, idade);
                                            		arqsInv = arquivo_invertido_emp(regts, qtd_registros);
                                            		break;
                                            	case 3:	//salario
                                            		printf("Digite novo salário: ");
                                            		scanf(" %lf", &salario);
                                            		alteraSalarioEmp(hash, regts, cod, tamHash, l, salario);
                                            		arqsInv = arquivo_invertido_emp(regts, qtd_registros);
                                            		break;
                                            	case 4:
                                            		break;
                                           		default :
		                                            printf("Opção invalida!\n");
                                                break;
                                            }
                                    }
                                }
                                }else{
                                    printf("Não há empregado cadastrado para o nome '%s'\n", nomeEmpregado);
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
                                    for(int i=0;i<qtdTotal;i++){
                                        printf("\n\nEmpregado:\nCod: %d\nNome: %s\nIdade:%d\nSalario:%f\nNº dependentes:%d\n", emps[i]->cod, emps[i]->nome, emps[i]->idade, emps[i]->salario, emps[i]->n_dependentes);
                                    }

                                    if(qtdTotal > 1){
                                        printf("Mais de um empregado encontrado. Por favor, digite o código do empregado desejado:\n");
                                        scanf(" %d", &cod);
                                        for(i=0;i<qtdTotal;i++){
                                            if(emps[i]->cod == cod)    break;
                                        }
                                    } else    i=0;

                                        printf("Empregados encontrados!\nDeseja alterar algum desses registros?\n1. Sim\n2. Não\n");
                                        scanf(" %d", &op);
                                        switch (op){
                                            case 1:

                                                printf("Digite o código do registro: ");
                                                scanf(" %d", &cod);

                                                for(int i=0;i<qtdTotal;i++){
                                                    if(cod == emps[i]->cod)
                                                         break;
                                                }
                                                if(i==qtdTotal)    printf("Código não encontrado.\n");


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
                                                        arqsInv = arquivo_invertido_emp(regts, qtd_registros);
                                                        break;
                                                    case 2:
                                                        printf("O que deseja modificar:\n1.Nome\n2.Idade\n3.Salario\n4.Voltar\n(Para modificar n_dependetes, deve-se excluir o dependente)\nDigite uma opção: ");
                                                        scanf(" %d", &op);
                                                        switch (op){
                                                            case 1:    //modificar nome
                                                                printf("Digite novo nome: ");
                                                                setbuf(stdin, NULL);
                                                                fgets(nomeEmpregado,50,stdin);
                                                                clean_fgets_error(nomeEmpregado);
                                                                fflush(stdin);
                                                                alteraNomeEmp(hash, regts, emps[i]->cod, tamHash, l, nomeEmpregado);
                                                                arqsInv = arquivo_invertido_emp(regts, qtd_registros);
                                                                break;
                                                        case 2: //idade
                                                            printf("Digite nova idade: ");
                                                            scanf(" %d", &idade);
                                                            alteraIdadeEmp(hash, regts, cod, tamHash, l, idade);
                                                            arqsInv = arquivo_invertido_emp(regts, qtd_registros);
                                                            break;
                                                        case 3: //salario
                                                            printf("Digite novo salário: ");
                                                            scanf(" %lf", &salario);
                                                            alteraSalarioEmp(hash, regts, cod, tamHash, l, salario);
                                                            arqsInv = arquivo_invertido_emp(regts, qtd_registros);
                                                            break;
                                                        case 4:
                                                            break;
                                                        default :
                                                            printf("Opção invalida!\n");
                                                        }                                                   
                                                        printf("Não implementado ainda!\n\n");
                                                    break;
                                                    case 3:
                                                        printf("Não implementado ainda!\n\n");
                                                        break;
                                                    case 4:
                                                        break;
                                                    default:
                                                        printf("Opção invalida!\n");
                                            }
                                        case 2:
                                            break;
                                        }
                                    
                                }
                                else printf("Nenhum empregado com idade maior que %d\n", idade);
                                break;
                            case 4 :    //por salario
                                printf("Buscar empregados com salário maior que : ");
                                scanf(" %lf", &salario);
                                emps = buscaSalarioMaiorQueX(arqsInv, salario, &qtdTotal);
                                if(emps != NULL){
                                    for(i=0;i<qtdTotal;i++)
                                        printf("\n\nEmpregado:\nCod: %d\nNome: %s\nIdade:%d\nSalario:%f\nNº dependentes:%d\n", emps[i]->cod, emps[i]->nome, emps[i]->idade, emps[i]->salario, emps[i]->n_dependentes);
                                    
                                    if(qtdTotal > 1){
                                        printf("Mais de um empregado encontrado. Por favor, digite o código do empregado desejado:\n");
                                        scanf(" %d", &cod);
                                        for(i=0;i<qtdTotal;i++){
                                            if(emps[i]->cod == cod)    break;
                                        }
                                    } else    i=0;
                                    printf("1. Excluir\n2. Modificar\n3. Imprimir\n4. Voltar\nDigite uma opção: ");
                                        scanf(" %d", &op);
                                        switch(op){
                                            case 1:    //excluir
                                                end = buscarCodEmp(hash, regts, emps[i]->cod, tamHash, p, l);
                                                excluirHashEmp(hash, regts, excls, end, tamHash, p, l, &qtd_registros);
                                                break;
                                            case 2:    //modificar
                                                printf("Qual atributo gostaria de modificar?\n1. Nome\n2. Idade\n3. Salario\n4. N_dependentes\n");
                                                scanf(" %d", &op);
                                                switch (op){
                                                    case 1:    //modificar nome
                                                        printf("Digite novo nome: ");
                                                        setbuf(stdin, NULL);
                                                        fgets(nomeEmpregado,50,stdin);
                                                        clean_fgets_error(nomeEmpregado);
                                                        fflush(stdin);
                                                        //        Mudar nome me arq regts
                                                        break;
                                                    case 2:    //modificar idade
                                                        printf("Digite nova idade: ");
                                                        scanf(" %d", &idade);
                                                        //        Mudar atriome me arq regts
                                                        break;
                                                    case 3:    //modificar salario
                                                        printf("Não implementado ainda!\n\n");
                                                        break;
                                                    case 4:    //modificar n_dependentes
                                                        printf("Não implementado ainda!\n\n");
                                                        break;
                                                    default:
                                                        printf("Opção invalida!\n");
                                                        
                                                }
                                                break;
                                            case 3:
                                                printf("Não implementado ainda!\n\n");
                                                break;
                                            case 4:
                                                break;
                                            default:
                                                printf("Opção invalida!\n");
                                        
                                    } 
                                }
                                else    printf("Nenhum empregado com salário maior que %.2f\n", salario);
                                break;
                            case 5 :    //por n_dependentes
                                printf("Buscar empregados com número de dependentes maior que : ");
                                scanf(" %d", &n_dependentes);
                                emps = buscaNumDepMaiorQueX(arqsInv, n_dependentes, &qtdTotal);
                                if(emps != NULL){
                                    for(i=0;i<qtdTotal;i++)
                                        printf("\n\nEmpregado:\nCod: %d\nNome: %s\nIdade:%d\nSalario:%f\nNº dependentes:%d\n", emps[i]->cod, emps[i]->nome, emps[i]->idade, emps[i]->salario, emps[i]->n_dependentes);
                                    
                                    if(qtdTotal > 1){
                                        printf("Mais de um empregado encontrado. Por favor, digite o código do empregado desejado:\n");
                                        scanf(" %d", &cod);
                                        for(i=0;i<qtdTotal;i++){
                                            if(emps[i]->cod == cod)    break;
                                        }
                                    } else    i=0;

                                    printf("1. Excluir\n2. Modificar\n3. Imprimir\n4. Voltar\nDigite uma opção: ");
                                        scanf(" %d", &op);
                                        switch(op){
                                            case 1:    //excluir
                                                end = buscarCodEmp(hash, regts, emps[i]->cod, tamHash, p, l);
                                                excluirHashEmp(hash, regts, excls, end, tamHash, p, l, &qtd_registros);
                                                break;
                                            case 2:    //modificar
                                                printf("Qual atributo gostaria de modificar?\n1. Nome\n2. Idade\n3. Salario\n4. N_dependentes\n");
                                                scanf(" %d", &op);
                                                switch (op){
                                                    case 1:    //modificar nome
                                                        printf("Digite novo nome: ");
                                                        setbuf(stdin, NULL);
                                                        fgets(nomeEmpregado,50,stdin);
                                                        clean_fgets_error(nomeEmpregado);
                                                        fflush(stdin);
                                                        //        Mudar nome me arq regts
                                                        break;
                                                    case 2:    //modificar idade
                                                        printf("Não implementado ainda!\n\n");
                                                        //        Mudar atriome me arq regts
                                                        break;
                                                    case 3:    //modificar salario
                                                        printf("Não implementado ainda!\n\n");
                                                        break;
                                                    case 4:    //modificar n_dependentes
                                                        printf("Não implementado ainda!\n\n");
                                                        break;
                                                    default:
                                                        printf("Opção invalida!\n");
                                                        
                                                }
                                                break;
                                            case 3:
                                                printf("Não implementado ainda!\n\n");
                                                break;
                                            case 4:
                                                break;
                                            default:
                                                printf("Opção invalida!\n");
                                        
                                    } 
                                }
                                else    printf("Nenhum empregado com número de dependentes maior que %d\n", n_dependentes);
                                break;//-------------------------
                            case 6 :
                            break;
                            default :
                                printf("Opção invalida!\n");
                        }
                        break;
                    case 2 :    //buscar Dependente
                        printf("1. Buscar codigo\n2. Buscar nome\n3. Buscar idade menor que\n4. Buscar idade maior que\n5. Voltar\nDigite uma opção: ");
                        scanf("%d", &op);
                        fflush(stdin);
                        switch(op){
                            case 1:
                                printf("Digite o codigo do dependente: ");
                                scanf("%d", &cod);
                                fflush(stdin);
                                if((end = buscarCodDep(hashDep, regtsDep, cod, tamHash, pD, lD)) != -1){
                                    printf("Dependente encontrado!\n\n1. Excluir\n2. Modificar\n3. Imprimir\n4. Voltar\nDigite uma opção: ");
                                    scanf("%d", &op);
                                    fflush(stdin);
                                    switch (op){
                                        case 1 :
                                            excluirHashDep(hashDep, regtsDep, exclsDep, end, tamHash, pD, lD, &qtd_registrosDep);
                                            printf("Dependente Excluido!\n");    //SEG FAULT*/
                                            arqsInvDep = arquivo_invertido_dep(regtsDep, qtd_registrosDep);
                                            break;
                                        case 2 :
                                        	printf("Não implementado ainda.\n");
                                            break;
                                        case 3 :
                                            fseek(regtsDep, end*tamanhoDependente(), SEEK_SET);
                                            dep_aux = le_dep(regtsDep);
                                            imprime_dep(dep_aux);
                                            free(dep_aux);
                                            printf("\n");
                                            break;
                                        case 4 :
                                            break;
                                        default :
                                            printf("Opção invalida!\n");
                                    }
                                }
                                else
                                    printf("Dependente NÃO encontrado!\n\n");
                            break;

                            case 2: // 
                                printf("Digite o nome do dependente: ");
                                setbuf(stdin, NULL);
                                fgets(nomeDependente,50,stdin);
                                clean_fgets_error(nomeDependente);
                                fflush(stdin);
                                deps = buscaNomeDep(arqsInvDep, nomeDependente, &qtdTotal);

                                if(deps != NULL){
                                
                                    if(qtdTotal == 1){
                                        printf("Dependente encontrado!\n\n1. Excluir\n2. Modificar\n3. Imprimir\n4. Voltar\nDigite uma opção: ");
                                        scanf("%d", &op);
                                        fflush(stdin);
                                        switch (op){
                                            case 1 :
                                                end = buscarCodDep(hashDep, regtsDep, deps[0]->cod, tamHash, pD, lD);
                                                excluirHashDep(hashDep, regtsDep, exclsDep, end, tamHash, pD, lD, &qtd_registrosDep);
                                                printf("Dependente Excluido!\n");    //SEG FAULT*/
                                                arqsInvDep = arquivo_invertido_dep(regtsDep, qtd_registrosDep);
                                                break;
                                            case 2 :
                                                printf("Não implementado ainda\n");
                                                break;
                                            case 3 :
                                                imprime_dep(deps[0]);
                                                printf("\n");
                                                break;
                                            case 4 :
                                                break;
                                            default :
                                                printf("Opção invalida!\n");
                                        }
                                    }
                                }else{
                                    printf("Não há dependente cadastrado para o nome '%s'\n", nomeEmpregado);
                                }
                            break;

                            case 3:
                                printf("Buscar por idade menor que: ");
                                scanf("%d", &idade);
                                deps = buscaIdadeMenorQueXDep(arqsInvDep, idade, &qtdTotal);

                                if(deps != NULL){

                                    for(i=0;i<qtdTotal;i++)
                                        printf("\n\nDependente:\nCod: %d\nNome: %s\nIdade:%d\nCod empregado:%d\n", deps[i]->cod, deps[i]->nome, deps[i]->idade, deps[i]->cod_emp);
                                        
                                    if(qtdTotal > 1){
                                        printf("Mais de um dependente encontrado. Por favor, digite o código do dependente desejado:\n");
                                        scanf(" %d", &cod);
                                        for(i=0;i<qtdTotal;i++){
                                            if(deps[i]->cod == cod)    break;
                                        }
                                    }else    i=0;

                                    printf("1. Excluir\n2. Modificar\n3. Imprimir\n4. Voltar\nDigite uma opção: ");
                                    scanf(" %d", &op);
                                    switch(op){
                                        case 1:    //excluir
                                            end = buscarCodDep(hashDep, regtsDep, deps[i]->cod, tamHash, pD, lD);
                                            excluirHashDep(hashDep, regtsDep, exclsDep, end, tamHash, pD, lD, &qtd_registrosDep);
                                            printf("Dependente Excluido!\n");    //SEG FAULT*/
                                            arqsInvDep = arquivo_invertido_dep(regtsDep, qtd_registrosDep);
                                        break;

                                        case 2:    //modificar
                                            printf("Qual atributo gostaria de modificar?\n1. Nome\n2. Idade\n");
                                            scanf(" %d", &op);
                                            switch (op){
                                                case 1:    //modificar nome
                                                    printf("Não implementado ainda!\n\n");
                                                break;

                                                case 2:    //modificar idade
                                                    printf("Não implementado ainda!\n\n");
                                                break;

                                                default:
                                                    printf("Opção invalida!\n");       
                                            }
                                        break;

                                        case 3://Imprimir
                                            imprime_dep(deps[i]);
                                            printf("\n\n");
                                        break;

                                        case 4://Voltar
                                        break;

                                        default:
                                        printf("Opção invalida!\n");
                                    } 
                                }
                                else    printf("Nenhum empregado com salário maior que %f\n", salario);
                            break;

                            case 4:
                                printf("Buscar por idade maior que: ");
                                scanf("%d", &idade);
                                deps = buscaIdadeMaiorQueXDep(arqsInvDep, idade, &qtdTotal);

                                if(deps != NULL){

                                    for(i=0;i<qtdTotal;i++)
                                        printf("\n\nDependente:\nCod: %d\nNome: %s\nIdade:%d\nCod empregado:%d\n", deps[i]->cod, deps[i]->nome, deps[i]->idade, deps[i]->cod_emp);
                                        
                                    if(qtdTotal > 1){
                                        printf("Mais de um dependente encontrado. Por favor, digite o código do dependente desejado:\n");
                                        scanf(" %d", &cod);
                                        for(i=0;i<qtdTotal;i++){
                                            if(deps[i]->cod == cod)    break;
                                        }
                                    }else    i=0;

                                    printf("1. Excluir\n2. Modificar\n3. Imprimir\n4. Voltar\nDigite uma opção: ");
                                    scanf(" %d", &op);
                                    switch(op){
                                        case 1:    //excluir
                                            end = buscarCodDep(hashDep, regtsDep, deps[i]->cod, tamHash, pD, lD);
                                            excluirHashDep(hashDep, regtsDep, exclsDep, end, tamHash, pD, lD, &qtd_registrosDep);
                                            printf("Dependente Excluido!\n");    //SEG FAULT*/
                                            arqsInvDep = arquivo_invertido_dep(regtsDep, qtd_registrosDep);
                                        break;

                                        case 2:    //modificar
                                            printf("Qual atributo gostaria de modificar?\n1. Nome\n2. Idade\n");
                                            scanf(" %d", &op);
                                            switch (op){
                                                case 1:    //modificar nome
                                                    printf("Não implementado ainda!\n\n");
                                                break;

                                                case 2:    //modificar idade
                                                    printf("Não implementado ainda!\n\n");
                                                break;

                                                default:
                                                    printf("Opção invalida!\n");       
                                            }
                                        break;

                                        case 3://Imprimir
                                            imprime_dep(deps[i]);
                                            printf("\n\n");
                                        break;

                                        case 4://Voltar
                                        break;

                                        default:
                                        printf("Opção invalida!\n");
                                    } 
                                }
                                else    printf("Nenhum empregado com salário maior que %f\n", salario);
                            break;
                            case 5:
                            break;
                            default:
                            printf("Opção invalida!\n");
                        }
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
                            case 2 :    //por nome
                                printf("Digite o nome do empregado: ");
                                setbuf(stdin, NULL);
                                fgets(nomeEmpregado,50,stdin);
                                clean_fgets_error(nomeEmpregado);
                                fflush(stdin);
                                emps = buscaNome(arqsInv, nomeEmpregado, &qtdTotal);

                                if(emps != NULL){
                                    if(qtdTotal == 1){
                                        printf("\n\nEmpregado:\nCod: %d\nNome: %s\nIdade:%d\nSalario:%f\nNº dependentes:%d\n", emps[0]->cod, emps[0]->nome, emps[0]->idade, emps[0]->salario, emps[0]->n_dependentes);
                                    }
                                }
                                else{
                                    printf("Não há empregado cadastrado para o nome '%s'\n", nomeEmpregado);
                                }
                                break;
                            case 3 : //por idade
                                printf("Buscar empregados com idade maior que: ");
                                scanf(" %d", &idade);
                                while(idade < 0){
                                    printf("Idade negativa. Tente novamente.\n");
                                    scanf(" %d", &idade);
                                }
                                emps = buscaIdadeMaiorQueX(arqsInv, idade, &qtdTotal);
                                if(emps != NULL){
                                    for(int i=0;i<qtdTotal;i++){
                                        printf("\n\nEmpregado:\nCod: %d\nNome: %s\nIdade:%d\nSalario:%f\nNº dependentes:%d\n", emps[i]->cod, emps[i]->nome, emps[i]->idade, emps[i]->salario, emps[i]->n_dependentes);
                                    }
                                }
                                else printf("Nenhum empregado com idade maior que %d\n", idade);
                                break;
                            case 4 ://por salario
                                printf("Buscar empregados com salário maior que : ");
                                scanf(" %lf", &salario);
                                emps = buscaSalarioMaiorQueX(arqsInv, salario, &qtdTotal);
                                if(emps != NULL){
                                    for(i=0;i<qtdTotal;i++){
                                        printf("\n\nEmpregado:\nCod: %d\nNome: %s\nIdade:%d\nSalario:%f\nNº dependentes:%d\n", emps[i]->cod, emps[i]->nome, emps[i]->idade, emps[i]->salario, emps[i]->n_dependentes);
                                    }
                                }
                                else    printf("Nenhum empregado com salário maior que %.2f\n", salario);
                                break;
                            case 5 ://por numero de depentes
                                printf("Buscar empregados com número de dependentes maior que : ");
                                scanf(" %d", &n_dependentes);
                                emps = buscaNumDepMaiorQueX(arqsInv, n_dependentes, &qtdTotal);
                                if(emps != NULL){
                                    for(i=0;i<qtdTotal;i++){
                                        printf("\n\nEmpregado:\nCod: %d\nNome: %s\nIdade:%d\nSalario:%f\nNº dependentes:%d\n", emps[i]->cod, emps[i]->nome, emps[i]->idade, emps[i]->salario, emps[i]->n_dependentes);
                                    }
                                }
                                else    printf("Nenhum empregado com número de dependentes maior que %d\n", n_dependentes);
                                break;
                            case 6 ://volta
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
                                    Dependente* dep = le_dep(regtsDep);
                                    imprime_dep(dep);
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

    //arquivos do empregado
    fclose(hash);
    fclose(regts);
    fclose(excls);
    
    //arquivos do dependente
    fclose(hashDep);
    fclose(regtsDep);
    fclose(exclsDep);

    printf("Programa finalizado!!!\n");
    return 0;
}
