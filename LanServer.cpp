#include "LanServer.h"
LanServer::LanServer(QObject* parent)
	:QTcpServer(parent)
	,PortToListen(Comunications::DefaultTCPport)
	,GameMode(Constants::GameMode::FreeForAll)
	,DecksFormat(Constants::Legality::Any)
	,MinPlayer(2)
	,MaxPlayer(4)
	,ServerName("MagiQ Server")
{}
void LanServer::incomingConnection(int socketDescriptor){
	if (clients.value(socketDescriptor,NULL))
		clients[socketDescriptor]->deleteLater();
	clients[socketDescriptor]=new LanServerThread(socketDescriptor,this);
	LanServerThread* TempPoint=clients[socketDescriptor];
	connect(this,SIGNAL(ServerInfos(QString,int,int,int,int,int)),TempPoint,SIGNAL(ServerInfos(QString,int,int,int,int,int)));
	connect(TempPoint,SIGNAL(RequestJoin(int)),this,SLOT(IncomingJoinRequest(int)));
	connect(this,SIGNAL(ServerIsFull(int)),TempPoint,SIGNAL(ServerIsFull(int)));
	connect(TempPoint,SIGNAL(ReachedEnd(int)),this,SLOT(EraseThread(int)));
	TempPoint->start();
	SendServerInfos();
}
void LanServer::EraseThread(int a){
	if (PlayersSockets.contains(a)){
		PlayersSockets.erase(PlayersSockets.begin()+PlayersSockets.indexOf(a));
		SendServerInfos();
		SendLeftTheGame(a);
	}
	QMap<int,bool>::iterator indexReady=ReadyPlayers.find(a);
	if (indexReady!=ReadyPlayers.end()){
		ReadyPlayers.erase(indexReady);
	}
	QMap<int,LanServerThread*>::iterator index=clients.find(a);
	if (index!=clients.end()){
		index.value()->deleteLater();
		clients.erase(index);
	}
}
void LanServer::IsReadyToPlay(int a,bool ready){
	if (ReadyPlayers.find(a)==ReadyPlayers.end()) return;
	ReadyPlayers[a]=ready;
	if (EverybodyReady()) StartMatch();
}
bool LanServer::EverybodyReady() const{
	if (ReadyPlayers.size()<MinPlayer) return false;
	foreach(bool rdy,ReadyPlayers){
		if (!rdy) return false;
	}
	return true;
}
void LanServer::IncomingJoinRequest(int a){
	if (!clients.value(a,NULL)) return;
	if (PlayersSockets.size()>=MaxPlayer){
		emit ServerIsFull(a);
		return;
	}
	PlayersSockets.append(a);
	ReadyPlayers[a]=false;
	LanServerThread* TempPoint=clients[a];
	connect(this,SIGNAL(ChatMessage(QString)),TempPoint,SIGNAL(SendMessage(QString)));
	connect(TempPoint,SIGNAL(ChatMessageRecieved(QString)),this,SIGNAL(ChatMessage(QString)));
	connect(TempPoint,SIGNAL(ReadyToPlay(int,bool)),this,SLOT(IsReadyToPlay(int,bool)));
	SendServerInfos();
}