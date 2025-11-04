#include <stdio.h>      // printf, perror
#include <stdlib.h>     // malloc, free, exit
#include <string.h>     // funciones de cadena (no se usan, pero no estorba)
#include <unistd.h>     // fork, pipe, read, write, close
#define ll long long
#define ld long double

int main() {
    int fd[2];          // Array con dos descriptores de archivo: [0] lectura, [1] escritura
    char buffer[5];     // (no se usa en este ejemplo, podría eliminarse)
    int r;              
    pid_t pid;
    ll half = 500e6;    // Mitad del número de términos a calcular

    // Crea una tubería anónima (pipe) para comunicación entre procesos
    // fd[0] será el extremo de lectura
    // fd[1] será el extremo de escritura
    r = pipe(fd);

    pid = fork();       // Crea un nuevo proceso (hijo)

    if (pid == 0) {   // --- PROCESO HIJO ---
        close(fd[0]); // El hijo no necesita leer, solo escribir

        ld firstTerms = 0;

        // Calcula la primera mitad de la serie de Leibniz para π
        for(ll i = 0; i < half; i++){
            if(i & 1){
                firstTerms = firstTerms - (ld)1/(2*i + 1);
            } else {
                firstTerms = firstTerms + (ld)1/(2*i + 1);
            }
        }
        
        // Escribe el resultado parcial en la tubería
        write(fd[1], &firstTerms, sizeof(firstTerms));

        // Cierra el extremo de escritura y finaliza
        close(fd[1]);
        exit(0);

    } else {   // --- PROCESO PADRE ---
        close(fd[1]); // El padre no escribe, solo lee

        ld pi;

        // Lee el valor parcial calculado por el hijo desde la tubería
        read(fd[0], &pi, sizeof(pi));
        close(fd[0]); // Cierra el extremo de lectura

        // Calcula la segunda mitad de la serie
        for(ll i = half; i < 2*half; i++){
            if(i & 1){
                pi = pi - (ld)1/(2*i + 1);
            } else {
                pi = pi + (ld)1/(2*i + 1);
            }
        }

        // Multiplica por 4 para obtener una aproximación de π
        pi = 4 * pi;

        // Imprime el resultado con 10 decimales
        printf("%.10Lf\n", pi);
    }

    return 0;
}
