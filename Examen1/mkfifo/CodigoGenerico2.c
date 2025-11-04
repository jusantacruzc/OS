#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("/tmp/mi_fifo", O_RDONLY);
    char buf[20];
    read(fd, buf, sizeof(buf));
    printf("Leído: %s\n", buf);
    close(fd);
}
