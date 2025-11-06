#include <stdio.h>       // Entrada y salida estándar
#include <stdlib.h>      // Funciones como exit()
#include <unistd.h>      // fork(), getpid(), pipe(), etc.
#include <sys/types.h>   // Definición de tipos como pid_t
#include <sys/ipc.h>     // Para claves de memoria compartida (ftok, IPC_CREAT, etc.)
#include <sys/shm.h>     // Para funciones de memoria compartida (shmget, shmat, etc.)
#include <sys/wait.h>    // Para esperar a procesos hijos (wait)

#define ll long long
#define ld long double

int main(){
    int shmid;               // Identificador del segmento de memoria compartida
    ld *sharedPi;            // Puntero a la memoria compartida
    ll half = 500e6;         // Mitad de las iteraciones (500 millones)
    key_t key = 1234;        // Clave única para identificar la memoria compartida

    // Crear un segmento de memoria compartida
    // shmget crea (o accede) a un segmento de memoria de tamaño sizeof(ld)
    // IPC_CREAT indica que se cree si no existe
    shmid = shmget(key, sizeof(ld), IPC_CREAT | 0666);
    
    //shmid = shmget(key, sizeof(ld), IPC_CREAT) ;

    // Vincular el segmento de memoria compartida al espacio de direcciones del proceso
    sharedPi = (ld*) shmat(shmid, NULL, 0);

    *sharedPi = 0;  // Inicializa el valor compartido

    // Crear un proceso hijo
    pid_t pid = fork();

    if (pid == 0) {  // Proceso hijo

        ld firstTerms = 0;  // Variable para almacenar la primera mitad del cálculo

        // Calcula la primera mitad de la serie de Leibniz
        for (ll i = 0; i < half; i++) {
            if (i & 1) {  // Si i es impar
                firstTerms = firstTerms - (ld)1 / (2 * i + 1);
            } else {      // Si i es par
                firstTerms = firstTerms + (ld)1 / (2 * i + 1);
            }
        }

        *sharedPi = firstTerms;  // Escribe el resultado parcial en la memoria compartida
        shmdt(sharedPi);         // Desvincula la memoria compartida del espacio del hijo
        exit(0);                 // Finaliza el proceso hijo

    } else {  // Proceso padre
        
        wait(NULL);  // Espera a que el proceso hijo termine

        ld pi = *sharedPi;  // Lee el valor parcial calculado por el hijo

        // Calcula la segunda mitad de la serie de Leibniz
        for (ll i = half; i < 2 * half; i++) {
            if (i & 1) {
                pi = pi - (ld)1 / (2 * i + 1);
            } else {
                pi = pi + (ld)1 / (2 * i + 1);
            }
        }

        // Multiplica por 4 para obtener el valor final aproximado de π
        pi = 4 * pi;
        printf("%.10Lf\n", pi);

        // Limpieza: desconecta y elimina el segmento de memoria compartida
        shmdt(sharedPi);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}
/*
Abrir PowerShell → ejecutar `wsl`

Navegar al directorio:

```c
// Ver archivos
ls

// Entrar a una carpeta
cd nombre

// Volver atras
cd ..
```

Compilar:

```c
gcc archivo.c -o programa
```

Ejecutar:

```c
./programa
```
*/

