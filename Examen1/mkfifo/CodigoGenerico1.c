#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    mkfifo("/tmp/mi_fifo", 0666);  // crea la FIFO
    int fd = open("/tmp/mi_fifo", O_WRONLY);
    write(fd, "Hola FIFO", 10);
    close(fd);
}