#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>

void error(const char *msg) {
    perror(msg);
    exit(0);
}

int open_socket() {
    int socket_fd = -1;
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        error("Error opening socket");
    }
    return socket_fd;
}

void call_socket(int socket_fd, const char *hostname, int port_number) {
    // Initialize the data structure
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    // Get the host address from the hostname
    struct hostent *server;
    if ((server = gethostbyname(hostname)) == NULL) {
        close(socket_fd);
        error("Error, unknown host");
    }

    // Set the required fields
    memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, (uint8_t)server->h_length);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons((uint16_t)port_number);

    // Try to connect
    if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("Error connecting to server");
    }
}

int main(int argc, char *argv[]) {
    int n = 0;						// Paul
    // Check input args
    if (argc < 3) {
        printf("usage %s hostname port\n", argv[0]);
        exit(0);
    }
    const char *hostname = argv[1];
    int port_number = atoi(argv[2]);

    // Open a socket
    int socket_fd = open_socket();

    // Call the server
    call_socket(socket_fd, hostname, port_number);

    // Run the ls command and send the output to the server
    const char *command = "ls -l | head -20";		// Paul
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        error("popen failed");
    }
    char buffer[1024];
    memset(buffer,0,sizeof(buffer));			// Paul
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // write(socket_fd, buffer, strlen(buffer));	// Paul
        write(socket_fd, buffer, sizeof(buffer));	// Paul
        memset(buffer,0,sizeof(buffer));		// Paul

	// Wait for OK response
        n = read(socket_fd, buffer, sizeof(buffer));	// Paul
	assert(n == sizeof(buffer));			// Paul
	printf("Client Sent: %s",buffer);		// Paul
        memset(buffer,0,sizeof(buffer));		// Paul
    }

    pclose(fp);
    close(socket_fd);
    return 0;
}
