#include "LanServer.h"
#include "ComunicationConstants.h"
#include "Judge.h"
LanServer::LanServer(QObject* parent)
	:QTcpServer(parent)
	,PortToListen(Comunications::DefaultTCPport)
{
	Ruler=new Judge(this);
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
	connect(Ruler,SIGNAL(ServerInfos(QString,int,int,int,int,int)),TempPoint,SIGNAL(ServerInfos(QString,int,int,int,int,int)));
	connect(TempPoint,SIGNAL(RequestJoin(int,QString,QPixmap)),this,SLOT(IncomingJoinRequest(int,QString,QPixmap)));
	connect(this,SIGNAL(ServerIsFull(int)),TempPoint,SIGNAL(ServerIsFull(int)));
	connect(TempPoint,SIGNAL(ReachedEnd(int)),this,SLOT(EraseThread(int)));
	connect(Ruler,SIGNAL(GameHasStarted()),TempPoint,SIGNAL(GameHasStarted()));
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
	connect(Ruler,SIGNAL(Joined(QString)),TempPoint,SIGNAL(Joined(QString)));
	connect(Ruler,SIGNAL(LeftTheGame(QString)),TempPoint,SIGNAL(LeftTheGame(QString)));
	connect(Ruler,SIGNAL(YourNameColor(int,QString,QColor)),TempPoint,SIGNAL(YourNameColor(int,QString,QColor)));
	connect(Ruler,SIGNAL(InvalidDeck(int)),TempPoint,SIGNAL(InvalidDeck(int)));
	connect(Ruler,SIGNAL(PlayOrder(QList<int>)),TempPoint,SIGNAL(PlayOrder(QList<int>)));
	connect(Ruler,SIGNAL(PlayersNameAvatar(QMap<int,QString>,QMap<int,QPixmap>)),TempPoint,SIGNAL(PlayersNameAvatar(QMap<int,QString>,QMap<int,QPixmap>)));
	connect(Ruler,SIGNAL(AllCards(QList<CardData>)),TempPoint,SIGNAL(AllCards(QList<CardData>)));
	connect(Ruler,SIGNAL(PlayerHand(int,QList<int>)),TempPoint,SIGNAL(PlayerHand(int,QList<int>)));
	connect(Ruler,SIGNAL(PlayerLibrary(int,int)),TempPoint,SIGNAL(PlayerLibrary(int,int)));
	connect(TempPoint,SIGNAL(Mulligan(int)),Ruler,SLOT(DoMulligan(int)));
	connect(TempPoint,SIGNAL(HandAccepted(int)),Ruler,SLOT(AcceptedHand(int)));
	connect(Ruler,SIGNAL(WaitingFor(int,QString)),TempPoint,SIGNAL(WaitingFor(int,QString)));
	connect(Ruler,SIGNAL(StopWaitingFor()),TempPoint,SIGNAL(StopWaitingFor()));
	connect(Ruler,SIGNAL(CurrentPhaseChanged(int)),TempPoint,SIGNAL(CurrentPhaseChanged(int)));
	connect(Ruler,SIGNAL(CardsToUntap(QList<int>)),TempPoint,SIGNAL(CardsToUntap(QList<int>)));
	connect(Ruler,SIGNAL(CardDrawn(int,int)),TempPoint,SIGNAL(CardDrawn(int,int)));
	connect(TempPoint,SIGNAL(TimerFinished(int)),Ruler,SLOT(TimerFinished(int)));
	connect(TempPoint,SIGNAL(TimerStopped(int)),Ruler,SLOT(TimerStopped(int)));
	connect(TempPoint,SIGNAL(TimerResumed(int)),Ruler,SLOT(ResumeTimer(int)));
	connect(Ruler,SIGNAL(StopTimers()),TempPoint,SIGNAL(StopTimers()));
	connect(Ruler,SIGNAL(StopTurnTimer()),TempPoint,SIGNAL(StopTurnTimer()));
	connect(Ruler,SIGNAL(ResumeTurnTimer()),TempPoint,SIGNAL(ResumeTurnTimer()));
	connect(Ruler,SIGNAL(ResumeStackTimer()),TempPoint,SIGNAL(ResumeStackTimer()));
	connect(Ruler,SIGNAL(EffectAddedToStack(quint32,const EffectData&)),TempPoint,SIGNAL(EffectAddedToStack(quint32,const EffectData&)));
	connect(Ruler,SIGNAL(EffectResolved()),TempPoint,SIGNAL(EffectResolved()));
	connect(Ruler,SIGNAL(PlayableCards(int,QList<int>)),TempPoint,SIGNAL(PlayableCards(int,QList<int>)));
	connect(TempPoint,SIGNAL(WantPlayCard(int,int,QList<int>)),Ruler,SLOT(WantsToPlayCard(int,int,QList<int>)));
	connect(Ruler,SIGNAL(PlayedCard(int,int)),TempPoint,SIGNAL(PlayedCard(int,int)));
	connect(Ruler,SIGNAL(RemoveFromHand(int,int)),TempPoint,SIGNAL(RemoveFromHand(int,int)));
	connect(Ruler,SIGNAL(PermanentResolved(int,int)),TempPoint,SIGNAL(PermanentResolved(int,int)));
	connect(Ruler,SIGNAL(CardsToTap(QList<int>)),TempPoint,SIGNAL(CardsToTap(QList<int>)));
	connect(TempPoint,SIGNAL(ContinueToNextPhase(int)),Ruler,SLOT(ContinueToNextPhase(int)));
	connect(Ruler,SIGNAL(AttackAbleCards(int,QList<int>)),TempPoint,SIGNAL(AttackAbleCards(int,QList<int>)));
	connect(TempPoint,SIGNAL(AttackingCards(int,QHash<int,int>)),Ruler,SLOT(SetAttackingCards(int,QHash<int,int>)));
	connect(Ruler,SIGNAL(SendAttackingCards(const QHash<int,int>&)),TempPoint,SIGNAL(SendAttackingCards(const QHash<int,int>&)));
	Ruler->IncomingJoinRequest(a,nam,avat);
}