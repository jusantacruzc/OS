#include <stdio.h>       // Entrada y salida estándar
#include <stdlib.h>      // Funciones como exit()
#include <string.h>      // Funciones de manejo de cadenas
#include <unistd.h>      // Funciones del sistema (write, close, etc.)
#include <arpa/inet.h>   // Funciones y estructuras para sockets TCP/IP (Linux/Unix)

#define ll long long
#define ld long double

int main() {

    ll half = 500e6;     // Mitad del número total de iteraciones (500 millones)
    ld firstTerms = 0;   // Variable para acumular la primera parte de la serie

    // Calcula la primera mitad de la serie de Leibniz para π
    for (ll i = 0; i < half; i++) {
        if (i & 1) {  // Si i es impar
            firstTerms = firstTerms - (ld)1 / (2 * i + 1);
        } else {      // Si i es par
            firstTerms = firstTerms + (ld)1 / (2 * i + 1);
        }
    }

    int sock;                       // Descriptor del socket del cliente
    struct sockaddr_in server_addr; // Estructura con la dirección del servidor

    // Crear un socket (IPv4 + TCP)
    // AF_INET: familia de direcciones IPv4
    // SOCK_STREAM: usa el protocolo TCP
    // Protocolo 0: toma el predeterminado para TCP
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");  // Muestra error si falla la creación del socket
        exit(1);
    }

    // Configurar la dirección del servidor al que se conectará el cliente
    server_addr.sin_family = AF_INET;       // Tipo de dirección: IPv4
    server_addr.sin_port = htons(8080);     // Puerto 8080 (convertido a formato de red)

    // inet_pton convierte una dirección IP en texto ("127.0.0.1")
    // a su forma binaria, que necesita la estructura sockaddr_in
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton");  // Error al convertir la dirección
        exit(1);
    }

    // Conectarse al servidor en la dirección y puerto especificados
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");  // Error si el servidor no está escuchando o no responde
        exit(1);
    }

    // Enviar el valor parcial de π al servidor
    write(sock, &firstTerms, sizeof(firstTerms));

    // Cerrar el socket una vez enviada la información
    close(sock);
    return 0;
}
