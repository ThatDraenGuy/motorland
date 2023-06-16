#include "daemon_utils.h"
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/motorsd_socket"

int soc_fd;

void signal_handler(int signum) {
  if (signum == SIGTERM) {
    printf("Received SIGTERM, shutting down.\n");
    close(soc_fd);
    exit(EXIT_SUCCESS);
  }
}

void connect_to_server() {
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

  // Connect to the server
  if (connect(soc_fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) < 0) {
    perror("Error connecting to server");
    close(soc_fd);
    exit(EXIT_FAILURE);
  }
}

void close_connection() {
  close(soc_fd);
}

void send_motor_command(MotorCommand command) {
  connect_to_server();
  // Send the motor command
  ssize_t bytesSent = send(soc_fd, &command, sizeof(MotorCommand), 0);
  close_connection();
  if (bytesSent < 0) {
    perror("Error sending motor command");
    exit(EXIT_FAILURE);
  } else if (bytesSent != sizeof(MotorCommand)) {
    fprintf(stderr, "Incomplete transmission of motor command\n");
    exit(EXIT_FAILURE);
  }
}

void move_camera(uint8_t motor_index, int16_t steps, uint8_t speed) {
  MotorCommand commandX;
  commandX.motorIndex = motor_index;
  commandX.numOfSteps = steps;
  commandX.speed = speed;
  commandX.checksum = 0xFF;

  send_motor_command(commandX);
}

int main() {
  // Register signal handler for graceful shutdown
  signal(SIGTERM, signal_handler);

  // Move the camera in a loop every 5 seconds
  while (1) {
    // Move camera left
    move_camera(1, 500, 80);
    sleep(1);

    // Move camera up
    move_camera(0, 500, 80);
    sleep(1);

    // Move camera right
    move_camera(1, -500, 80);
    sleep(1);

    // Move camera down
    move_camera(0, -500, 80);
    sleep(1);
  }
}
