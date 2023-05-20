#include "daemon.h"
#include "daemon_utils.h"

#define SOCKET_PATH "/tmp/motorsd_socket"

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
    int socFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socFd < 0) {
        perror("Error creating socket");
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
    if (bind(socFd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) < 0) {
        perror("Error binding socket");
        close(socFd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(socFd, 1) < 0) {
        perror("Error trying to listen to socket");
        close(socFd);
        exit(EXIT_FAILURE);
    }

    printf("Listening for connections on %s\n", SOCKET_PATH);

    // Accept incoming connections and process data
    while (1) {
        int clientFd = accept(socFd, NULL, NULL);
        if (clientFd < 0) {
            perror("Error accepting connection");
            close(socFd);
            exit(EXIT_FAILURE);
        }

        char buffer[sizeof(MotorCommand)];
        size_t bytesRead = recv(clientFd, buffer, sizeof(buffer), 0);
        if (bytesRead < 0) {
            perror("Error receiving message from socket");
            close(clientFd);
            close(socFd);
            exit(EXIT_FAILURE);
        }

        MotorCommand command = parseMotorCommand((const uint8_t *) &bytesRead, sizeof(buffer));


        close(clientFd);
    }

    // Close the socket and remove the socket file
    close(socFd);
    unlink(SOCKET_PATH);

    return 0;
}
