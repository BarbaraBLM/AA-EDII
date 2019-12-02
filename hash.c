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

void imprime_reg(FILE* r, int c){
    rewind(r);
    if(c == 1){
        Empregado* e = (Empregado*) malloc(tamanhoEmpregado());
        imprime_empreg(le_empreg(r));
    }
    else if(c == 2){
        Dependente* e = (Dependente*) malloc(tamanhoDependente());
        imprime_dep(le_dep(r));
    }
    else    printf("Opção inválida.\n");
}

int countExc(FILE* excl){
    int aux = 0, cont=0;
    while(fread(&aux, sizeof(int), 1, excl) > 0){
        if(aux!=-1)    cont++;
    }
    return cont;
}

void percorrendo_lista(FILE* h, FILE* reg, int r_hash, int c){
    if(c == 1){
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
    else if(c == 2){
        Dependente* d = (Dependente*) malloc(tamanhoDependente());
        int aux = -1;
        fseek(h, r_hash*sizeof(int), SEEK_SET);
        fread(&aux, sizeof(int), 1, h);

        fseek(reg, aux*tamanhoDependente(), SEEK_SET);
        d = le_dep(reg);

        if(d->status != -1)   imprime_dep(d);
        while(d->prox != -1){
            fseek(reg, d->prox*tamanhoDependente(), SEEK_SET);
            d = le_dep(reg);
            if(d->status != -1)    imprime_dep(d);
        } 
        free(d);
    }
    else
        printf("Opção inválida.\n");
}

void inserirHashEmp(FILE *h, FILE *r, FILE *exclusao, Empregado *emp, int tam, int p, int l, int *qtd_registros){
	int chave = hash(emp->cod, tam, l);
	int excl, aux, end_excl = -1;
	rewind(exclusao);
	if(chave < p){
		chave = hash(emp->cod, tam, l+1);
	}
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

void inserirHashDep(FILE *h, FILE *r, FILE *exclusao, Dependente *dep, int tam, int p, int l, int *qtd_registros){
	int chave = hash(dep->cod, tam, l);
	int excl, aux, end_excl = -1;
	rewind(exclusao);
	if(chave < p){
		chave = hash(dep->cod, tam, l+1);
	}
	while(fread(&excl, sizeof(int), 1, exclusao) != 0){ //Procurar caso tenha algum registro excluido
		end_excl++;
		if(excl != -1) break;
	}
	fseek(h, chave*sizeof(int), SEEK_SET);
	fread(&aux, sizeof(int), 1, h);
	if(aux == -1){ //Caso a chave da hash esteja vazia
		fseek(h, chave*tamanhoDependente(), SEEK_SET);
		if(excl != -1){ //Caso tenha um registro excluido
			fseek(r, excl*tamanhoDependente(), SEEK_SET);
			salva_dep(dep, r);
			fseek(h, chave*sizeof(int), SEEK_SET);
			fwrite(&excl, sizeof(int), 1, h);
			fseek(exclusao, end_excl*sizeof(int), SEEK_SET);
			fwrite(&end_excl, sizeof(int), 1, exclusao);
		}
		else{
			fseek(r, 0, SEEK_END);
			salva_dep(dep, r);
			fseek(h, chave*sizeof(int), SEEK_SET);
			fwrite(&(*qtd_registros), sizeof(int), 1, h);
			*qtd_registros += 1;
		}
	}
	else{
		Dependente *dep_aux;
		int end_atual = aux;
		fseek(r, aux*tamanhoDependente(), SEEK_SET);
		dep_aux = le_dep(r);
		while(dep_aux->prox != -1){ //Vai até o final da lista encadeada
			fseek(r, dep_aux->prox*tamanhoDependente(), SEEK_SET);
			end_atual = dep_aux->prox;
			dep_aux = le_dep(r);
		}
		if(excl != -1){ //Caso tenha um registro excluido
			fseek(r, excl*tamanhoDependente(), SEEK_SET);
			salva_dep(dep, r);
			dep_aux->prox = excl;
			fseek(r, end_atual*tamanhoDependente(), SEEK_SET);
			salva_dep(dep_aux, r);
			fseek(exclusao, end_excl*sizeof(int), SEEK_SET);
			fwrite(&end_excl, sizeof(int), 1, exclusao);
		}
		else{
			fseek(r, 0, SEEK_END);
			salva_dep(dep, r);
			dep_aux->prox = *qtd_registros;
			fseek(r, end_atual*tamanhoDependente(), SEEK_SET);
			salva_dep(dep_aux, r);
			*qtd_registros += 1;
		}
	}
}


void expandHashEmp(FILE *h, FILE *r, int tam, int* p, int l){
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

void expandHashDep(FILE *h, FILE *r, int tam, int* p, int l){
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

		Dependente *dep;
		int nova_chave;
		int end_atual = aux;
		int end_foi = -1;
		int end_ficou = -1;
		//Lê o registro q estava em p
		fseek(r, aux*tamanhoDependente(), SEEK_SET);
		dep = le_dep(r);
		while(end_atual != -1){
			Dependente *dep_aux;
			nova_chave = hash(dep->cod, tam, l+1);
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
					fseek(r, end_foi*tamanhoDependente(), SEEK_SET);
					dep_aux = le_dep(r);
					dep_aux->prox = end_atual;
					fseek(r, end_foi*tamanhoDependente(), SEEK_SET);
					salva_dep(dep_aux, r);
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
					fseek(r, end_ficou*tamanhoDependente(), SEEK_SET);
					dep_aux = le_dep(r);
					dep_aux->prox = end_atual;
					fseek(r, end_ficou*tamanhoDependente(), SEEK_SET);
					salva_dep(dep_aux, r);
					end_ficou = end_atual;
				}
			}
			nova_chave = end_atual;
			end_atual = dep->prox;
			if(end_atual != -1){ //Continua até o final da lista de p
				fseek(r, dep->prox*tamanhoDependente(), SEEK_SET);
				dep = le_dep(r);
			}
			fseek(r, nova_chave*tamanhoDependente(), SEEK_SET);
			dep_aux = le_dep(r);
			dep_aux->prox = -1;
			fseek(r, nova_chave*tamanhoDependente(), SEEK_SET);
			salva_dep(dep_aux, r);
		}
	}
	*p+=1;
}


