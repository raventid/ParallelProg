// finalcpp2.cpp : Defines the entry point for the console application.
//

#ifdef __linux
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "server.h"

int main(int argc, char* argv[])
{
#ifdef WIN32
	WSADATA wsaData;
	DWORD res = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (res != 0) {
		return EXIT_FAILURE;
	}
#endif

	Settings s;

	s.path = "c:\\";
	s.port = 8080;
	s.address = "0.0.0.0";

#ifdef __linux
	static const char *optString = "h:p:d:";

	int opt;
	// -h <ip> -p <port> -d <directory>
	while ((opt = getopt(argc, argv, optString)) != -1)
	{
		switch( opt )
		{
			case 'h':
			s.address = optarg;
			break;
			case 'p':
			s.port = atoi(optarg);
			break;
			case 'd':
			s.path = optarg;
			break;
		}
	}

	pid_t pid, sid;

	pid = fork();
	if (pid < 0)
	{
		exit(EXIT_FAILURE);
	}
	if (pid > 0)
	{
		exit(EXIT_SUCCESS);
	}

	umask(0);

	sid = setsid();
	if (sid < 0)
	{
		exit(EXIT_FAILURE);
	}

	if ((chdir("/")) < 0)
	{
		exit(EXIT_FAILURE);
	}


	//close(STDIN_FILENO);
	//close(STDOUT_FILENO);
	//close(STDERR_FILENO);

#endif
	FinalServer server(s);
	server.run();

#ifdef WIN32
	WSACleanup();
#endif

	return 0;

}

