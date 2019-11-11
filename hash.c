#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dependente.h"
#include "empregado.h"

void expandHash(FILE *h, FILE *r, int tam, int* p, int l);

void initHash(FILE *h, int tam){
	int j = -1;
	for (int i = 0; i < tam; ++i){
		fwrite(&j, sizeof(int), 1, h);
	}
}

int hash(int n, int tam, int l){
    if(tam == 0)    return 0;
    return (n%(tam*((int)(pow(2,l)))));
}

void imprimeHash(FILE *hash){
	int aux;
	rewind(hash);
	while(fread(&aux, sizeof(int), 1, hash) != 0){
		printf("%d\n", aux);
	}
}

void imprime_reg(FILE* r){
    rewind(r);
    Empregado* e = (Empregado*) malloc(tamanhoEmpregado());
    imprime_empreg(le_empreg(r));
}

int countExc(FILE* excl){
    int aux = 0, cont=0;
    while(fread(&aux, sizeof(int), 1, excl) > 0){
        if(aux!=-1)    cont++;
    }
    return cont;
}

void percorrendo_lista(FILE* h, FILE* reg, int r_hash){
    Empregado* e = (Empregado*) malloc(tamanhoEmpregado());
    int aux = -1;
    fseek(h, r_hash*sizeof(int), SEEK_SET);
    fread(&aux, sizeof(int), 1, h);
    
    fseek(reg, aux*tamanhoEmpregado(), SEEK_SET);
    e = le_empreg(reg);
    if(e->status != -1)   imprime_empreg(e);
    while(e->prox != -1){
        fseek(reg, e->prox*tamanhoEmpregado(), SEEK_SET);
        e = le_empreg(reg);
        if(e->status != -1)    imprime_empreg(e);
    } 
    free(e);
}

void inserirHash(FILE *h, FILE *r, FILE *exclusao, Empregado *emp, int tam, int l, int *qtd_registros){
    int chave = hash(emp->cod, tam, l);
    //printf("Dps da hash\n\n");
	int excl, aux, end_excl = -1;
	rewind(exclusao);
	while(fread(&excl, sizeof(int), 1, exclusao) != 0){ //Procurar caso tenha algum registro excluido
		end_excl++;
		if(excl != -1) break;
	}
    
	fseek(h, chave*sizeof(int), SEEK_SET);
	fread(&aux, sizeof(int), 1, h);
	if(aux == -1){ //Caso a chave da hash esteja vazia
		fseek(h, chave*tamanhoEmpregado(), SEEK_SET);
		if(excl != -1){ //Caso tenha um registro excluido
			fseek(r, excl*tamanhoEmpregado(), SEEK_SET);
			salva_empreg(emp, r);
			fseek(h, chave*sizeof(int), SEEK_SET);
			fwrite(&excl, sizeof(int), 1, h);
			fseek(exclusao, end_excl*sizeof(int), SEEK_SET);
			fwrite(&end_excl, sizeof(int), 1, exclusao);
		}
		else{
			fseek(r, 0, SEEK_END);
			salva_empreg(emp, r);
			fseek(h, chave*sizeof(int), SEEK_SET);
			fwrite(&(*qtd_registros), sizeof(int), 1, h);
			*qtd_registros += 1;
		}
	}
	else{
		Empregado *emp_aux;
		int end_atual = aux;
		fseek(r, aux*tamanhoEmpregado(), SEEK_SET);
		emp_aux = le_empreg(r);
		while(emp_aux->prox != -1){ //Vai até o final da lista encadeada
			fseek(r, emp_aux->prox*tamanhoEmpregado(), SEEK_SET);
			end_atual = emp_aux->prox;
			emp_aux = le_empreg(r);
		}
		if(excl != -1){ //Caso tenha um registro excluido
			fseek(r, excl*tamanhoEmpregado(), SEEK_SET);
			salva_empreg(emp, r);
			emp_aux->prox = excl;
			fseek(r, end_atual*tamanhoEmpregado(), SEEK_SET);
			salva_empreg(emp_aux, r);
			fseek(exclusao, end_excl*sizeof(int), SEEK_SET);
			fwrite(&end_excl, sizeof(int), 1, exclusao);
		}
		else{
			fseek(r, 0, SEEK_END);
			salva_empreg(emp, r);
			emp_aux->prox = *qtd_registros;
			fseek(r, end_atual*tamanhoEmpregado(), SEEK_SET);
			salva_empreg(emp_aux, r);
			*qtd_registros += 1;
		}
	}
}

