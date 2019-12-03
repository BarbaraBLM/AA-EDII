//    ATENÇÃO : argumento de linha de comando = tamanho da Hash. Rode: .\exe tam
#include <stdio.h>
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
    Empregado** emps;
    Dependente* d;
    
    int cod;
    char nome[50], nome2[50];
    int idade = 0;
    double salario;
    int end, n_dependentes;

    /*
    FILE *hash;
    FILE *regts;
    FILE *excls;
    
    FILE *hashDep;
    FILE *regtsDep;
    FILE *exclsDep;

    
    if ((hash = fopen("hash.dat","r+b")) == NULL) {

        hash = fopen("hash.dat","w+b");
    }
    if ((regts = fopen("registros.dat","r+b")) == NULL) {

        regts = fopen("registros.dat","w+b");
    }
    if ((excls = fopen("exclusao.dat","r+b")) == NULL) {

        excls = fopen("exclusao.dat","w+b");
    }


    if ((hashDep = fopen("hashDep.dat","r+b")) == NULL) {

        hashDep = fopen("hashDep.dat","w+b");
    }
    if ((regtsDep = fopen("registrosDep.dat","r+b")) == NULL) {

        regtsDep = fopen("registrosDep.dat","w+b");
    }
    if ((exclsDep = fopen("exclusaoDep.dat","r+b")) == NULL) {

        excls = fopen("exclusaoDep.dat","w+b");
    }*/

    
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
                arqsInv = arquivo_invertido_emp(regts, qtd_registros);       
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
    
                emps = buscaNome(arqsInv, nome2, &qtdTotal);
                if(emps != NULL){
                    if(qtdTotal > 1){    //Precisa escolher um empregado
                        printf("Mais de um empregado com esse nome. Por favor, digite o código do empregado desejado:\n");
                        for(int i=0;i<qtdTotal;i++)
                            printf("\n\nEmpregado:\nCod: %d\nNome: %s\nIdade:%d\nSalario:%f\nNº dependentes:%d\n", emps[i]->cod, emps[i]->nome, emps[i]->idade, emps[i]->salario, emps[i]->n_dependentes);
                        scanf(" %d", &cod);
                         for(int i=0;i<qtdTotal;i++){
                             if(cod == emps[i]->cod){
                                d = criarDependente(nome, idade, emps[i]->cod);
                                inserirHashDep(hashDep, regtsDep, exclsDep, d, tamHash, pD, lD, &qtd_registrosDep);
                             }
                         }
                    }
                    else{
                        printf("qtdTotal = 1\n");
                        for(i=0;i<qtdTotal;i++){
                            printf("Empregado encontrado:\n");
                            imprime_empreg(emps[i]);
                            //--------------------------------
                            //--------------------------------
                            d = criarDependente(nome, idade, emps[i]->cod);    //-->    A SEG FAULT ESTÁ AQUI    (acho q é  )
                            printf("TESTE   \n");
                            inserirHashDep(hashDep, regtsDep, exclsDep, d, tamHash, pD, lD, &qtd_registrosDep);
                            printf("Dep em dep.c:\n");
                            imprime_dep(d);

                            printf("HASH DEPENDENTE\n");
                            imprimeHash(hashDep);
                            imprime_reg(regtsDep, 2);

                            arqsInv = arquivo_invertido_emp(regts, qtd_registros);
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
                                printf("teste ---\n");
                                if((end = buscarCodEmp(hash, regts, cod, tamHash, p, l)) != -1){
                                    printf("Empregado encontrado!\n\n1. Excluir\n2. Modificar\n3. Imprimir\n4. Voltar\nDigite uma opção: ");
                                    scanf("%d", &op);
                                    fflush(stdin);
                                    switch (op){
                                        case 1 :
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
                            case 2 :    //por nome    ERRO
                                printf("Digite o nome do empregado: ");
                                scanf("%s[^\n]", nome);
                                emps = buscaNome(arqsInv, nome, &qtdTotal);

                                printf("Dps de arqsInv    ---    -    -    -\n\n");
                                if(emps != NULL){
                                
                                    if(qtdTotal == 1){
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
                                }else{
                                    printf("Não há empregado cadastrado para o nome '%s'\n", nome);
                                }
                                break;
                            case 3 :    //por idade    ERRO  (id > maior_id)
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
                                else printf("Nenhum empregado com idade maior que %f\n", salario);;
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
                                                        scanf(" %s[^\n]", nome);
                                                        //        Mudar nome me arq regts
                                                        break;
                                                    case 2:    //modificar idade
                                                        printf("Digite nova idade: ");
                                                        scanf(" %d", &idade);
                                                        //        Mudar atriome me arq regts
                                                        break;
                                                    case 3:    //modificar salario
                                                        break;
                                                    case 4:    //modificar n_dependentes
                                                        break;
                                                    default:
                                                        printf("Opção invalida!\n");
                                                        
                                                }
                                                break;
                                            case 3:
                                                break;
                                            case 4:
                                                break;
                                            default:
                                                printf("Opção invalida!\n");
                                        
                                    } 
                                }
                                else    printf("Nenhum empregado com salário maior que %f\n", salario);
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
                                                        scanf(" %s[^\n]", nome);
                                                        //        Mudar nome me arq regts
                                                        break;
                                                    case 2:    //modificar idade
                                                        printf("Digite nova idade: ");
                                                        scanf(" %d", &idade);
                                                        //        Mudar atriome me arq regts
                                                        break;
                                                    case 3:    //modificar salario
                                                        break;
                                                    case 4:    //modificar n_dependentes
                                                        break;
                                                    default:
                                                        printf("Opção invalida!\n");
                                                        
                                                }
                                                break;
                                            case 3:
                                                break;
                                            case 4:
                                                break;
                                            default:
                                                printf("Opção invalida!\n");
                                        
                                    } 
                                }
                                else    printf("Nenhum empregado com número de dependentes maior que %f\n", salario);
                                break;//-------------------------
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

    //arquivos do empregado
    fclose(hash);
    fclose(regts);
    fclose(excls);
    
    //arquivos do dependente
    fclose(hashDep);
    fclose(regtsDep);
    fclose(exclsDep);


    /*
    for(int i=0; i<5; i++){

        fclose(arqsInv[i]);
        fclose(arqsInvDep[i]);
    }*/

    printf("Programa finalizado!!!\n");
    return 0;
}
