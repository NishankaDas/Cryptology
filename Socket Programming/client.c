#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 3490
#define SHIFT_KEY 3
#define BUF_LEN 1024

void encode_msg(char *text) {
    for (int idx = 0; text[idx]; idx++) {
        if (text[idx] >= 'a' && text[idx] <= 'z')
            text[idx] = (text[idx] - 'a' + SHIFT_KEY) % 26 + 'a';
        else if (text[idx] >= 'A' && text[idx] <= 'Z')
            text[idx] = (text[idx] - 'A' + SHIFT_KEY) % 26 + 'A';
    }
}

void decode_msg(char *text) {
    for (int idx = 0; text[idx]; idx++) {
        if (text[idx] >= 'a' && text[idx] <= 'z')
            text[idx] = (text[idx] - 'a' - SHIFT_KEY + 26) % 26 + 'a';
        else if (text[idx] >= 'A' && text[idx] <= 'Z')
            text[idx] = (text[idx] - 'A' - SHIFT_KEY + 26) % 26 + 'A';
    }
}

int main() {
    int client_fd;
    struct sockaddr_in srv_addr;
    char data_buf[BUF_LEN];

    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(SERVER_PORT);
    srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(client_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    printf("Connected to server\n");

    while (1) {
        printf("Client: ");
        fgets(data_buf, BUF_LEN, stdin);
        encode_msg(data_buf);
        send(client_fd, data_buf, strlen(data_buf), 0);

        int recv_bytes = recv(client_fd, data_buf, BUF_LEN, 0);
        data_buf[recv_bytes] = '\0';
        printf("Encrypted Server: %s", data_buf);
        decode_msg(data_buf);
        printf("Server: %s", data_buf);
    }

    close(client_fd);
    return 0;
}