void expandHash(FILE *h, FILE *r, int tam, int* p, int l){
	int j = -1;
	int aux;
	//Expande a hash
	fseek(h, 0, SEEK_END);
	fwrite(&j, sizeof(int), 1, h);
	//Confere a posição de p na hash
	fseek(h, *p*sizeof(int), SEEK_SET);
	fread(&aux, sizeof(int), 1, h);
	if(aux != -1){ //Se na posição p da hash não estiver vazio
		//Coloca como vazia a posição de p
		fseek(h, *p*sizeof(int), SEEK_SET);
		fwrite(&j, sizeof(int), 1, h);

		Empregado *emp;
		int nova_chave;
		int end_atual = aux;
		int end_foi = -1;
		int end_ficou = -1;
		//Lê o registro q estava em p
		fseek(r, aux*tamanhoEmpregado(), SEEK_SET);
		emp = le_empreg(r);
		while(end_atual != -1){
			Empregado *emp_aux;
			nova_chave = hash(emp->cod, tam, l+1);
			if(nova_chave != *p){ //Se a nova chave for diferente da posição p da hash significa q ela precisa ser deslocado pra a expansão
				int end;
				fseek(h, nova_chave*sizeof(int), SEEK_SET);
				fread(&end, sizeof(int), 1, h);
				if(end_foi == -1){ //Caso a nova partição extendida da hash esteja vazia
					fseek(h, nova_chave*sizeof(int), SEEK_SET);
					fwrite(&aux, sizeof(int), 1, h);
					end_foi = end_atual;
				}
				else{
					fseek(r, end_foi*tamanhoEmpregado(), SEEK_SET);
					emp_aux = le_empreg(r);
					emp_aux->prox = end_atual;
					fseek(r, end_foi*tamanhoEmpregado(), SEEK_SET);
					salva_empreg(emp_aux, r);
					end_foi = end_atual;
				}
			}
			else{
				if(end_ficou == -1){ //Caso seja a primeira chave a continuar em p
					fseek(h, nova_chave*sizeof(int), SEEK_SET);
					fwrite(&end_atual, sizeof(int), 1, h);
					end_ficou = end_atual;
				}
				else{
					fseek(r, end_ficou*tamanhoEmpregado(), SEEK_SET);
					emp_aux = le_empreg(r);
					emp_aux->prox = end_atual;
					fseek(r, end_ficou*tamanhoEmpregado(), SEEK_SET);
					salva_empreg(emp_aux, r);
					end_ficou = end_atual;
				}
			}
			nova_chave = end_atual;
			end_atual = emp->prox;
			if(end_atual != -1){ //Continua até o final da lista de p
				fseek(r, emp->prox*tamanhoEmpregado(), SEEK_SET);
				emp = le_empreg(r);
			}
			fseek(r, nova_chave*tamanhoEmpregado(), SEEK_SET);
			emp_aux = le_empreg(r);
			emp_aux->prox = -1;
			fseek(r, nova_chave*tamanhoEmpregado(), SEEK_SET);
			salva_empreg(emp_aux, r);
		}
	}
	*p+=1;
}

