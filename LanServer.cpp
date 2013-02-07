#include "LanServer.h"
#include "ComunicationConstants.h"
#include "Judge.h"
LanServer::LanServer(QObject* parent)
	:QTcpServer(parent)
	,PortToListen(Comunications::DefaultTCPport)
{
	Ruler=new Judge(this);
	connect(Ruler,SIGNAL(ServerInfos(QString,int,int,int,int,int)),this,SIGNAL(ServerInfos(QString,int,int,int,int,int)));
	connect(Ruler,SIGNAL(LeftTheGame(QString)),this,SIGNAL(LeftTheGame(QString)));
	connect(Ruler,SIGNAL(Joined(QString)),this,SIGNAL(Joined(QString)));
	connect(Ruler,SIGNAL(YourNameColor(int,QString,QColor)),this,SIGNAL(YourNameColor(int,QString,QColor)));
	connect(Ruler,SIGNAL(InvalidDeck(int)),this,SIGNAL(InvalidDeck(int)));
	connect(Ruler,SIGNAL(PlayerHand(int,QList<CardData>)),this,SIGNAL(PlayerHand(int,QList<CardData>)));
	connect(Ruler,SIGNAL(PlayerLibrary(int,QList<CardData>)),this,SIGNAL(PlayerLibrary(int,QList<CardData>)));
	connect(Ruler,SIGNAL(PlayOrder(QList<int>)),this,SIGNAL(PlayOrder(QList<int>)));
	connect(Ruler,SIGNAL(PlayersNameAvatar(QMap<int,QString>,QMap<int,QPixmap>)),this,SIGNAL(PlayersNameAvatar(QMap<int,QString>,QMap<int,QPixmap>)));
	connect(Ruler,SIGNAL(GameHasStarted()),this,SIGNAL(GameHasStarted()));
	connect(Ruler,SIGNAL(WaitingFor(int,QString)),this,SIGNAL(WaitingFor(int,QString)));
	connect(Ruler,SIGNAL(StopWaitingFor()),this,SIGNAL(StopWaitingFor()));
	connect(Ruler,SIGNAL(CurrentPhaseChanged(int)),this,SIGNAL(CurrentPhaseChanged(int)));
	connect(Ruler,SIGNAL(CardsToUntap(QList<int>)),this,SIGNAL(CardsToUntap(QList<int>)));
	connect(Ruler,SIGNAL(CardDrawn(int,CardData)),this,SIGNAL(CardDrawn(int,CardData)));
	connect(Ruler,SIGNAL(StopTimers()),this,SIGNAL(StopTimers()));
	connect(Ruler,SIGNAL(ResumeTimers()),this,SIGNAL(ResumeTimers()));
	connect(Ruler,SIGNAL(StopTurnTimer()),this,SIGNAL(StopTurnTimer()));
	connect(Ruler,SIGNAL(ResumeTurnTimer()),this,SIGNAL(ResumeTurnTimer()));
	connect(Ruler,SIGNAL(ResumeStackTimer()),this,SIGNAL(ResumeStackTimer()));
	connect(Ruler,SIGNAL(EffectAddedToStack(quint32,const EffectData&)),this,SIGNAL(EffectAddedToStack(quint32,const EffectData&)));
	connect(Ruler,SIGNAL(EffectResolved()),this,SIGNAL(EffectResolved()));
	connect(Ruler,SIGNAL(PlayableCards(int,QList<int>)),this,SIGNAL(PlayableCards(int,QList<int>)));
	connect(Ruler,SIGNAL(PlayedCard(int Who,const CardData& crd)),this,SIGNAL(PlayedCard(int Who,const CardData& crd)));
}
void LanServer::StartListening(){
	if (isListening()) return;
	if (!listen(QHostAddress::Any, PortToListen))
		emit CantBindPort();
}
void LanServer::incomingConnection(int socketDescriptor){
	if (Ruler->GetGameStarted()) return;
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
	Ruler->SendServerInfos();
}
void LanServer::EraseThread(int a){
	Ruler->SomebodyDisconnected(a);
	QMap<int,LanServerThread*>::iterator index=clients.find(a);
	if (index!=clients.end()){
		index.value()->deleteLater();
		clients.erase(index);
	}
}

