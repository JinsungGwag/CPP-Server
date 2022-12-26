#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
using namespace std;

#define IP_MAX_LENGTH 15
#define MSG_MAX_LENGTH 30

void ErrorHandling(const char* message);

int main()
{
	WSADATA wsaData;
	SOCKET clntSocket;
	SOCKADDR_IN servAddr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	clntSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (clntSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;

	while (1)
	{
		char ip[IP_MAX_LENGTH];
		printf("Enter server IP: ");
		scanf("%s", ip);

		u_short port;
		printf("Enter server port : ");
		scanf("%ud", &port);

		servAddr.sin_addr.s_addr = inet_addr(ip);
		servAddr.sin_port = htons(port);

		if (connect(clntSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
			ErrorHandling("connect() error");
		
		break;
	}

	while (1)
	{
		char message[MSG_MAX_LENGTH];
		printf("Enter message: ");
		scanf("%s", &message);

		if (message == "quit")
			break;

		send(clntSocket, message, sizeof(message), 0);

		int strLen = recv(clntSocket, message, sizeof(message), 0);
		if (strLen == -1)
		{
			ErrorHandling("read() error");
			continue;
		}
		else
			printf("Message from server: %s\n", message);
	}

	closesocket(clntSocket);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}