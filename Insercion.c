#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>

#define UMBRAL 500.0 
#define K 1000

//Inicializar y verificar vectores
void inicializar_semilla() {
		srand(time(NULL));
}

void aleatorio(int v[], int n) {
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

void listarVector(int v[], int n){
		int i;
		for (i = 0; i < n; i++)
				printf("%d ", v[i]);
		printf("\n");
}

int estaOrdenado (int v[], int n){
		int i;
		for(i = 1; i < n; i++){
				if(v[i] < v[i - 1])
						return 0;
		}
		return 1;
}

//Aplicar inserción
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

void ordenacionInsercion (int v[], int tam){
		ordenacionInsercionAux (v, 0, tam - 1);
}

//Función test del Ej2

void test(){
		int v[10];
		inicializar_semilla();

		printf("Ordenación por inserción con inicialización aleatoria\n");
		aleatorio(v, 10);
		listarVector(v, 10);
		printf("Ordenando...\n");
		ordenacionInsercion(v, 10);
		listarVector(v, 10);
		if (estaOrdenado(v, 10))
				printf("El vector está ordenado\n\n");
		else
				printf("ERROR\n\n");
		printf("Ordenación por inserción con con inicialización descendiente\n");	
		descendente(v, 10);
		listarVector(v, 10);
		printf("Ordenando...\n");
		ordenacionInsercion(v, 10);	 
		listarVector(v, 10);
		if (estaOrdenado(v, 10))
				printf("El vector está ordenado\n\n");
		else
				printf("ERROR\n\n");
		printf("Ordenación por inserción con con inicialización ascendiente\n");	
		ascendente(v, 10);
		listarVector(v, 10);
		printf("Ordenando...\n");
		ordenacionInsercion(v, 10);	 
		listarVector(v, 10);
		if (estaOrdenado(v, 10))
				printf("El vector está ordenado\n");
		else
				printf("ERROR\n");		
}

//Funciones para la medición de tiempos del Ej3

double microsegundos() {
		struct timeval t;
		if (gettimeofday(&t, NULL) < 0 )
				return 0.0;
		return (t.tv_usec + t.tv_sec * 1000000.0);
}

double medir_tiempo(void (*alg)(int [], int),void (*ini)(int [], int), int n, int *k) {
		int *v, i;
		double ta, tb, ta2, tb2, t;
		v = malloc(n * sizeof(int));
		ini(v, n);
		ta = microsegundos();
		alg(v, n);
		tb = microsegundos();
		t = tb - ta;

		if (t < 500){
				*k = 1;
				ta = microsegundos();
				for (i = 0; i < K; i++) {
						ini(v, n);
						alg(v, n);
				}
				tb = microsegundos();
				ta2 = microsegundos();
				for (i = 0; i < K; i++) {
						ini(v, n);
				}
				tb2 = microsegundos();
				t = ((tb - ta) - (tb2 - ta2)) / K;
		}else{
				*k = 0;
		}
		free(v);
		return t;
}

void ej3(){
		int k, n;
		double t;
		inicializar_semilla();
		printf("\nEj3 - tiempos en microsegundos\n");
		printf("\nVector inicializado aleatoriamente:\n");
		printf("%10s %13s\n", "v[n]", "t(µs)");		
		for (n = 500; n <= 32000; n *= 2) {
				t = medir_tiempo(ordenacionInsercion, aleatorio, n, &k);
				if (k)
						printf("* %8d %12.3f\n", n, t);
				else
						printf("  %8d %12.3f\n", n, t);
		}
		printf("\nVector inicializado en orden ascendente:\n");
		printf("%10s %13s\n", "v[n]", "t(µs)");
		for (n = 500; n <= 32000; n *= 2) {
				t = medir_tiempo(ordenacionInsercion, ascendente, n, &k);
				if (k)
						printf("* %8d %12.3f\n", n, t);
				else
						printf("  %8d %12.3f\n", n, t);
		}
		printf("\nVector inicializado en orden descendente:\n");
		printf("%10s %13s\n", "v[n]", "t(µs)");
		for (n = 500; n <= 32000; n *= 2) {
				t = medir_tiempo(ordenacionInsercion, descendente, n, &k);		
				if (k)
						printf("* %8d %12.3f\n", n, t);
				else
						printf("  %8d %12.3f\n", n, t);
		}
		printf("\n* tiempo promedio de K ejecuciones\n");
}

//Función para el análisis empírico del Ej4

void ej4(){
		int v[] = {500, 1000, 2000, 4000, 8000, 16000, 32000};
		int i, n, k;
		double t;
		printf("\nTabla 1: Vector inicializado aleatoriamente - cotas t/n^1.8 (sub), t/n^2 (aj), t/n^2.2 (sobre)\n");
		printf("\n%10s%15s%2s%16s%16s%16s\n", "n", "t(n)", "", "t/n^1.8", "t/n^2", "t/n^2.2");
		for (i = 0; i < 7; i++) {
				n = v[i];
				t = medir_tiempo(ordenacionInsercion, aleatorio, n, &k);
				printf("%c %8d %14.3f %17.9f %15.9f %15.9f\n", k ? '*' : ' ', n, t, t / pow(n, 1.8), t / pow(n, 2), t / pow(n, 2.2));
		}
		printf("\n* tiempo promedio de K ejecuciones\n");

		printf("\nTabla 2: Vector inicializado en orden ascendente - cotas t/n^1.8 (sub), t/n^2 (aj), t/n^2.2 (sobre)\n");
		printf("\n%10s%15s%2s%16s%16s%16s\n", "n", "t(n)", "", "t/n^0.8", "t/n", "t/n^1.2");
		for (i = 0; i < 7; i++) {
				n = v[i];
				t = medir_tiempo(ordenacionInsercion, ascendente, n, &k);
				printf("%c %8d %14.3f %17.9f %15.9f %15.9f\n", k ? '*' : ' ', n, t, t / pow(n, 0.8), t / n, t / pow(n, 1.2));
		}
		printf("\n* tiempo promedio de K ejecuciones\n");

		printf("\nTabla 3: Vector inicializado en orden descendente - cotas t/n^1.8 (sub), t/n^2 (aj), t/n^2.2 (sobre)\n");
		printf("\n%10s%15s%2s%16s%16s%16s\n", "n", "t(n)", "", "t/n^1.8", "t/n^2", "t/n^2.2");
		for (i = 0; i < 7; i++) {
				n = v[i];
				t = medir_tiempo(ordenacionInsercion, descendente, n, &k);
				printf("%c %8d %14.3f %17.9f %15.9f %15.9f\n", k ? '*' : ' ', n, t, t / pow(n, 1.8), t / pow(n, 2), t / pow(n, 2.2));
		}
		printf("\n* tiempo promedio de K ejecuciones\n");		
}

int main(){
		ej3();
		return 0;
}
