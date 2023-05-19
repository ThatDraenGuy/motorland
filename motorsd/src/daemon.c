#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>

#define SOCKET_PATH "/tmp/motorsd_socket"
#define CODE_WORD "secret"

void handleCodeWordReceived() {
    printf("Code word received!\n");
    // Do something when the code word is received
}

void signalHandler(int signum) {
    if (signum == SIGTERM) {
        printf("Received SIGTERM, shutting down.\n");
        exit(EXIT_SUCCESS);
    }
}

int main() {
    // Register signal handler for graceful shutdown
    signal(SIGTERM, signalHandler);

    // Create a Unix socket
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up socket address structure
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // Remove the socket file if it already exists
    unlink(SOCKET_PATH);

    // Bind the socket to the specified address
    if (bind(sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, 1) < 0) {
        perror("listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Listening for connections on %s\n", SOCKET_PATH);

    // Accept incoming connections and process data
    while (1) {
        int clientfd = accept(sockfd, NULL, NULL);
        if (clientfd < 0) {
            perror("accept");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        char buffer[sizeof(CODE_WORD)];
        ssize_t bytesRead = recv(clientfd, buffer, sizeof(buffer), 0);
        if (bytesRead < 0) {
            perror("recv");
            close(clientfd);
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        if (strncmp(buffer, CODE_WORD, sizeof(CODE_WORD)) == 0) {
            handleCodeWordReceived();
        }

        close(clientfd);
    }

    // Close the socket and remove the socket file
    close(sockfd);
    unlink(SOCKET_PATH);

    return 0;
}
