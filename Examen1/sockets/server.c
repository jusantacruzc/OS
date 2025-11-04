#include <stdio.h>       // Entrada y salida estándar
#include <stdlib.h>      // Funciones como exit()
#include <string.h>      // Funciones de manejo de cadenas
#include <unistd.h>      // Funciones del sistema (read, close, etc.)
#include <arpa/inet.h>   // Funciones y estructuras para programación con sockets (TCP/IP)

#define ll long long
#define ld long double

int main() {
    int server_fd, client_fd;              // Descriptores del socket del servidor y del cliente
    struct sockaddr_in server_addr, client_addr; // Estructuras para almacenar direcciones IP y puertos
    socklen_t client_len = sizeof(client_addr);  // Tamaño de la estructura del cliente

    // Crear un socket (IPv4 + TCP)
    // AF_INET: familia de direcciones IPv4
    // SOCK_STREAM: protocolo TCP (confiable y orientado a conexión)
    // Protocolo 0: usa el predeterminado para TCP
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");   // Muestra error si falla la creación
        exit(1);
    }

    // Configurar la estructura de dirección del servidor
    server_addr.sin_family = AF_INET;         // Tipo de dirección: IPv4
    server_addr.sin_port = htons(8080);       // Puerto 8080 (htons convierte a formato de red - big endian)
    server_addr.sin_addr.s_addr = INADDR_ANY; // Acepta conexiones desde cualquier IP local

    // Enlazar el socket con la IP y el puerto especificados
    // Esto “reserva” el puerto para que los clientes puedan conectarse
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    // Poner el socket en modo escucha (esperar conexiones entrantes)
    // El parámetro 5 indica el tamaño máximo de la cola de conexiones pendientes
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        exit(1);
    }

    printf("Servidor esperando conexión...\n");

    // Aceptar una conexión entrante de un cliente
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("accept");
        exit(1);
    }

    // Aquí el servidor ya tiene una conexión activa con el cliente
    // Recibe el valor parcial de π enviado por el cliente
    ld pi;
    ll half = 500e6;
    read(client_fd, &pi, sizeof(pi));  // Lee el valor inicial enviado desde el cliente

    // Calcula la segunda mitad de la serie de Leibniz para π
    for (ll i = half; i < 2 * half; i++) {
        if (i & 1) {  // Si i es impar
            pi = pi - (ld)1 / (2 * i + 1);
        } else {      // Si i es par
            pi = pi + (ld)1 / (2 * i + 1);
        }
    }

    // Multiplica por 4 para obtener el valor final aproximado de π
    pi = 4 * pi;
    printf("%.10Lf\n", pi);  // Imprime π con 10 decimales

    // Cierra la conexión con el cliente y el socket del servidor
    close(client_fd);
    close(server_fd);

    return 0;
}
