#include "daemon.h"
#include "daemon_utils.h"
#include "motor_invoker.h"

#include "config.h"
#include "config_parser.h"

#define SOCKET_PATH "/tmp/motorsd_socket"

int soc_fd;

void signal_handler(int signum)
{
	if (signum == SIGTERM) {
		close(soc_fd);
		unlink(SOCKET_PATH);
		printf("Received SIGTERM, shutting down.\n");
		exit(EXIT_SUCCESS);
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Error: Motor config file path argument is required.\n");
		return 1;
	}

	char *filepath = argv[1];
	ConfigWrapper *wrapper;
	int err = config_parser_parse_config(filepath, &wrapper);

	config_parser_print_config(err, wrapper);

	// Register signal handler for graceful shutdown
	signal(SIGTERM, signal_handler);

	//Read configuration from file

	// Create a Unix socket
	soc_fd = socket(AF_UNIX, SOCK_STREAM, 0);
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
	if (bind(soc_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) <
	    0) {
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

		printf("Accepted request from socket");

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

		MotorCommand command = parse_motor_command(
			(const uint8_t *)&bytesRead, sizeof(buffer));

		process_motor_command(command);

		close(client_fd);
	}

	return 0;
}
