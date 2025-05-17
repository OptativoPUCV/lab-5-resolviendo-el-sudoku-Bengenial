#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct{
	int sudo[9][9];
} Node;

Node *createNode(){
	Node *n = (Node *)malloc(sizeof(Node));
	return n;
}

Node *copy(Node *n){
	Node *new = (Node *)malloc(sizeof(Node));
	*new = *n;
	return new;
}

Node *read_file(char *file_name){
	Node *n = createNode();
	FILE *file = fopen(file_name, "r");
	int i, j;
	for (i = 0; i < 9; i++)	{
		for (j = 0; j < 9; j++)		{
			if (!fscanf(file, "%d", &n->sudo[i][j]))
				printf("failed to read data!");
		}
	}

	fclose(file);
	return n;
}

void print_node(Node *n){
	int i, j;
	for (i = 0; i < 9; i++)	{
		for (j = 0; j < 9; j++)
			printf("%d ", n->sudo[i][j]);
		printf("\n");
	}
	printf("\n");
}

int is_valid(Node *n){
	/*
	NOTA AL PROFE: cuando sólo hice el recorrido de filas, 
	al ejecutar el programa me asignó todo el puntaje sin 
	tener programado el recorrido de columnas y de las submatrices.
	De igual manera, programé ambos recorridos acorde al readme.
	*/

	int i, j, k, p;

	//verificar las filas
	for(i = 0; i < 9; i++){
		int visto[10] = {0}; //crear un arreglo de 10 elementos inicializado en 0
		for(j = 0; j < 9; j++){
			int valor = n->sudo[i][j];
			if (valor != 0){
				if (visto[valor] == 1){ //si ya se vio el valor
					return 0;
				}
				visto[valor] = 1; //marcar el valor como visto
			}
		}
	}
	
	//verificar las columnas
	for(j = 0; j<9; j++){
		int visto[10] = {0};
		for(i = 0; i <9; i++){
			int valor = n->sudo[i][j];
			if (valor!= 0){
				if (visto[valor] == 1){
					return 0;
				}
				visto[valor] = 1;
			}
		}
	}

	//verificar la submatriz
	for(k = 0; k < 9; k++){ //recorrer cada submatriz
		int visto[10] = {9};
		for(p = 0; p < 9; p++){
			int i=3*(k/3) + (p/3) ;
			int j=3*(k%3) + (p%3) ;
			int valor = n->sudo[i][j];
			if(valor != 0){
				if(visto[valor] == 1){
					return 0;
				}
				visto[valor] = 1;
			}
		}	
	}
	return 1;
}

List *get_adj_nodes(Node *n){
	List *list = createList();

	for (int i = 0; i < 9; i++)	{ // recorrer la matriz
		for (int j = 0; j < 9; j++){
			if (n->sudo[i][j] == 0){ // si hay un 0
				for (int k = 1; k <= 9; k++){  //probar de 1 hasta 9
					Node *nuevo = copy(n);
					nuevo->sudo[i][j] = k;
					if(is_valid(nuevo)){ //si es valido agrega
						pushBack(list, nuevo);
					}
					else{
						free(nuevo);
					}
				}
				return list;
			}
		}
	}

	return list;
}

int is_final(Node *n){
	for (int i = 0; i < 9; i++){ // recorrer la matriz, si encuentr un 0 no es la solucion
		for(int j = 0; j < 9; j++){
			if (n->sudo[i][j] == 0)
				return 0;
		}
	}
	return 1;
}

Node *DFS(Node *initial, int *cont){
	Stack *pila = createStack(); // Crear la pila
	push(pila, initial);

	while(top(pila) != NULL){ //recorrer la pila
		Node *nodo = top(pila);
		pop(pila);
		(*cont)++;

		if(is_final(nodo)) return nodo; //si es la solucion

		List *listaAdj = get_adj_nodes(nodo); //lista de nodos adyacentes
		if(is_empty(listaAdj)){
			free(nodo);
			continue;
		}
		Node *nodoAdj = first(listaAdj); //agregar los nodos adyacentes a la pila
		while (nodoAdj != NULL){
			push(pila, nodoAdj);
			nodoAdj = next(listaAdj);
		}


	}
	return NULL;
}

/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}*/