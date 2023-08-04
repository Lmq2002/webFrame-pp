#include "HttpSrv.h"
#include<string>
using namespace std;




int main() {
	string request = "Get /url 1.0.0\r\nContent-Length: 100\r\n";
	
	/*HttpSrv httpServer;
	HttpPropotol result;
	httpServer.ParseHttp(request, result);*/
	HttpSrv httpServer;
	httpServer.Serve();
}