#include <stdio.h>      // Entrada y salida estándar
#include <stdlib.h>     // Funciones como exit()
#include <unistd.h>     // Funciones POSIX (write, close)
#include <fcntl.h>      // Constantes para control de archivos (O_WRONLY, etc.)
#include <sys/stat.h>   // Funciones y estructuras para manejo de archivos especiales

#define ll long long     // Atajo para long long
#define ld long double   // Atajo para long double

int main(){
    const char* fifo = "/tmp/pi_fifo";  // Ruta del archivo FIFO
    struct stat st;                     // Estructura para verificar existencia del FIFO

    // Si el FIFO no existe, se crea con permisos de lectura/escritura para todos
    if (stat(fifo, &st) == -1) {
        if (mkfifo(fifo, 0666) == -1) {
            perror("mkfifo");  // Muestra error si falla la creación
            exit(1);
        }
    }

    ll half = 500e6;    // Mitad del número total de iteraciones (500 millones)
    ld firstTerms = 0;  // Variable para acumular la primera parte de la serie

    // Calcula la primera mitad de la serie de Leibniz para π
    for(ll i = 0; i < half; i++){
        if(i & 1){  // Si i es impar
            firstTerms = firstTerms - (ld)1/(2*i + 1);
        } else {    // Si i es par
            firstTerms = firstTerms + (ld)1/(2*i+1);
        }
    }
    
    // Abre el FIFO en modo escritura
    int fd = open(fifo, O_WRONLY);
    // Escribe el valor parcial de π en el FIFO
    write(fd, &firstTerms, sizeof(firstTerms));
    close(fd);  // Cierra el FIFO

    return 0;
}
