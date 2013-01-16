#include "LanServer.h"
void LanServer::incomingConnection(int socketDescriptor){
	if (clients.value(socketDescriptor,NULL))
		clients[socketDescriptor]->deleteLater();
	clients[socketDescriptor]=new LanServerThread(socketDescriptor,this);
	clients[socketDescriptor]->start();
}