int buscarCodEmp(FILE *h, FILE* regts, int cod, int tam, int p, int l){ //retorna o endereço do arquivo de registros
	int end_atual;
	int chave = hash(cod, tam, l);
	Empregado* emp;
	if(chave < p){
		chave = hash(emp->cod, tam, l+1);
	}
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

	return -1;
}

int buscarCodDep(FILE *h, FILE* regts, int cod, int tam, int p, int l){ //retorna o endereço do arquivo de registros
	int end_atual;
	int chave = hash(cod, tam, l);
	Dependente* dep;
	if(chave < p){
		chave = hash(dep->cod, tam, l+1);
	}
	fseek(h, chave*sizeof(int), SEEK_SET);
	fread(&end_atual, sizeof(int), 1, h);
	fseek(regts, end_atual*tamanhoDependente(), SEEK_SET);
	dep = le_dep(regts);
	if(dep->cod == cod && dep->status == 1)
		return end_atual;
	else{
		while(dep->prox != -1){
			fseek(regts, dep->prox*tamanhoDependente(), SEEK_SET);
			end_atual = dep->prox;
			dep = le_dep(regts);
			if(dep->cod == cod && dep->status == 1)
				return end_atual;
		}
	}

	return -1;
}

void excluirHashEmp(FILE *h, FILE *r, FILE *exclusao, int end, int tam, int p, int l, int *qtd_registros){
    int aux,
    chave, 
    excl,
    end_ant;
    Empregado* emp;
    Empregado* prox;

    fseek(r, end*tamanhoEmpregado(), SEEK_SET);    // indo até registro em arq de reg
    emp = le_empreg(r);

    chave = hash(emp->cod, tam, l);
    if(chave < p){
		chave = hash(emp->cod, tam, l+1);
	}
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
    *qtd_registros -=1;
}

void excluirHashDep(FILE *h, FILE *r, FILE *exclusao, int end, int tam, int p, int l, int *qtd_registros){
    int aux,
    chave, 
    excl,
    end_ant;
    Dependente* dep;
    Dependente* prox;

    fseek(r, end*tamanhoDependente(), SEEK_SET);    // indo até registro em arq de reg
    dep = le_dep(r);

    chave = hash(dep->cod, tam, l);
    if(chave < p){
		chave = hash(dep->cod, tam, l+1);
	}
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
    		fwrite(&dep->prox, sizeof(int), 1, h);
    		dep->status = -1;
    		fseek(r, end*tamanhoDependente(), SEEK_SET);
    		salva_dep(dep, r);
    	}
    	else{ //Caso tenha q salvar no fim do arquivo de excluidos
    		fseek(exclusao, 0, SEEK_END);
    		fwrite(&end, sizeof(int), 1, exclusao);
    		fseek(h, chave*sizeof(int), SEEK_SET);
    		fwrite(&dep->prox, sizeof(int), 1, h);
    		dep->status = -1;
    		fseek(r, end*tamanhoDependente(), SEEK_SET);
    		salva_dep(dep, r);
    	}
    }
    else{
    	fseek(r, aux*tamanhoDependente(), SEEK_SET);
    	prox = le_dep(r);
    	end_ant = aux;
    	while(prox->prox != -1 && prox->prox != end){ //Procura o anterior ao registro a ser excluido
			fseek(r, dep->prox*tamanhoDependente(), SEEK_SET);
			end_ant = dep->prox;
			prox = le_dep(r);
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
	    		fseek(r, end_ant*tamanhoDependente(), SEEK_SET);
	    		prox->prox = dep->prox;
	    		salva_dep(prox, r);
	    		dep->status = -1;
	    		fseek(r, end*tamanhoDependente(), SEEK_SET);
	    		salva_dep(dep, r);
	    	}
	    	else{
	    		fseek(exclusao, 0, SEEK_END);
	    		fwrite(&end, sizeof(int), 1, exclusao);
	    		fseek(r, end_ant*tamanhoDependente(), SEEK_SET);
	    		prox->prox = dep->prox;
	    		salva_dep(prox, r);
	    		dep->status = -1;
	    		fseek(r, end*tamanhoDependente(), SEEK_SET);
	    		salva_dep(dep, r);
	    	}
    	}
    }
    *qtd_registros -=1;
}
