

#include "Server.h"
#ifdef _WIN_32
    #pragma comment(lib, "ws2_32.lib")
    #include <winsock2.h>
    #include <windows.h>
#else
    #include <signal.h>
    #include <sys/socket.h>
#endif

#include <iostream>
#include <string>
#include <QThread>
#include <QObject>
#include <QDebug>
#include <QSharedPointer>
#include <list>
#include "../Socket/TcpSocket.h"
#include "Database/DataAccessObject.h"
#include "ClientThread.h"

#ifdef __unix__

void my_handler(int s){
    std::cout<<"Przechwycono sygnal"<<s<<std::endl;

    exit(1);

}
#endif

Server::Server(int port) {

this->port = port;
	this->maxClients = 1024;
    this->dao = new DataAccessObject();
}


Server::~Server(void)
{
    delete dao;
}



int Server::start() {
    #ifdef _WIN_32
        WSADATA wsaData;
        if( WSAStartup( MAKEWORD(2,2), &wsaData ) != 0 ) {

		std::cout<<"Wystapil blad z zainicjowaniem biblioteki!\n";
		return -1;
	}
    #endif

    #ifdef __unix__
        struct sigaction sigIntHandler;

        sigIntHandler.sa_handler = my_handler;
        sigemptyset(&sigIntHandler.sa_mask);
        sigIntHandler.sa_flags = 0;
        sigaction(SIGINT, &sigIntHandler, NULL);
    #endif

    mainSocket = new TcpSocket(this->port,20,120);
    if (mainSocket->setSockOpt() == 1) return 1;
	if (mainSocket->bindSocket() == 1) return 1;
	if (mainSocket->listenSocket() == 1) return 1;
	TcpSocket* clientSocket;
    ClientThread* client;
    int ret = 0;

    while (true) {
        if ((ret = mainSocket->selectSocket() ) > 0) {
            //PRZYJALEM STRZALEK
        //if( !clientSocket->checkIfInvalid() ) {
            int sock = mainSocket->acceptSocket();
            if (sock > 0 && !clientSocket->checkIfInvalid()){
                    clientSocket = new TcpSocket(sock);
                    clientSocket->set(120);
                    qDebug()<<"Polaczenie";


                    client = new ClientThread(clientSocket, dao);


                    QThreadPool::globalInstance()->start(client);
                }
            }
    }


	//reszta, obaczymy co tutaj bedzie
    #ifdef _WIN_32
        WSACleanup();
        system("pause");
    #endif
	return 0;
}
