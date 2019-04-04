#include <iostream>
#include <cstring>
#include "include/sockLib.h"
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

int main(int argc, char const *argv[])
{
	if ( argc < 3 ) {
		cout << "<host> <port>" << endl;
		return 0;
	}

	int s;
	struct sockaddr_in server;

	s = Socket();
	initSockaddr( &server, argv[1], atoi( argv[2] ) );
	Connect( s, &server );

	char msg[128];
	int n;

	for (;;){
		cin.getline( msg, 128, '\n' );

		if ( strlen(msg) == 0 ){
			cout << "[-] disconnecting ..." << endl;
			break;
		}

		n = send( s, msg, strlen(msg), 0 );
		if ( n < 0 ) {
			perror( "[-] send call failed" );
			close( s );
			exit( 1 );
		}

		n = recv( s, msg, sizeof(msg), 0 );
		if ( n < 0 ) {
			perror( "[-] recv call failed" );
			close( s );
			exit( 1 );
		}
		cout << "Server: " << msg << endl;

		if ( strcmp( msg, "exit()" ) == 0 ) {
			cout << "[-] disconnecting ..." << endl;
			break;
		}
	}

	close( s );
	cout << "[-] disconnected." << endl;

	return 0;
}