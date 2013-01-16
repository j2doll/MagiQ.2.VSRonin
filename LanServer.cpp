#include "LanServer.h"
LanServer::LanServer(QObject* parent)
	:QTcpServer(parent)
	,PortToListen(Comunications::DefaultTCPport)
{}
void LanServer::incomingConnection(int socketDescriptor){
	if (clients.value(socketDescriptor,NULL))
		clients[socketDescriptor]->deleteLater();
	clients[socketDescriptor]=new LanServerThread(socketDescriptor,this);
	LanServerThread* TempPoint=clients[socketDescriptor];
	connect(TempPoint,SIGNAL(ChatMessageRecieved(QString)),this,SIGNAL(ChatMessage(QString)));
	connect(this,SIGNAL(ChatMessage(QString)),TempPoint,SLOT(SendChatMessage(QString)));
	TempPoint->start();
}