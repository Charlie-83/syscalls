#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char** argv) {
    if (argc < 4) {
        fprintf(stderr, "Please give a path, target text, replacement text");
        return 1;
    }
    int fd = open(argv[1], O_RDWR, 0);
    if (fd == -1) {
        fprintf(stderr, "Error opening %s", argv[1]);
        return 1;
    }
    char* target = argv[2];
    int target_len = strlen(target);
    char* replacement = argv[3];

    ssize_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) {
        fprintf(stderr, "Couldn't seek in file %s", argv[1]);
        return 1;
    }
    char read_buf[file_size];
    char stack_buf[file_size];
    lseek(fd, 0, SEEK_SET);
    ssize_t bytes_read = read(fd, read_buf, file_size);
    lseek(fd, 0, SEEK_SET);
    if (bytes_read == -1) {
        fprintf(stderr, "Error reading %s\n", argv[1]);
        return 1;
    }
    int total_replacements = 0;
    int stack_buf_counter = 0;
    for (int i = 0; i < file_size; ++stack_buf_counter, ++i) {
        if (read_buf[i] != target[0]) {
            stack_buf[stack_buf_counter] = read_buf[i];
            continue;
        }
        bool match = true;
        for (int j = 1; j < target_len; ++j) {
            if (target[j] != read_buf[i+j]) {
                match = false;
                break;
            }
        }
        if (!match) {
            stack_buf[stack_buf_counter] = read_buf[i];
            continue;
        }

        write(fd, stack_buf, stack_buf_counter);
        stack_buf_counter = -1;
        write(fd, replacement, strlen(replacement));
        i += strlen(target) - 1;
        ++total_replacements;
    }
    write(fd, stack_buf, stack_buf_counter);
    ssize_t eof = lseek(fd, 0, SEEK_CUR);
    ftruncate(fd, eof);
    printf("Total replacements: %d", total_replacements);
    return 0;
}
