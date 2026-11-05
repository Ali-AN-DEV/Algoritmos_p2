#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>

#define K 1000
#define UMBRAL 500.0
#define N_TEST 17
#define NUM_SITUACIONES 3
#define NUM_UMBRALES 3
#define NUM_TAMANOS 10

typedef struct {
    double tiempos[NUM_SITUACIONES][NUM_UMBRALES][NUM_TAMANOS];
    int promedios[NUM_SITUACIONES][NUM_UMBRALES][NUM_TAMANOS];
} ResultadosEj3;

/* Funciones auxiliares: inicializacion y comprobacion de vectores */

void inicializar_semilla(void) {
    srand((unsigned int) time(NULL));
}

void aleatorio(int v[], int n) {
    int i, m;

    m = 2 * n + 1;
    for (i = 0; i < n; i++)
        v[i] = (rand() % m) - n;
}

void ascendente(int v[], int n) {
    int i;

    for (i = 0; i < n; i++)
        v[i] = i;
}

void descendente(int v[], int n) {
    int i;

    for (i = 0; i < n; i++)
        v[i] = n - i;
}

void listarVector(int v[], int n) {
    int i;

    for (i = 0; i < n; i++)
        printf("%d ", v[i]);
    printf("\n");
}

int estaOrdenado(int v[], int n) {
    int i;

    for (i = 1; i < n; i++) {
        if (v[i] < v[i - 1])
            return 0;
    }
    return 1;
}

int *crearVector(int n) {
    int *v;

    if (n <= 0) {
        fprintf(stderr, "El tamano del vector debe ser positivo.\n");
        exit(EXIT_FAILURE);
    }
    v = malloc((size_t) n * sizeof(int));
    if (v == NULL) {
        fprintf(stderr, "Error reservando memoria.\n");
        exit(EXIT_FAILURE);
    }
    return v;
}

/* Ej1: implementacion de la ordenacion por fusion hibrida */

void fusionar(int v[], int izq, int medio, int der, int aux[]) {
    int i, j, k;

    i = izq;
    j = medio + 1;
    k = izq;
    while (i <= medio && j <= der) {
        if (v[i] <= v[j]) {
            aux[k] = v[i];
            k++;
            i++;
        } else {
            aux[k] = v[j];
            k++;
            j++;
        }
    }
    while (i <= medio) {
        aux[k] = v[i];
        k++;
        i++;
    }
    while (j <= der) {
        aux[k] = v[j];
        k++;
        j++;
    }
    for (i = izq; i <= der; i++)
        v[i] = aux[i];
}

void ordenacionInsercionAux(int v[], int izq, int der) {
    int x, i, j;

    for (i = izq + 1; i <= der; i++) {
        x = v[i];
        j = i - 1;
        while (j >= izq && v[j] > x) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = x;
    }
}

void ordenacionFusionHibridaAux(int v[], int izq, int der, int aux[],
                                int umbral) {
    int medio;

    if (izq < der) {
        if (der - izq + 1 <= umbral) {
            ordenacionInsercionAux(v, izq, der);
        } else {
            medio = izq + (der - izq) / 2;
            ordenacionFusionHibridaAux(v, izq, medio, aux, umbral);
            ordenacionFusionHibridaAux(v, medio + 1, der, aux, umbral);
            fusionar(v, izq, medio, der, aux);
        }
    }
}

void ordenacionFusionHibrida(int v[], int tam, int umbral) {
    int *aux;

    if (tam <= 1)
        return;
    if (umbral < 1)
        umbral = 1;
    aux = crearVector(tam);
    ordenacionFusionHibridaAux(v, 0, tam - 1, aux, umbral);
    free(aux);
}

void ej1(void) {
    int v[15];

    aleatorio(v, 15);
    printf("\n=== Ej1: prueba de funcionamiento ===\n");
    printf("Fusion con umbral 3 sobre 15 elementos aleatorios\n");
    listarVector(v, 15);
    printf("ordenando...\n");
    ordenacionFusionHibrida(v, 15, 3);
    listarVector(v, 15);
    printf("%s\n", estaOrdenado(v, 15) ? "esta ordenado" : "ERROR");
}

/* Ej2: validacion del correcto funcionamiento */

void validarFusion(void (*ini)(int[], int), int n, int umbral,
                   const char *nombre) {
    int v[N_TEST];

    printf("Ordenacion con umbral %d e inicializacion %s\n",
           umbral, nombre);
    ini(v, n);
    listarVector(v, n);
    printf("ordenando...\n");
    ordenacionFusionHibrida(v, n, umbral);
    listarVector(v, n);
    printf("%s\n\n", estaOrdenado(v, n) ? "esta ordenado" : "ERROR");
}

void validarUmbral(int umbral, int n) {
    int *v;

    v = crearVector(n);
    aleatorio(v, n);
    ordenacionFusionHibrida(v, n, umbral);
    printf("n = %d, umbral = %3d: %s\n", n, umbral,
           estaOrdenado(v, n) ? "esta ordenado" : "ERROR");
    free(v);
}

void test(void) {
    validarFusion(aleatorio, 15, 1, "aleatoria");
    validarFusion(descendente, 17, 1, "descendente");
    validarFusion(ascendente, 17, 1, "ascendente");
    validarFusion(aleatorio, 15, 3, "aleatoria");
    validarFusion(descendente, 17, 3, "descendente");
    validarFusion(ascendente, 17, 3, "ascendente");
    validarUmbral(1, 1000);
    validarUmbral(20, 1000);
    validarUmbral(200, 1000);
}

void ej2(void) {
    printf("\n=== Ej2: validacion ===\n");
    test();
}

/* Funciones comunes para Ej3 y Ej5: medicion de tiempos */

