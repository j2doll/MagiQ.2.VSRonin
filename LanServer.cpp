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
	connect(this,SIGNAL(ChatMessage(QString)),TempPoint,SIGNAL(SendMessage(QString)));
	connect(TempPoint,SIGNAL(ChatMessageRecieved(QString)),this,SIGNAL(ChatMessage(QString)));
	connect(TempPoint,SIGNAL(ReachedEnd(int)),this,SLOT(EraseThread(int)));
	TempPoint->start();
}
void LanServer::EraseThread(int a){
	QMap<int,LanServerThread*>::iterator index=clients.find(a);
	if (index!=clients.end()){
		index.value()->deleteLater();
		clients.erase(index);
	}
}