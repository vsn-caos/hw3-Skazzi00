#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <ipv4_addr> <port>\n", argv[0]);
        return 1;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)atoi(argv[2]));
    if (inet_aton(argv[1], &addr.sin_addr) == 0) {
        fprintf(stderr, "Invalid address: %s\n", argv[1]);
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        return 1;
    }

    int32_t n;
    while (scanf("%d", &n) == 1) {
        if (send(sockfd, &n, sizeof(n), 0) < 0) {
            perror("send");
            break;
        }

        int32_t resp;
        ssize_t r = recv(sockfd, &resp, sizeof(resp), MSG_WAITALL);
        if (r <= 0) {
            close(sockfd);
            return 0;
        }

        printf("%d\n", resp);
        fflush(stdout);
    }

    close(sockfd);
    return 0;
}
