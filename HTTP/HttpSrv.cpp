#include "HttpSrv.h"
#include <iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
/**
*字符串分割函数
* 输入分割目标和依据的字符串
* 输出分割的字符串vector结果
*/
std::vector<std::string> split(std::string strInput, std::string strSpliter) {
    std::vector<std::string> vecResult;
    std::string subStr;/* = strInput;*/
    int nStartPos=0, nEndPos;
    while (1) {
        nEndPos = (int)strInput.find(strSpliter);
        if (nEndPos == -1) {
            subStr = strInput;
            if(subStr.length())
                vecResult.push_back(subStr);
            return vecResult;
        }
        subStr = strInput.substr(nStartPos, nEndPos - nStartPos);
        strInput = strInput.substr(nEndPos+strSpliter.length());
        vecResult.push_back(subStr);
    }

}

HttpSrv::HttpSrv()
{
}

HttpSrv::~HttpSrv()
{
}

bool HttpSrv::ParseHttp(std::string strInput, HttpPropotol& result)
{
    //包头包体
    std::vector<std::string> vecHeader_Body =  split(strInput, "\r\n\r\n");
    std::vector<std::string> vecHeader = split(vecHeader_Body[0], "\r\n");

    vector<string> vecSegment = split(vecHeader[0], " ");
    if (vecSegment.size() != 3)return false;
    result.type = vecSegment[0];
    result.url = vecSegment[1];
    result.version = vecSegment[2];
    
    vector<string>::iterator it = vecHeader.begin();
    it++;
    for (; it != vecHeader.end(); it++) {
        vector<string> vecParam = split(*it,":");
        if (vecParam.size() != 2)return false;
        result.headerParameter[vecParam[0]]=vecParam[1];
    }
    result.body = vecHeader_Body[1];
    return true;
}

bool HttpSrv::Serve()
{
    cout << "start to launch http server" << endl;
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(1, 1);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        return -1;
    }

    if(LOBYTE(wsaData.wVersion)!=1||
       HIBYTE(wsaData.wVersion != 1)) {
        WSACleanup();
        return -1;
    }

    m_listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    m_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(5000);

    bind(m_listenSocket, (SOCKADDR*)&m_addr, sizeof(SOCKADDR));
    listen(m_listenSocket, 5);
    SOCKADDR_IN addrClient;
    int len = sizeof(SOCKADDR);
    while (1) {
        SOCKET sockConn = accept(m_listenSocket, (SOCKADDR*)&addrClient, &len);
        char sendBuf[100];
        char str[INET_ADDRSTRLEN];
        char recvBuf[100];
        recv(sockConn, recvBuf, 100, 0);

        HttpPropotol result;
        this->ParseHttp(recvBuf, result);
        closesocket(sockConn);
    }
}