double microsegundos(void) {
    struct timeval t;

    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return t.tv_sec * 1000000.0 + t.tv_usec;
}

double medir_tiempo(void (*alg)(int[], int, int),
                    void (*ini)(int[], int), int n, int umbral, int *k) {
    int *v, i;
    double ta, tb, ta2, tb2, t, t_ini;

    v = crearVector(n);
    ini(v, n);
    alg(v, n, umbral);              /* Calentamiento */
    ini(v, n);
    ta = microsegundos();
    alg(v, n, umbral);
    tb = microsegundos();
    t = tb - ta;
    *k = 0;
    if (t < UMBRAL) {
        *k = 1;
        ta = microsegundos();
        for (i = 0; i < K; i++) {
            ini(v, n);
            alg(v, n, umbral);
        }
        tb = microsegundos();
        ta2 = microsegundos();
        for (i = 0; i < K; i++)
            ini(v, n);
        tb2 = microsegundos();
        t_ini = tb2 - ta2;
        t = ((tb - ta) - t_ini) / K;
    }
    free(v);
    return t;
}

/* Ej3: tablas separadas por situacion inicial y por umbral */

void ej3(ResultadosEj3 *resultados) {
    void (*inicializaciones[NUM_SITUACIONES])(int[], int) = {
        aleatorio, ascendente, descendente
    };
    const char *nombres[NUM_SITUACIONES] = {
        "aleatoria", "ascendente", "descendente"
    };
    int umbrales[NUM_UMBRALES] = {1, 20, 200};
    int i, u, j, n, k;
    double t;

    printf("\n=== Ej3: tiempos de ejecucion (microsegundos) ===\n");
    for (i = 0; i < NUM_SITUACIONES; i++) {
        printf("\nVector inicializado en orden %s:\n", nombres[i]);
        for (u = 0; u < NUM_UMBRALES; u++) {
            printf("\nUmbral = %d\n", umbrales[u]);
            printf("%10s %13s\n", "v[n]", "t(us)");
            for (n = 1000, j = 0; j < NUM_TAMANOS; j++, n *= 2) {
                t = medir_tiempo(ordenacionFusionHibrida,
                                 inicializaciones[i], n, umbrales[u], &k);
                resultados->tiempos[i][u][j] = t;
                resultados->promedios[i][u][j] = k;
                printf("%c %8d %12.3f\n", k ? '*' : ' ', n, t);
            }
        }
    }
    printf("\n(*) tiempo promedio de K = %d ejecuciones\n", K);
}

/* Ej4: comparacion de umbrales usando los tiempos obtenidos en Ej3 */

void ej4(const ResultadosEj3 *resultados) {
    const char *nombres[NUM_SITUACIONES] = {
        "aleatoria", "ascendente", "descendente"
    };
    int umbrales[NUM_UMBRALES] = {1, 20, 200};
    int i, u, j, n, mejor;

    printf("\n=== Ej4: comparacion de umbrales ===\n");
    for (i = 0; i < NUM_SITUACIONES; i++) {
        printf("\nInicializacion %s\n", nombres[i]);
        printf("%10s %13s %13s %13s %8s\n", "n", "umbral 1",
               "umbral 20", "umbral 200", "mejor");
        for (j = 0, n = 1000; j < NUM_TAMANOS; j++, n *= 2) {
            mejor = 0;
            for (u = 1; u < NUM_UMBRALES; u++) {
                if (resultados->tiempos[i][u][j] <
                    resultados->tiempos[i][mejor][j])
                    mejor = u;
            }
            printf("%10d", n);
            for (u = 0; u < NUM_UMBRALES; u++) {
                printf(" %12.3f%c", resultados->tiempos[i][u][j],
                       resultados->promedios[i][u][j] ? '*' : ' ');
            }
            printf(" %8d\n", umbrales[mejor]);
        }
    }
}

/* Ej5: complejidad empirica (tres umbrales por tres situaciones) */

void tablaFusion(int umbral, void (*ini)(int[], int),
                 const char *nombre) {
    int n, k;
    double t, logn;

    printf("\nOrdenacion con umbral %d e inicializacion %s\n",
           umbral, nombre);
    printf("%10s %13s %14s %17s %19s\n", "n", "t(n)", "t(n)/n",
           "t(n)/(n*ln(n))", "t(n)/(n*ln^2(n))");
    for (n = 1000; n <= 512000; n *= 2) {
        t = medir_tiempo(ordenacionFusionHibrida, ini, n, umbral, &k);
        logn = log((double) n);
        printf("%c %8d %12.3f %14.8f %17.8f %19.8f\n",
               k ? '*' : ' ', n, t, t / n, t / (n * logn),
               t / (n * logn * logn));
    }
}

void ej5(void) {
    void (*inicializaciones[NUM_SITUACIONES])(int[], int) = {
        aleatorio, ascendente, descendente
    };
    const char *nombres[NUM_SITUACIONES] = {
        "aleatoria", "ascendente", "descendente"
    };
    int umbrales[NUM_UMBRALES] = {1, 20, 200};
    int i, u;

    printf("\n=== Ej5: complejidad empirica ===\n");
    for (i = 0; i < NUM_SITUACIONES; i++) {
        for (u = 0; u < NUM_UMBRALES; u++) {
            tablaFusion(umbrales[u], inicializaciones[i], nombres[i]);
        }
    }
    printf("\n(*) tiempo promedio de K = %d ejecuciones\n", K);
}

/* Programa principal: cada ejercicio se ejecuta desde su propia funcion */

int main(void) {
    ResultadosEj3 resultados;

    inicializar_semilla();
    ej1();
    ej2();
    ej3(&resultados);
    ej4(&resultados);
    ej5();
    return EXIT_SUCCESS;
}
