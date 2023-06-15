#include "daemon.h"
#include "daemon_utils.h"
#include "motor_invoker.h"

#define SOCKET_PATH "/tmp/motorsd_socket"

void signal_handler(int signum) {
    if (signum == SIGTERM) {
        printf("Received SIGTERM, shutting down.\n");
        exit(EXIT_SUCCESS);
    }
}

int main() {
    // Register signal handler for graceful shutdown
    signal(SIGTERM, signal_handler);

    // Create a Unix socket
    int soc_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (soc_fd < 0) {
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
    if (bind(soc_fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) < 0) {
        perror("Error binding socket");
        close(soc_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(soc_fd, 1) < 0) {
        perror("Error trying to listen to socket");
        close(soc_fd);
        exit(EXIT_FAILURE);
    }

    printf("Listening for connections on %s\n", SOCKET_PATH);

    // Accept incoming connections and process data
    while (1) {
        int client_fd = accept(soc_fd, NULL, NULL);
        if (client_fd < 0) {
            perror("Error accepting connection");
            close(soc_fd);
            exit(EXIT_FAILURE);
        }

        char buffer[sizeof(MotorCommand)];
        size_t bytesRead = recv(client_fd, buffer, sizeof(buffer), 0);
        if (bytesRead < 0) {
            perror("Error receiving message from socket");
            close(client_fd);
            close(soc_fd);
            exit(EXIT_FAILURE);
        }

        MotorCommand command = parse_motor_command((const uint8_t *) &buffer, sizeof(buffer));

        process_motor_command(command);

        close(client_fd);
    }

    // Close the socket and remove the socket file
    close(soc_fd);
    unlink(SOCKET_PATH);

    return 0;
}