void LanServer::IncomingJoinRequest(int a, QString nam, QPixmap avat){
	if (Ruler->GetGameStarted()) return;
	if (!clients.value(a,NULL)) return;
	if (Ruler->GetNumPlayers()>=Ruler->GetMaxPlayer()){
		emit ServerIsFull(a);
		return;
	}
	LanServerThread* TempPoint=clients[a];
	connect(this,SIGNAL(ChatMessage(QString)),TempPoint,SIGNAL(SendMessage(QString)));
	connect(TempPoint,SIGNAL(ChatMessageRecieved(QString)),this,SIGNAL(ChatMessage(QString)));
	connect(TempPoint,SIGNAL(ReadyToPlay(int,bool)),Ruler,SLOT(IsReadyToPlay(int,bool)));
	connect(TempPoint,SIGNAL(DeckSetUp(int,CardDeck)),Ruler,SLOT(DeckSetUp(int,CardDeck)));
	connect(this,SIGNAL(Joined(QString)),TempPoint,SIGNAL(Joined(QString)));
	connect(this,SIGNAL(LeftTheGame(QString)),TempPoint,SIGNAL(LeftTheGame(QString)));
	connect(this,SIGNAL(YourNameColor(int,QString,QColor)),TempPoint,SIGNAL(YourNameColor(int,QString,QColor)));
	connect(this,SIGNAL(InvalidDeck(int)),TempPoint,SIGNAL(InvalidDeck(int)));
	connect(this,SIGNAL(PlayOrder(QList<int>)),TempPoint,SIGNAL(PlayOrder(QList<int>)));
	connect(this,SIGNAL(PlayersNameAvatar(QMap<int,QString>,QMap<int,QPixmap>)),TempPoint,SIGNAL(PlayersNameAvatar(QMap<int,QString>,QMap<int,QPixmap>)));
	connect(this,SIGNAL(PlayerHand(int,QList<CardData>)),TempPoint,SIGNAL(PlayerHand(int,QList<CardData>)));
	connect(this,SIGNAL(PlayerLibrary(int,QList<CardData>)),TempPoint,SIGNAL(PlayerLibrary(int,QList<CardData>)));
	connect(TempPoint,SIGNAL(Mulligan(int)),Ruler,SLOT(DoMulligan(int)));
	connect(TempPoint,SIGNAL(HandAccepted(int)),Ruler,SLOT(AcceptedHand(int)));
	connect(this,SIGNAL(WaitingFor(int,QString)),TempPoint,SIGNAL(WaitingFor(int,QString)));
	connect(this,SIGNAL(StopWaitingFor()),TempPoint,SIGNAL(StopWaitingFor()));
	connect(this,SIGNAL(CurrentPhaseChanged(int)),TempPoint,SIGNAL(CurrentPhaseChanged(int)));
	connect(this,SIGNAL(CardsToUntap(QList<int>)),TempPoint,SIGNAL(CardsToUntap(QList<int>)));
	connect(this,SIGNAL(CardDrawn(int,CardData)),TempPoint,SIGNAL(CardDrawn(int,CardData)));
	connect(TempPoint,SIGNAL(TimerFinished(int)),Ruler,SLOT(TimerFinished(int)));
	connect(TempPoint,SIGNAL(TimerStopped(int)),Ruler,SLOT(TimerStopped(int)));
	connect(TempPoint,SIGNAL(TimerResumed(int)),Ruler,SLOT(ResumeTimer(int)));
	connect(this,SIGNAL(StopTimers()),TempPoint,SIGNAL(StopTimers()));
	connect(this,SIGNAL(StopTurnTimer()),TempPoint,SIGNAL(StopTurnTimer()));
	connect(this,SIGNAL(ResumeTurnTimer()),TempPoint,SIGNAL(ResumeTurnTimer()));
	connect(this,SIGNAL(ResumeStackTimer()),TempPoint,SIGNAL(ResumeStackTimer()));
	connect(this,SIGNAL(EffectAddedToStack(quint32,const EffectData&)),TempPoint,SIGNAL(EffectAddedToStack(quint32,const EffectData&)));
	connect(this,SIGNAL(EffectResolved()),TempPoint,SIGNAL(EffectResolved()));
	connect(this,SIGNAL(PlayableCards(int,QList<int>)),TempPoint,SIGNAL(PlayableCards(int,QList<int>)));
	connect(TempPoint,SIGNAL(WantPlayCard(int,int)),Ruler,SLOT(WantsToPlayCard(int,int)));
	connect(this,SIGNAL(PlayedCard(int,const CardData&)),TempPoint,SIGNAL(PlayedCard(int,const CardData&)));
	Ruler->IncomingJoinRequest(a,nam,avat);
}