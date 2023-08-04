#pragma once
#include <String>
#include <map>
#include <vector>
#include<WinSock2.h>
#include<WS2tcpip.h>

struct HttpPropotol {
	std::string type;
	std::string url;
	std::string version;
	std::map<std::string, std::string> headerParameter;
	std::string body;
};


class HttpSrv
{
private:	
	SOCKET m_listenSocket;
	std::vector<SOCKET> m_subSockets;
	SOCKADDR_IN m_addr;
public:
	HttpSrv();
	~HttpSrv();
	bool ParseHttp(std::string strInput, HttpPropotol& result);
	bool Serve();
};

