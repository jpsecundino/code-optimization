#include"fileManagement.h"
#include "listaOrdenada.h"
#include<stdlib.h>
#include<stdio.h>
/**
allocs and initializes a list node an returns it
*/
NODE* getnode(){
	NODE *n;

	n = (NODE*)malloc(sizeof(NODE));
	
	if(n == NULL)
		return NULL;
	
	n->employeeInfo.byteOffSet = 0;
	n->next = NULL; 
	n->prev = NULL;

	return n;
}

LIST* createList(){
	
	LIST *l =(LIST*) malloc (sizeof(LIST));
	
	if(l == NULL)
		return NULL;
	
	l->begin = l->end = NULL;
	l->sz = 0;

	return l;
}

void insertOrderedList(LIST *l, EmployeeRegister *x, long long int byteOffset, int discovery){
		
	NODE *n = getnode();
	if(n == NULL){
		return;
	}
	//fill the node info
	n->employeeInfo.employeeRegister = x;
	n->employeeInfo.byteOffSet = byteOffset;
	n->employeeInfo.employeeRegister->removido = '*';
	n->discovery = discovery;
	//add in the beggining
	if(isListEmpty(l)){
		l->begin = n;
		l->end = l->begin;
		n->employeeInfo.employeeRegister->encadeamentoLista = -1;
		
	}else{
		NODE *m = l->begin;
		
		//find the best position for the new element
		while(m!=NULL){
			
			int regSize = m->employeeInfo.employeeRegister->tamanhoRegistro;
			char mRemoved = m->employeeInfo.employeeRegister->removido;
			
			//if we found a register that was not restored yet and is greater or equal to x
			if(x->tamanhoRegistro <= regSize && mRemoved == '*' ){
					
					while(m != NULL && x->tamanhoRegistro == m->employeeInfo.employeeRegister->tamanhoRegistro && discovery < m->discovery){
						m = m->next;
					}
					
					break;
			}

			m = m->next;
		
		}

		//if we'll put the new element on the list begin
		if(m == l->begin){
			// n--> m--> ...
			n->next = m;
			// || <--n <--m <--...
			m->prev = n;
			n->prev = NULL;
			//new begin
			l->begin = n;
			//change the encadeamentoLista field
			n->employeeInfo.employeeRegister->encadeamentoLista = n->next->employeeInfo.byteOffSet;
		}else if(m == NULL){//if we'll put the new element in the list end
 			// Lend --> n
			l->end->next = n;
			// Lend <-- n
			n->prev = l->end;
			//updates the l->end and n encadeamentoLista fields
			l->end->employeeInfo.employeeRegister->encadeamentoLista = n->employeeInfo.byteOffSet;
			n->employeeInfo.employeeRegister->encadeamentoLista = -1;
			//new end
			l->end = n;
		}else{
			// ..mPrev <--m <--...
			NODE *mPrev = m->prev;
			//change prev encadeamentoLista field
			mPrev->employeeInfo.employeeRegister->encadeamentoLista = n->employeeInfo.byteOffSet;
			// mPrev --> n --> m
			mPrev->next = n;
			n->next = m;
			//change n encadeamentoLista field
			n->employeeInfo.employeeRegister->encadeamentoLista = m->employeeInfo.byteOffSet;
			// prev <-- n <-- m
			m->prev = n;
			n->prev = mPrev;
		}
	}	

	(l->sz)++;
	return;
}

void insertEndList(LIST *l, EmployeeRegister *x, long long int byteOffset){

	NODE *n = getnode();

	if(n == NULL){
		return;
	}

	n->employeeInfo.employeeRegister = x;
	n->employeeInfo.byteOffSet = byteOffset;
	n->discovery = 0;

	if(isListEmpty(l)){
		//adds in the beginning
		//new start
		l->begin = n;
		//new end
		l->end = l->begin;
	}else{
		//adds in the end
		// .. <-- Lend <-- n ..
		n->prev = l->end;
		//Lend --> n --> ||
		l->end->next = n;
		//new end
		l->end = n;
	}
	
	l->sz++;
}

LIST* findInList(LIST *l, Options o, int option){
	if(isListEmpty(l))
		return NULL;
	
	NODE *aux = l->begin;
	LIST *out = createList();
	
	while(aux!=NULL){//search for Register that has the designated field
		EmployeeRegister *e = aux->employeeInfo.employeeRegister;
		if(hasCharacteristic(e,o,option)){
			insertEndList(out, e, aux->employeeInfo.byteOffSet);
			//if the field has a unique value
			if (option == 0) {
				return out;
			}
		}
		aux = aux->next;
	}
	
	if(out->sz){
		return out;
	}else{
		destroyList(out);
		return NULL;
	}
}

NODE *greaterEqualRegisterInList(LIST *l, EmployeeRegister *e){
	if(isListEmpty(l))
		return NULL;
	NODE *aux = l->begin;
	int auxRegisterSize =  aux->employeeInfo.employeeRegister->tamanhoRegistro;

	while(aux != NULL){
			//if our register fits in the removed register
			if(auxRegisterSize > e->tamanhoRegistro){
				//verify if this register has been replaced
				if(aux->employeeInfo.employeeRegister->removido != '-')
					break;
			}
			aux = aux->next;
			if(aux!=NULL)
				auxRegisterSize = aux->employeeInfo.employeeRegister->tamanhoRegistro;
	}
	
	return aux;

}

long long int replaceInfoInList(NODE *correctPlace, EmployeeRegister *e){

	EmployeeRegister *aux = correctPlace->employeeInfo.employeeRegister;
	int oldRegisterSize = aux->tamanhoRegistro; 
	copyRegisterContent(aux, e);
	aux->tamanhoRegistro = oldRegisterSize;
	return correctPlace->employeeInfo.byteOffSet;
}

int removeElemByRef(LIST *l, NODE *x){
	if(l == NULL || x == NULL){
		return 1;
	}
	
	NODE *prev = x->prev;
	NODE *next = x->next;
	
	if(x == l->begin){
		// NULL<--next<--..
		next->prev = NULL;
		// new begin
		l->begin = next;
	}
	else if (x == l->end){
		// ..--> prev -->NULL
		prev->next =NULL;
		//new end
		l->end = prev;
	}
	else{//.. --> prev <--> x <--> next -->..
		//..--> prev --> next
		prev->next = next;
		//..<-- prev <-- next <--..
		next->prev = prev;
	}


	(l->sz)--;
	return 0; 
}


int isListEmpty(LIST *l){
	if(l->sz == 0)
		return 1;
	return 0;
}

void destroyList(LIST *l){
	if(!isListEmpty(l)){
		NODE *aux = l->begin;
		NODE *aux1 = aux->next;
		
		while(aux1!=NULL){
			destroyEmployeeRegister(aux->employeeInfo.employeeRegister);
			free(aux);
			aux = aux1;
			aux1 = aux1->next; 
		}	
		
		destroyEmployeeRegister(aux->employeeInfo.employeeRegister);
		free(aux);
	}
	free(l);
}

void showList(LIST *l){
	if(l == NULL)
		return;
	NODE *n = l->begin;
	
	while(n){
		printf("%d | %lld %lld\n", n->discovery ,n->employeeInfo.byteOffSet,n->employeeInfo.employeeRegister->encadeamentoLista);		
		n = n->next;
	}

	fflush(stdout);
}