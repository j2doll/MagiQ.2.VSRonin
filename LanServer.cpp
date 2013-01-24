#include "LanServer.h"
#include <QTime>
LanServer::LanServer(QObject* parent)
	:QTcpServer(parent)
	,PortToListen(Comunications::DefaultTCPport)
	,GameMode(Constants::GameMode::FreeForAll)
	,DecksFormat(Constants::Legality::Any)
	,MinPlayer(2)
	,MaxPlayer(4)
	,ServerName("MagiQ Server")
	,GameStarted(false)
	,TurnNumber(0)
	,TurnOfPlayer(-1)
	,CardIDCounter(0)
{
	qsrand(QTime::currentTime().msec());
}
void LanServer::incomingConnection(int socketDescriptor){
	if (GameStarted) return;
	if (clients.value(socketDescriptor,NULL))
		clients[socketDescriptor]->deleteLater();
	clients[socketDescriptor]=new LanServerThread(socketDescriptor,this);
	LanServerThread* TempPoint=clients[socketDescriptor];
	connect(this,SIGNAL(ServerInfos(QString,int,int,int,int,int)),TempPoint,SIGNAL(ServerInfos(QString,int,int,int,int,int)));
	connect(TempPoint,SIGNAL(RequestJoin(int,QString,QPixmap)),this,SLOT(IncomingJoinRequest(int,QString,QPixmap)));
	connect(this,SIGNAL(ServerIsFull(int)),TempPoint,SIGNAL(ServerIsFull(int)));
	connect(TempPoint,SIGNAL(ReachedEnd(int)),this,SLOT(EraseThread(int)));
	connect(this,SIGNAL(GameHasStarted()),TempPoint,SIGNAL(GameHasStarted()));
	TempPoint->start();
	SendServerInfos();
}
void LanServer::EraseThread(int a){
	QMap<int,MagiQPlayer*>::iterator playpoint=PlayersList.find(a);
	if (playpoint!=PlayersList.end()){
		QString Temp=playpoint.value()->GetPlayerName();
		PlayersList.erase(playpoint);
		SendServerInfos();
		SendLeftTheGame(Temp);
	}
	QMap<int,LanServerThread*>::iterator index=clients.find(a);
	if (index!=clients.end()){
		index.value()->deleteLater();
		clients.erase(index);
	}
	if (EverybodyReady() && !GameStarted) StartMatch();
}
void LanServer::IsReadyToPlay(int a,bool ready){
	if (PlayersList.find(a)==PlayersList.end()) return;
	PlayersList[a]->SetReadyToStartMatch(ready);
	if (EverybodyReady() && !GameStarted) StartMatch();
}
bool LanServer::EverybodyReady() const{
	if (PlayersList.size()<MinPlayer) return false;
	foreach(MagiQPlayer* PlayPoint,PlayersList){
		if (!PlayPoint->GetReadyToStartMatch()) return false;
	}
	return true;
}
bool LanServer::EverybodyAcceptedHand() const{
	foreach(MagiQPlayer* PlayPoint,PlayersList){
		if (!PlayPoint->GetHasAcceptedHand()) return false;
	}
	return true;
}
void LanServer::DeckSetUp(int socID,CardDeck deck){
	if (PlayersList.find(socID)==PlayersList.end()) return;
	if (deck.Legality().contains(DecksFormat)){
		PlayersList[socID]->SetLibrary(deck);
		for (QList<CardData>::const_iterator i=PlayersList[socID]->GetLibrary().constBegin();i!=PlayersList[socID]->GetLibrary().constEnd();i++)
			i->SetCardID(++CardIDCounter);
	}
	else{
		emit InvalidDeck(socID);
		PlayersList[socID]->SetReadyToStartMatch(false);
	}
}
void LanServer::StartMatch(){
	GameStarted=true;
	emit GameHasStarted();
	//Decide who plays first
	QMap<int,int> Randomizer;
	foreach(int socID,PlayersList.keys())
		Randomizer.insert(qrand()%100,socID);
	PlayersOrder=Randomizer.values();
	emit PlayOrder(PlayersOrder);
	QMap<int,QString> PlayerNames;
	QMap<int,QPixmap> PlayerAvatars;
	for (QMap<int,MagiQPlayer*>::const_iterator ite=PlayersList.constBegin();ite!=PlayersList.constEnd();ite++){
		PlayerNames.insert(ite.key(),ite.value()->GetPlayerName());
		PlayerAvatars.insert(ite.key(),ite.value()->GetAvatar());
	}
	emit PlayersNameAvatar(PlayerNames,PlayerAvatars);
	//Shuffle the libraries, draw 7 card for each player and send them the result 
	for(QMap<int,MagiQPlayer*>::iterator index=PlayersList.begin();index!=PlayersList.end();index++){
		index.value()->ShuffleLibrary();
		for(int i=0;i<7;i++) index.value()->DrawCard();
		emit PlayerLibrary(index.key(),index.value()->GetLibrary());
		emit PlayerHand(index.key(),index.value()->GetHand());
	}
}
void LanServer::DoMulligan(int socID){
	MagiQPlayer* TempPoint=PlayersList.value(socID,NULL);
	if (!TempPoint) return;
	int HandSize=TempPoint->GetHand().size();
	--HandSize;
	TempPoint->HandToBottomLibrary();
	TempPoint->ShuffleLibrary();
	for(int i=0;i<HandSize;i++) TempPoint->DrawCard();
	emit PlayerLibrary(socID,TempPoint->GetLibrary());
	emit PlayerHand(socID,TempPoint->GetHand());
}
void LanServer::AcceptedHand(int socID){
	MagiQPlayer* TempPoint=PlayersList.value(socID,NULL);
	if (!TempPoint) return;
	TempPoint->SetHasAcceptedHand(true);
	if (EverybodyAcceptedHand()){
		TurnOfPlayer=PlayersOrder.at(0);
		TurnOfGame();
	}
}
void LanServer::TurnOfGame(){
	++TurnNumber;
}
void LanServer::IncomingJoinRequest(int a, QString nam, QPixmap avat){
	if (GameStarted) return;
	if (!clients.value(a,NULL)) return;
	if (PlayersList.size()>=MaxPlayer){
		emit ServerIsFull(a);
		return;
	}
	PlayersList[a]=new MagiQPlayer(this);
	MagiQPlayer* PlayPoint=PlayersList[a];
	PlayPoint->SetPlayerID(a);
	bool Duplicate;
	int Counter=1;
	QString adjName(nam);
	do{
		Duplicate=false;
		for(QMap<int,MagiQPlayer*>::const_iterator i=PlayersList.constBegin();i!=PlayersList.constEnd() && !Duplicate;++i){
			if (i.value()->GetPlayerName()==adjName){
				Duplicate=true;
				adjName=nam+QString(" (%1)").arg(Counter++);
			}
		}
	}while(Duplicate);
	PlayPoint->SetPlayerName(adjName);
	PlayPoint->SetAvatar(avat);
	int TempNum=PlayersList.size();
	while (TempNum>8) TempNum-=8;
	PlayPoint->SetPlayerColor(Constants::PlayerColors[--TempNum]);
	emit Joined(adjName);
	LanServerThread* TempPoint=clients[a];
	connect(this,SIGNAL(ChatMessage(QString)),TempPoint,SIGNAL(SendMessage(QString)));
	connect(TempPoint,SIGNAL(ChatMessageRecieved(QString)),this,SIGNAL(ChatMessage(QString)));
	connect(TempPoint,SIGNAL(ReadyToPlay(int,bool)),this,SLOT(IsReadyToPlay(int,bool)));
	connect(TempPoint,SIGNAL(DeckSetUp(int,CardDeck)),this,SLOT(DeckSetUp(int,CardDeck)));
	connect(this,SIGNAL(Joined(QString)),TempPoint,SIGNAL(Joined(QString)));
	connect(this,SIGNAL(LeftTheGame(QString)),TempPoint,SIGNAL(LeftTheGame(QString)));
	connect(this,SIGNAL(YourNameColor(int,QString,QColor)),TempPoint,SIGNAL(YourNameColor(int,QString,QColor)));
	connect(this,SIGNAL(InvalidDeck(int)),TempPoint,SIGNAL(InvalidDeck(int)));
	connect(this,SIGNAL(PlayOrder(QList<int>)),TempPoint,SIGNAL(PlayOrder(QList<int>)));
	connect(this,SIGNAL(PlayersNameAvatar(QMap<int,QString>,QMap<int,QPixmap>)),TempPoint,SIGNAL(PlayersNameAvatar(QMap<int,QString>,QMap<int,QPixmap>)));
	connect(this,SIGNAL(PlayerHand(int,QList<CardData>)),TempPoint,SIGNAL(PlayerHand(int,QList<CardData>)));
	connect(this,SIGNAL(PlayerLibrary(int,QList<CardData>)),TempPoint,SIGNAL(PlayerLibrary(int,QList<CardData>)));
	connect(TempPoint,SIGNAL(Mulligan(int)),this,SLOT(DoMulligan(int)));
	connect(TempPoint,SIGNAL(HandAccepted(int)),this,SLOT(AcceptedHand(int)));
	emit YourNameColor(a,adjName,PlayPoint->GetPlayerColor());
	SendServerInfos();
}