#include <arpa/inet.h>
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
    exit(1);
}

int open_socket() {
    int socket_fd = -1;
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        error("Error opening socket");
    }
    return socket_fd;
}

void bind_socket(int socket_fd, int port_number) {
    // Initialize the data structure
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    // Set the required fields
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons((uint16_t)port_number);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Try to bind socket
    if (bind(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("Error binding socket to this server");
    }
}

int main(int argc, char *argv[]) {
    // Check input args
    if (argc < 5) {
        printf("usage %s port ssh_executable ssh_destination client_executable\n", argv[0]);
        exit(0);
    }
    int port_number = atoi(argv[1]);    // ex. 5544
    char *ssh_executable = argv[2];     // ex. /usr/bin/ssh
    char *ssh_destination = argv[3];    // ex. ug04.cs.ualberta.ca
    char *client_executable = argv[4];  // ex. ~/379/labs/lab9/client

    // Get the IP address of the host machine that the server is running on
    char hostname[128];
    gethostname(hostname, sizeof(hostname));
    struct hostent *server;
    if ((server = gethostbyname(hostname)) == NULL) {
        error("Error: Unable to get host details");
    }
    // Convert from network byte order to a string IPv4 address
    char *host_ip = inet_ntoa(*(struct in_addr *)server->h_addr);

    // ***Before we run the client, we need to first ensure our server socket is listening for connections
    // Open a socket
    int socket_fd = open_socket();
    // Bind socket to all local interfaces
    bind_socket(socket_fd, port_number);
    // Allow process to listen on socket for connections
    if (listen(socket_fd, 5) == -1) {
        error("Error listening to socket");
    }
    printf("Waiting for connections.\n");

    // Now we can start the client program on a remote machine
    // Note: The client executable must already be present on the remote machine
    // (we get this for free if we're using the CS lab machines, since our storage is networked)
    char command[256];
    // Paul:  NOTE:  Assumes your default shell is bash
    // Paul:  See assignment description, Other comments and hints #6
    snprintf(command, sizeof(command), "%s %s '%s %s %d </dev/null >/dev/null 2>&1  &'", ssh_executable, ssh_destination, client_executable, host_ip, port_number);
    printf("Running command: %s\n", command);
    // Executes a shell command (blocks until command is completed, avoid by sending it to background via &)
    // Paul:  portnumber == 8000 is special case, to start client manually
    if ( port_number != 8000 && system(command) == -1) {	// Paul
        error("Error: Failed to run command");
    }

    // Block process until a client connects to this server
    int connected_socket_fd = 0;
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    if ((connected_socket_fd = accept(socket_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_len)) < 0) {
        error("Error accepting connection");
    }
    char *ip = inet_ntoa(client_addr.sin_addr);
    printf("Client %s connected!\n", ip);

    // Read message from client
    printf("Message from client:\n");
    int bytes_read = 0;
    // char buffer[512];			// Paul
    char buffer[1024];				// Paul
    int n = 0;					// Paul
    char reply[] = "OK\n\0";			// Paul
    memset(buffer,0,sizeof(buffer));        	// Paul
    while ((bytes_read = (int)read(connected_socket_fd, buffer, sizeof(buffer))) > 0) {
        // buffer[bytes_read] = '\0';
        printf("%s", buffer);
        memset(buffer,0,sizeof(buffer));        // Paul

	memcpy(buffer,reply,sizeof(reply));	// Paul
    	n = write(connected_socket_fd, buffer, sizeof(buffer));
	printf("Sent: %s", buffer);
	assert(n == sizeof(buffer));
        memset(buffer,0,sizeof(buffer));        // Paul
    }

    close(connected_socket_fd);
    close(socket_fd);
    return 0;
}
