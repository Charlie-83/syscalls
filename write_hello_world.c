#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Please give a path");
        return 1;
    }
    int fd = open(argv[1],  O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        fprintf(stderr, "Error opening %s", argv[1]);
        return 1;
    }
    char* text = "Hello world";
    ssize_t written = write(fd, text, strlen(text));
    if (written == -1) {
        fprintf(stderr, "Error writing to file %s", argv[1]);
        return 1;
    }
    return 0;
}