int buscarCod(FILE *h, FILE* regts, int cod, int tam, int p, int l){ //retorna o endereço do arquivo de registros
    int end_atual = 0;
	int chave = hash(cod, tam, l);
    
	Empregado* emp = (Empregado*) malloc(tamanhoEmpregado());
	fseek(h, chave*sizeof(int), SEEK_SET);
	if(fread(&end_atual, sizeof(int), 1, h) <= 0){
        printf("Erro ao ler a hash.\n");
        return -1;
    }
    
	fseek(regts, end_atual*tamanhoEmpregado(), SEEK_SET);
	emp = le_empreg(regts);
    
    if(emp == NULL){
        printf("Arquivo de registros vazio\n");
        return -1;
    }
    //printf("emp->cod = %d  emp->status = %d\n", emp->cod, emp->status);
    if(emp->cod == cod && emp->status == 1){
        //printf("if cod e status\n");
        return end_atual;
    }
	else{
        printf("else cod e status\n");
		while(emp->prox != -1){
			fseek(regts, emp->prox*tamanhoEmpregado(), SEEK_SET);
			end_atual = emp->prox;
			emp = le_empreg(regts);
			if(emp->cod == cod && emp->status == 1)
				return end_atual;
		}
	}
	if(chave < p){
		chave = hash(cod, tam, l+1);
		fseek(h, chave*sizeof(int), SEEK_SET);
		fread(&end_atual, sizeof(int), 1, h);
		fseek(regts, end_atual*tamanhoEmpregado(), SEEK_SET);
		emp = le_empreg(regts);
		if(emp->cod == cod && emp->status == 1)
			return end_atual;
		else{
			while(emp->prox != -1){
				fseek(regts, emp->prox*tamanhoEmpregado(), SEEK_SET);
				end_atual = emp->prox;
				emp = le_empreg(regts);
				if(emp->cod == cod && emp->status == 1)
					return end_atual;
			}
		}
	}

	return -1;
}

