#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMessage(string msg) {

	cout << "[Error Occurrence]" << msg << endl;
	system("pause");
	exit(1);
}

int main() {

	WSADATA wsaData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9876;
	char received[256];
	string sent;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)	// initialzie winsock
		ShowErrorMessage("WSAStartup()");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0);	// create a TCP socket.
	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");	// string ip to network byte type
	serverAddress.sin_port = htons(serverPort);	// 2 type integer to network byte type

	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("connect()");

	cout << "[Current Status] : connect" << endl;

	while (1) {

		cout << "[Message transfer] : ";
		getline(cin, sent);
		if (sent == "") continue;

		send(clientSocket, sent.c_str(), sent.length(), 0);
		
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = '\0';
		if (strcmp(received, "[exit]") == 0) {

			cout << "[server end]" << endl;
			break;
		}
		cout << "[server message] : " << received << endl;
	}

	closesocket(clientSocket);
	WSACleanup();

	system("pause");
	return 0;
}