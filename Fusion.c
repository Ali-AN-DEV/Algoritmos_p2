#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>

#define K 1000
#define UMBRAL 500.0 

//Aplicacion del algoritmo

void fusionar (int v[], int izq, int medio,int der, int aux[]){
	int i, j, k;
	i = izq;
	j = medio + 1;
	k = izq;
	while(i <= medio && j <= der){
		if(v[i] <= v[j]){
			aux[k] = v[i];
			k = k + 1;
			i = i + 1;
		} else{
			aux[k] = v[j];
			k = k + 1;
			j = j + 1;
		}
	}
	while(i <= medio){
		aux[k] = v[i];
		k =  k +1;
		i = i + 1; 
	}
	while(j <= der){
		aux[k] = v[j];
		k =  k +1;
		j = j + 1; 
	}
	for(i = izq; i <= der; i++){
		v[i] = aux[i];
	}
}

void ordenacionInsercionAux (int v[], int izq, int der){
		int x, i, j;
		for (i = izq + 1; i <=  der; i++){
				x = v[i];
				j = i - 1;
				while (j >= izq && v[j] > x){
						v[j +1] = v[j];
						j--;
				}
				v[j + 1] = x;
		} 
}

void ordenacionFusionHibridaAux(int v[], int izq, int der, int aux[], int umbral){
	int medio;
	if (izq < der){
		if ((der - izq + 1) <= umbral){
			ordenacionInsercionAux(v, izq, der);
		} else{
			medio = izq + (der - izq) / 2;
			ordenacionFusionHibridaAux(v, izq, medio, aux, umbral);
			ordenacionFusionHibridaAux(v, medio + 1, der, aux, umbral);
			fusionar(v, izq, medio, der, aux);
		}
	}
}

int *crearVector(int n){
    int *v;

    v = malloc(n * sizeof(int));

    if (v == NULL){
        printf("Error reservando memoria\n");
        exit(EXIT_FAILURE);
    }

    return v;
}

void ordenacionFusionHibrida(int v[], int n, int umbral){
    int *aux;

    aux = crearVector(n);

    ordenacionFusionHibridaAux(v, 0, n - 1, aux, umbral);

    free(aux);
}

//Inicializar los vectores

void inicializar_semilla() {
		srand(time(NULL));
}


void aleatorio(int v [], int n) {
		int i, m = 2 * n + 1;
		for (i = 0; i < n; i++)
				v[i] = (rand() % m) - n;
}

void ascendente(int v[], int n) {
		int i;
		for (i=0; i < n; i++)
		v[i] = i;
}

void descendente(int v[], int n) {
		int i;
		for (i = 0; i < n; i++)
		v[i] = n - i;
}

void listarVector(int v [], int n){
		int i;
		for (i = 0; i < n; i++)
				printf("%d ", v[i]);
		printf("\n");
}

//Función test del Ej2

void test(){
	int v[15];
	inicializar_semilla();
	aleatorio(v, 15);
	listarVector(v, 15);
	ordenacionFusionHibrida(v, 15, 3);
	listarVector(v, 15);
}

void ej3(){
}

int main(){
	test();
}
