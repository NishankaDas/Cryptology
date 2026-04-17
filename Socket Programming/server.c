#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define SERVER_PORT "3490"
#define QUEUE_LIMIT 10
#define SHIFT_VAL 3
#define BUF_SIZE 1024

void encode_text(char *text) {
    for (int idx = 0; text[idx] != '\0'; idx++) {
        if (text[idx] >= 'a' && text[idx] <= 'z')
            text[idx] = (text[idx] - 'a' + SHIFT_VAL) % 26 + 'a';
        else if (text[idx] >= 'A' && text[idx] <= 'Z')
            text[idx] = (text[idx] - 'A' + SHIFT_VAL) % 26 + 'A';
    }
}

void decode_text(char *text) {
    for (int idx = 0; text[idx] != '\0'; idx++) {
        if (text[idx] >= 'a' && text[idx] <= 'z')
            text[idx] = (text[idx] - 'a' - SHIFT_VAL + 26) % 26 + 'a';
        else if (text[idx] >= 'A' && text[idx] <= 'Z')
            text[idx] = (text[idx] - 'A' - SHIFT_VAL + 26) % 26 + 'A';
    }
}

void zombie_handler(int signum) {
    (void)signum;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void *extract_ip(struct sockaddr *addr) {
    if (addr->sa_family == AF_INET)
        return &(((struct sockaddr_in*)addr)->sin_addr);
    return &(((struct sockaddr_in6*)addr)->sin6_addr);
}

int main(void) {
    int server_fd, client_fd;
    struct addrinfo config, *addr_list, *iter;
    struct sockaddr_storage client_addr;
    socklen_t addr_len;
    struct sigaction action;
    int reuse_flag = 1;
    char ip_str[INET6_ADDRSTRLEN];
    char msg_buffer[BUF_SIZE];

    memset(&config, 0, sizeof config);
    config.ai_family = AF_INET;
    config.ai_socktype = SOCK_STREAM;
    config.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, SERVER_PORT, &config, &addr_list);

    for (iter = addr_list; iter != NULL; iter = iter->ai_next) {
        server_fd = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol);
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_flag, sizeof(int));
        if (bind(server_fd, iter->ai_addr, iter->ai_addrlen) == 0) break;
        close(server_fd);
    }

    freeaddrinfo(addr_list);
    listen(server_fd, QUEUE_LIMIT);

    action.sa_handler = zombie_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &action, NULL);

    printf("server: waiting for connections...\n");

    while (1) {
        addr_len = sizeof client_addr;
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);

        inet_ntop(client_addr.ss_family,
                  extract_ip((struct sockaddr *)&client_addr),
                  ip_str, sizeof ip_str);
        printf("server: connection from %s\n", ip_str);

        if (!fork()) {
            close(server_fd);

            while (1) {
                int bytes_read = recv(client_fd, msg_buffer, BUF_SIZE, 0);
                if (bytes_read <= 0) break;

                msg_buffer[bytes_read] = '\0';
                printf("\nEncrypted Client: %s", msg_buffer);
                decode_text(msg_buffer);
                printf("\nDecrypted Client: %s", msg_buffer);

                printf("Server: ");
                fgets(msg_buffer, BUF_SIZE, stdin);
                encode_text(msg_buffer);
                send(client_fd, msg_buffer, strlen(msg_buffer), 0);
            }

            close(client_fd);
            exit(0);
        }
        close(client_fd);
    }
}

