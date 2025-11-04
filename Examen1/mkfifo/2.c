#include <stdio.h>      // Librería estándar de entrada y salida
#include <stdlib.h>     // Para funciones como exit()
#include <unistd.h>     // Para funciones del sistema POSIX (read, close)
#include <fcntl.h>      // Para constantes de control de archivos (O_RDONLY, etc.)
#include <sys/stat.h>   // Para funciones relacionadas con permisos y tipos de archivo

#define ll long long     // Atajo para escribir long long
#define ld long double   // Atajo para escribir long double

int main(){
    const char* fifo = "/tmp/pi_fifo";  // Ruta del archivo FIFO
    struct stat st;                     // Estructura para verificar estado del archivo

    // Verifica si el FIFO ya existe
    if (stat(fifo, &st) == -1) {
        // Si no existe, lo crea con permisos 0666 (lectura/escritura para todos)
        if (mkfifo(fifo, 0666) == -1) {
            perror("mkfifo");  // Muestra error si falla la creación
            exit(1);
        }
    }

    ll half = 500e6;   // Mitad del número de iteraciones (500 millones)
    ld pi = 0.0;       // Variable para acumular el valor de π

    // Abre el FIFO para lectura
    int fd = open(fifo, O_RDONLY);
    if (fd == -1) {
        perror("Error opening FIFO for reading");
        exit(1);
    }

    // Lee el valor parcial de π enviado por otro proceso
    ssize_t bytesRead = read(fd, &pi, sizeof(pi));
    close(fd);  // Cierra el FIFO

    // Verifica que se haya leído el número de bytes esperado
    if (bytesRead != sizeof(pi)) {
        fprintf(stderr, "Error: expected %zu bytes but got %zd\n",
                sizeof(pi), bytesRead);
        //unlink(fifo); // (opcional) eliminar el FIFO si hay error
        exit(1);
    }

    // Calcula la segunda mitad de la serie de Leibniz para π
    for(ll i = half; i < 2*half; i++){
        if(i & 1){  // Si i es impar
            pi = pi - (ld)1/(2*i + 1);
        } else {    // Si i es par
            pi = pi + (ld)1/(2*i+1);
        }
    }

    // Multiplica por 4 para obtener el valor final aproximado de π
    pi = 4*pi;
    printf("%.10Lf\n", pi);  // Imprime π con 10 decimales
    return 0;
}
