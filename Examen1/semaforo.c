// %cflags: -lpthread -lm

/** --compilar con math.h --lm
 ** compilar con threads: -lpthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_PROCESOS 1 //o sea que solo puede entrar 1 hilo a la vez a la seccion

/* Variables para semáforos*/
sem_t *semaforo; //se declara un puntero a un semaforo

struct datos_tipo {
    int data; //num que imprimira el hilo
    int p;    //cuantas repeticiones imprimira
}; //estructura para pasar parametros a cada hilo

void *process(void *datos) //forma tipica, devuelve y acepta void* para pasar cualquier cosa
{
    struct datos_tipo *datos_process; 
    datos_process = (struct datos_tipo *) datos; //se castea?
    int a, i, j, p; //variables locales del hilo
    
    sem_wait(semaforo);
    /*aqui el hilo pide permiso para entrar a la seccion critica, si semaforo>0
    decrece y continua, si es o se queda bloqueado hasta que otro haga sem_post */
    a = datos_process->data; 
    p = datos_process->p;
    //se copian los valores de la estructura a variables locales
    
    for(i = 0; i <= p; i++) {
        printf("%i ", a);
    } //imprime el num a repetido p+1 veces 
    
    fflush(stdout); //fuerza que lo que esta en el buffer s emuestre ya 
    sleep(2); //el hilo duerme 2 segundos
    
    for(i = 0; i <= p; i++) {
        printf("- ");
    } //despues de dormir imprime guiones p+1 veces
    
    fflush(stdout);
    sem_post(semaforo); //libera el recurso, incrementa el semaforo para que otro hilo pueda usarlo
    
    return NULL;
} //todo eso es la funcion que ejecutan los hilos, sieeempre

int main()
{
    int error;
    char *valor_devuelto;
    /* Variables para hilos*/
    struct datos_tipo hilo1_datos, hilo2_datos;
    pthread_t idhilo1, idhilo2, idhilo3, idhilo4;

    hilo1_datos.data = 1;
    hilo2_datos.data = 2;
    hilo1_datos.p = 10;
    hilo2_datos.p = 5;

    semaforo = sem_open("semaforo_name", O_CREAT, 0700, MAX_PROCESOS);

    error = pthread_create(&idhilo1, NULL, process, (void *)(&hilo1_datos));
    if (error != 0) {
        perror("No puedo crear hilo");
        exit(-1);
    }

    error = pthread_create(&idhilo2, NULL, process, (void *)(&hilo2_datos));
    if (error != 0) {
        perror("No puedo crear thread");
        exit(-1);
    }

    pthread_join(idhilo2, (void **)&valor_devuelto);
    pthread_join(idhilo1, (void **)&valor_devuelto);
    //espera a que los hilos terminen

    sem_close(semaforo); //cierra descriptor local del semaforo
    sem_unlink("semaforo_name"); //elimina nombre del semaforo dle sistema
    return 0;
}