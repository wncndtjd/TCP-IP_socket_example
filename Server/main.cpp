#include <iostream>
#include <string>
#include <winSock2.h>

using namespace std;

void ShowErrorMessage(string msg) {

	cout << "Error occurence : " << msg << endl;
	system("pause");
	exit(1);
}

int main() {

	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)  // initialize WINSOCKET;
		ShowErrorMessage("WSAStartUp()");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0);	// create a TCP socket.
	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);	// 4 byte integer type to network byte type
	serverAddress.sin_port = htons(serverPort);	// 2 byte integer type to network byte type 

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");

	cout << "[Current State] : bind()" << endl;

	if (listen(serverSocket, 5) == SOCKET_ERROR)
		ShowErrorMessage("listen()");

	cout << "[Current State] : listen()" << endl;

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "[Current State] : accept()" << endl;

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");

	while (1) { // accept a client msg and send that back.

		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;

		cout << "Client Message : " << received << endl;
		cout << "Transfer Message : " << received << endl;

		if (strcmp(received, "[exit]") == 0) {

			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "Server End." << endl;
			break;
		}

		send(clientSocket, received, sizeof(received) - 1, 0);
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();

	system("pause");
	return 0;
}