void excluirHash(FILE *h, FILE *r, FILE *exclusao, int end, int tam, int p, int l, int* qtd_registros){
    int aux,
    chave, 
    excl,
    end_ant;
    Empregado* emp;
    Empregado* prox;
    fseek(r, end*tamanhoEmpregado(), SEEK_SET);    // indo até registro em arq de reg
    emp = le_empreg(r);

    chave = hash(emp->cod, tam, l);

    fseek(h, chave*sizeof(int), SEEK_SET);
    fread(&aux, sizeof(int), 1, h);

    if(aux == end){ //Caso o registro a ser excluido é o primeiro na hash
    	rewind(exclusao);
    	for (int i = 0; fread(&aux, sizeof(int), 1, exclusao) != 0; ++i){ //Verificando onde será salvo no arquivo de excluidos
    		if(aux == -1){
    			excl = i;
    			break;
    		}
    	}
    	if(aux == -1){ //Caso tenha um espaço nos excluidos
    		fseek(exclusao, excl*sizeof(int), SEEK_SET);
    		fwrite(&end, sizeof(int), 1, exclusao);
    		fseek(h, chave*sizeof(int), SEEK_SET);
    		fwrite(&emp->prox, sizeof(int), 1, h);
    		emp->status = -1;
    		fseek(r, end*tamanhoEmpregado(), SEEK_SET);
    		salva_empreg(emp, r);
    	}
    	else{ //Caso tenha q salvar no fim do arquivo de excluidos
    		fseek(exclusao, 0, SEEK_END);
    		fwrite(&end, sizeof(int), 1, exclusao);
    		fseek(h, chave*sizeof(int), SEEK_SET);
    		fwrite(&emp->prox, sizeof(int), 1, h);
    		emp->status = -1;
    		fseek(r, end*tamanhoEmpregado(), SEEK_SET);
    		salva_empreg(emp, r);
    	}
    }
    else{
    	fseek(r, aux*tamanhoEmpregado(), SEEK_SET);
    	prox = le_empreg(r);
    	end_ant = aux;
    	while(prox->prox != -1 && prox->prox != end){ //Procura o anterior ao registro a ser excluido
			fseek(r, emp->prox*tamanhoEmpregado(), SEEK_SET);
			end_ant = emp->prox;
			prox = le_empreg(r);
		}
		if(prox->prox != -1){ //Caso encontrei
			rewind(exclusao);
	    	for (int i = 0; fread(&aux, sizeof(int), 1, exclusao) != 0; ++i){ //Verifica se 
	    		if(aux == -1){
	    			excl = i;
	    			break;
	    		}
	    	}
	    	if(aux == -1){
	    		fseek(exclusao, excl*sizeof(int), SEEK_SET);
	    		fwrite(&end, sizeof(int), 1, exclusao);
	    		fseek(r, end_ant*tamanhoEmpregado(), SEEK_SET);
	    		prox->prox = emp->prox;
	    		salva_empreg(prox, r);
	    		emp->status = -1;
	    		fseek(r, end*tamanhoEmpregado(), SEEK_SET);
	    		salva_empreg(emp, r);
	    	}
	    	else{
	    		fseek(exclusao, 0, SEEK_END);
	    		fwrite(&end, sizeof(int), 1, exclusao);
	    		fseek(r, end_ant*tamanhoEmpregado(), SEEK_SET);
	    		prox->prox = emp->prox;
	    		salva_empreg(prox, r);
	    		emp->status = -1;
	    		fseek(r, end*tamanhoEmpregado(), SEEK_SET);
	    		salva_empreg(emp, r);
	    	}
    	}
    }
    if(chave < p){ //Caso a hash já tenha extendido e o registro se encontra na extensão oq muda é a chave calculada com l+1
    	chave = hash(emp->cod, tam, l+1);

	    fseek(h, chave*sizeof(int), SEEK_SET);
	    fread(&aux, sizeof(int), 1, h);

	    if(aux == end){
	    	rewind(exclusao);
	    	for (int i = 0; fread(&aux, sizeof(int), 1, exclusao) != 0; ++i){
	    		if(aux == -1){
	    			excl = i;
	    			break;
	    		}
	    	}
	    	if(aux == -1){
	    		fseek(exclusao, excl*sizeof(int), SEEK_SET);
	    		fwrite(&end, sizeof(int), 1, exclusao);
	    		fseek(h, chave*sizeof(int), SEEK_SET);
	    		fwrite(&emp->prox, sizeof(int), 1, h);
	    		emp->status = -1;
	    		fseek(r, end*tamanhoEmpregado(), SEEK_SET);
	    		salva_empreg(emp, r);
	    	}
	    	else{
	    		fseek(exclusao, 0, SEEK_END);
	    		fwrite(&end, sizeof(int), 1, exclusao);
	    		fseek(h, chave*sizeof(int), SEEK_SET);
	    		fwrite(&emp->prox, sizeof(int), 1, h);
	    		emp->status = -1;
	    		fseek(r, end*tamanhoEmpregado(), SEEK_SET);
	    		salva_empreg(emp, r);
	    	}
	    }
	    else{
	    	fseek(r, aux*tamanhoEmpregado(), SEEK_SET);
	    	prox = le_empreg(r);
	    	end_ant = aux;
	    	while(prox->prox != -1 && prox->prox != end){
				fseek(r, emp->prox*tamanhoEmpregado(), SEEK_SET);
				end_ant = emp->prox;
				prox = le_empreg(r);
			}
			if(prox->prox != -1){
				rewind(exclusao);
		    	for (int i = 0; fread(&aux, sizeof(int), 1, exclusao) != 0; ++i){
		    		if(aux == -1){
		    			excl = i;
		    			break;
		    		}
		    	}
		    	if(aux == -1){
		    		fseek(exclusao, excl*sizeof(int), SEEK_SET);
		    		fwrite(&end, sizeof(int), 1, exclusao);
		    		fseek(r, end_ant*tamanhoEmpregado(), SEEK_SET);
		    		prox->prox = emp->prox;
		    		salva_empreg(prox, r);
		    		emp->status = -1;
		    		fseek(r, end*tamanhoEmpregado(), SEEK_SET);
		    		salva_empreg(emp, r);
		    	}
		    	else{
		    		fseek(exclusao, 0, SEEK_END);
		    		fwrite(&end, sizeof(int), 1, exclusao);
		    		fseek(r, end_ant*tamanhoEmpregado(), SEEK_SET);
		    		prox->prox = emp->prox;
		    		salva_empreg(prox, r);
		    		emp->status = -1;
		    		fseek(r, end*tamanhoEmpregado(), SEEK_SET);
		    		salva_empreg(emp, r);
		    	}
	    	}
	    }
    }
    (*qtd_registros)--;
}
