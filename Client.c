#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFF_SIZE 1024

int main(int argc, char *argv[]){

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];
	int msg_len;

	if(argc != 3){
		printf("Please use command : \n");
		printf("./client IPAddress PortNumber\n");
		return 0;
	}

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[2]));
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	while(1){
		printf("Enter Message (Enter NULL to exit): \n");
		memset(buffer,'\0',(strlen(buffer)+1));
		fgets(buffer, BUFF_SIZE, stdin);		
		msg_len = strlen(buffer);
		if (msg_len == 1){
			printf("\nExit!\n");
			break;
		}
		send(clientSocket, buffer, strlen(buffer), 0);

		if(recv(clientSocket, buffer, BUFF_SIZE, 0) < 0){
			printf("[-]Error in receiving data.\n");
		}else{
			printf("Server: \n%s\n", buffer);
		}
	}
	close(clientSocket);
	printf("[-]Disconnected from server.\n");
	exit(1);
	return 0;
}