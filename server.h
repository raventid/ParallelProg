#pragma once

#include <list>

#include <iostream>
#include <fstream>

#include <memory>

#include <thread>
#include <mutex>
#include <atomic>

#include "settings.h"

#ifdef WIN32
#include <winsock.h>
#else
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

typedef int SOCKET;
#endif

class FinalServer
{
	class ClientConnection
	{
	public:
		ClientConnection(FinalServer& server, SOCKET s)
			: _server(server), _root(server.getRootPath()), _socket(s), _isWorking(true)
		{
			if (_root.size() > 0 && _root[_root.size() - 1] != '/')
				_root += "/";

			std::thread t(&ClientConnection::doRead, this);
			_t.swap(t);
			_t.detach();
		}

		~ClientConnection()
		{
			close();
		}

		void stop()
		{
			close();
			if (_t.joinable()){
				_t.join();
			}
		}

		void close()
		{
			_isWorking = false;
			::shutdown(_socket, 2);
#ifdef WIN32
			::closesocket(_socket);
#else
			::close(_socket);
#endif
		}

		void doRead()
		{
			std::cerr << "Wait for query" << std::endl;
			std::string query;
			const size_t BUFFER_SIZE = 1024;
			char buff[BUFFER_SIZE+1] = {0};
			while (_isWorking)
			{
				int size = ::recv(_socket, buff, BUFFER_SIZE, 0);
				if (size < 0)
					break;

				std::cerr << "Got " << size << " bytes "<< std::endl;
				buff[size] = '\0';
				query += buff;
				if (query.find('\n') != std::string::npos)
				{
					std::string method = query.substr(0, 3);

					if (method != "GET")
						break;

					std::string path = query.substr(5, std::string::npos);
					size_t pos = path.find_first_of(" ?");
					if (pos != std::string::npos)
						path = path.substr(0, pos);
					path = _root + path;

					std::cerr << "Searching file " << path << std::endl;

					std::string reply;
					std::ifstream f(path);
					if (f.is_open())
					{
						reply = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n";

						const size_t BUFFER_SIZE = 1024;
						char buff[BUFFER_SIZE+1] = {0};
						while (f.read(buff, BUFFER_SIZE))
						{
							reply += buff;
						}
						buff[f.gcount()] = 0;
						reply += buff;
						std::cerr << "Replied 200, " << reply.size() << " bytes" << std::endl;
					}
					else
					{
						std::cerr << "errno: " << errno << std::endl;
						reply = "HTTP/1.0 404 Not found\r\n\r\n";
						std::cerr << "Replied 404" << std::endl;
					}
					::send(_socket, reply.c_str(), reply.size(), 0);
					break;
				}
			}

			_server.onClose(this);
		}

	private:
		FinalServer& _server;
		std::string _root;
		SOCKET _socket;
		std::atomic<bool> _isWorking;
		std::thread _t;
	};

	typedef std::shared_ptr<ClientConnection> ClientConnectionPtr;
	typedef std::list<ClientConnectionPtr> ClientConnectionList;

public:
	FinalServer(Settings& settings) : _settings(settings), _isWorking(true){}

	void stop()
	{
		_isWorking = false;
		::shutdown(_socket, 2);
#ifdef WIN32
		::closesocket(_socket);
#else
		::close(_socket);
#endif
		std::unique_lock<std::mutex> lock(_clientsMutex);
		for (auto& c: _clients)
			c->close();
	}

	void run()
	{
		_socket =::socket(AF_INET, SOCK_STREAM, 0);

		if (_socket == -1)
		{
			std::cerr << "Cannot create socket: "  << errno << std::endl;
			exit(EXIT_FAILURE);
		}

		sockaddr_in addr;
		::memset(&addr, 0, sizeof(addr));

		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(_settings.port);

		if (::bind(_socket, (sockaddr*)&addr, sizeof(addr)) < 0)
		{
			std::cout << "Cannot bind socket: " << errno << std::endl;
			exit(EXIT_FAILURE);
		};

		listen(_socket, 10);

		while(_isWorking)
		{
			sockaddr_in cliAddr;
#ifdef WIN32
			int cliLen = sizeof(cliAddr);
#else
			unsigned int cliLen = sizeof(cliAddr);
#endif
			SOCKET cliSock = ::accept(_socket, (sockaddr *)&cliAddr, &cliLen);
			if (cliSock != -1)
			{
				std::cerr << "Accepted connection" << std::endl;
				ClientConnectionPtr client(new ClientConnection(*this, cliSock));
				std::unique_lock<std::mutex> lock(_clientsMutex);
				_clients.push_back(client);
			}
		}
	}

	const std::string& getRootPath()
	{
		return _settings.path;
	}

	void onClose(ClientConnection* pClient)
	{
		std::unique_lock<std::mutex> lock(_clientsMutex);
		for (ClientConnectionList::iterator it = _clients.begin(); it != _clients.end(); ++it)
		{
			if (it->get() == pClient)
			{
				_clients.erase(it);
				break;
			}
		}
	}

private:
	std::mutex _clientsMutex;
	ClientConnectionList _clients;

	SOCKET _socket;
	std::atomic<bool> _isWorking;

	Settings& _settings;
};
