#include <iostream>
#include <cstring>
#include "include/sockLib.h"
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

int main(int argc, char const *argv[])
{
	if ( argc < 2 ) {
		cout << "<port>" << endl;
		return 0;
	}

	int s, sockfd;
	struct sockaddr_in server;

	s = Socket();
	initSockaddr( &server, atoi( argv[1] ) );
	Bind( s, &server );
	Listen( s, 2 );

	pid_t pid;
	char msg[128];
	int n;

	for (;;) {
		sockfd = Accept( s );
		
		pid = fork();
		if ( pid < 0 ) {
			perror( "[-] fork call failed" );
			close( sockfd );
			close( s );
			exit( 1 );
		}
		else if ( pid == 0 ) { // Child process
			close ( s );

			for (;;) {
				n = recv( sockfd, msg, sizeof(msg), 0 );
				if ( n < 0 ) {
					perror( "[-] recv call failed" );
					close( sockfd );
					exit( 1 );
				}
				else if ( n == 0 ){
					cout << "[-] disconnecting client " << getpid() << endl;
					break;
				}
				msg[n] = '\0';

				cout << "Client_" << getpid() << ": " << msg << endl;

				n = send( sockfd, msg, strlen(msg), 0 );
				if ( n < 0 ){
					perror( "[-] send call failed" );
					close( sockfd );
					exit( 1 );
				}

				if ( strcmp( msg, "exit()" ) == 0 ) {
					cout << "[-] disconnecting client " << getpid() << endl;
					break;
				}
			} // for(;;)

			close( sockfd );
			cout << "[-] disconnected.\n" << endl;
			exit( 0 );
		} // pid == 0

		// Parent process
		close( sockfd );
	} // for(;;)

	close( s );
	cout << "[-] server closed." << endl;

	return 0;
}