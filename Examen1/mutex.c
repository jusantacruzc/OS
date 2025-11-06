/* Ejemplo de manejo de hilos
 * compilar con threads: -lpthread
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_PROCESOS 1
#define NUM_HILOS 8

// Mutex global
pthread_mutex_t mutex;

struct datos_tipo {
    int dato;
    int p;
};

void *proceso(void *datos) {
    struct datos_tipo *datos_proceso;
    datos_proceso = (struct datos_tipo *) datos;
    int a, i, p;

    a = datos_proceso->dato;
    p = datos_proceso->p;

    // 🔒 Bloquear el mutex antes de la sección crítica
    pthread_mutex_lock(&mutex);

    // Seccion critica --->
    for (i = 0; i <= p; i++) {
        printf("%i ", a);
    }
    fflush(stdout);
    sleep(1);
    for (i = 0; i <= p; i++) {
        printf("- ");
    }
    fflush(stdout);
    // --->

    // 🔓 Desbloquear el mutex
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main() {
    int error, i;
    char *valor_devuelto;

     // Inicializar el mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Error al inicializar el mutex\n");
        return 1;
    }

    /* Variables para hilos */
    struct datos_tipo hilo_datos[NUM_HILOS];
    pthread_t idhilo[NUM_HILOS];

    for (i = 0; i < NUM_HILOS; i++) {
        hilo_datos[i].dato = i;
        hilo_datos[i].p = i + 1;
    }

    /* Se lanzan los hilos */
    for (i = 0; i < NUM_HILOS; i++) {
        error = pthread_create(&idhilo[i], NULL, proceso, (void *)(&hilo_datos[i]));
        if (error != 0) {
            perror("No puedo crear hilo");
            exit(-1);
        }
    }

    /* Esperar a que terminen */
    for (i = 0; i < NUM_HILOS; i++) {
        pthread_join(idhilo[i], (void **)&valor_devuelto);
    }

    // Destruir el mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}