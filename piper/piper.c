// piper
// A program that redirects stdin and stdout of this program to another process

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define BUFFER_SIZE 128

char recv_buffer[BUFFER_SIZE];
char send_buffer[BUFFER_SIZE];

void send_line(void) {
    fprintf(stderr, "> %s", send_buffer);
    fprintf(stdout, "> %s", send_buffer);
    fflush(stdout);
}

void recv_line(void) {
    fgets(recv_buffer, BUFFER_SIZE, stdin);
    fprintf(stderr, "< %s", recv_buffer);
}

void skip_lines(int n) {
    for (int i = 0; i < n; i++) {
        recv_line();
    }
}

int main() {
    // Set up pipes
    int from_server[2], to_server[2];
    pipe(from_server); pipe(to_server);

    if (!fork()) {
        // Child: Will exec nc to server
        close(from_server[0]);
        close(to_server[1]);
        dup2(from_server[1], 1);
        dup2(to_server[0], 0);
        close(from_server[1]);
        close(to_server[0]);
        execlp("nc", "nc", "localhost", "31905", NULL);
    } else {
        // Parent: Will solve problems here
        close(from_server[1]);
        close(to_server[0]);
        dup2(to_server[1], 1);
        dup2(from_server[0], 0);
        close(from_server[0]);
        close(to_server[1]);
    }

    skip_lines(1);
}
