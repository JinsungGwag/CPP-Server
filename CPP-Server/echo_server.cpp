#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <winsock2.h>
using namespace std;

#define IP_MAX_LENGTH 15
#define MSG_MAX_LENGTH 30

void ErrorHandling(const char* message);

int main()
{
	WSADATA wsaData;
	SOCKET servSock, clntSock;
	SOCKADDR_IN servAddr, clntAddr;
	int szClntAddr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	servSock = socket(PF_INET, SOCK_STREAM, 0);
	if (servSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	
	while (1)
	{
		char ip[IP_MAX_LENGTH];
		printf("Enter IP: ");
		scanf("%s", ip);

		u_short port;
		printf("Enter port: ");
		scanf("%ud", &port);

		servAddr.sin_addr.s_addr = inet_addr(ip);
		servAddr.sin_port = htons(port);

		if (bind(servSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
			ErrorHandling("bind() error");


		if (listen(servSock, 5) == SOCKET_ERROR)
			ErrorHandling("listen() error");
		
		break;
	}

	szClntAddr = sizeof(clntAddr);
	clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (clntSock == INVALID_SOCKET)
		ErrorHandling("accept() error");

	while (1)
	{
		char message[MSG_MAX_LENGTH];
		
		int strLen = recv(clntSock, message, sizeof(message), 0);
		if (strLen == -1)
		{
			ErrorHandling("read() error");
			continue;
		}

		printf("Message from client: %s\n", message);
		send(clntSock, message, sizeof(message), 0);
	}

	closesocket(clntSock);
	closesocket(servSock);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}