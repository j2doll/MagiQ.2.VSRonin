#include "LanServerThread.h"
#include "LanServerSocket.h"
LanServerThread::LanServerThread(int SockDesc,QObject* parent)
	:QThread(parent)
	,SocketID(SockDesc)
{
	tcpSocket=new LanServerSocket(SockDesc,this);
#ifdef USE_SSL
	connect(tcpSocket,SIGNAL(sslErrors(const QList<QSslError>&)),this,SIGNAL(sslErrors(const QList<QSslError>&)));
#endif
	connect(tcpSocket,SIGNAL(ChatMessageRecieved(QString)),this,SIGNAL(ChatMessageRecieved(QString)));
	connect(this,SIGNAL(SendMessage(QString)),tcpSocket,SLOT(SendMessage(QString)));
	connect(this,SIGNAL(ServerInfos(QString,int,int,int,int,int)),tcpSocket,SLOT(SendServerInfos(QString,int,int,int,int,int)));
	connect(tcpSocket,SIGNAL(RequestJoin(int,QString,QPixmap)),this,SIGNAL(RequestJoin(int,QString,QPixmap)));
	connect(this,SIGNAL(ServerIsFull(int)),tcpSocket,SLOT(SendServerIsFull(int)));
	connect(this,SIGNAL(Joined(QString)),tcpSocket,SLOT(SendJoined(QString)));
	connect(this,SIGNAL(YourNameColor(int,QString,QColor)),tcpSocket,SLOT(SendYourNameColor(int,QString,QColor)));
	connect(this,SIGNAL(LeftTheGame(QString)),tcpSocket,SLOT(SendLeftTheGame(QString)));
	connect(tcpSocket,SIGNAL(ReadyToPlay(int,bool)),this,SIGNAL(ReadyToPlay(int,bool)));
	connect(tcpSocket,SIGNAL(DeckSetUp(int,CardDeck)),this,SIGNAL(DeckSetUp(int,CardDeck)));
	connect(this,SIGNAL(InvalidDeck(int)),tcpSocket,SLOT(SendInvalidDeck(int)));
	connect(this,SIGNAL(GameHasStarted()),tcpSocket,SLOT(SendGameHasStarted()));
	connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(stop()));
	connect(this,SIGNAL(finished()),this,SLOT(SendReachedEnd()));
	connect(this,SIGNAL(PlayOrder(QList<int>)),tcpSocket,SLOT(SendPlayOrder(QList<int>)));
	connect(this,SIGNAL(PlayersNameAvatar(QMap<int,QString>,QMap<int,QPixmap>)),tcpSocket,SLOT(SendPlayersNameAvatar(QMap<int,QString>,QMap<int,QPixmap>)));
	connect(this,SIGNAL(PlayerHand(int,QList<CardData>)),tcpSocket,SLOT(SendPlayerHand(int,QList<CardData>)));
	connect(this,SIGNAL(PlayerLibrary(int,QList<CardData>)),tcpSocket,SLOT(SendPlayerLibrary(int,QList<CardData>)));
	connect(tcpSocket,SIGNAL(Mulligan(int)),this,SIGNAL(Mulligan(int)));
	connect(tcpSocket,SIGNAL(HandAccepted(int)),this,SIGNAL(HandAccepted(int)));
	connect(this,SIGNAL(WaitingFor(int,QString)),tcpSocket,SLOT(SendWaitingFor(int,QString)));
	connect(this,SIGNAL(StopWaitingFor()),tcpSocket,SLOT(SendStopWaitingFor()));
	connect(this,SIGNAL(CurrentPhaseChanged(int)),tcpSocket,SLOT(SendCurrentPhaseChanged(int)));
	connect(this,SIGNAL(CardsToUntap(QList<int>)),tcpSocket,SLOT(SendCardsToUntap(QList<int>)));
	connect(this,SIGNAL(CardDrawn(int,CardData)),tcpSocket,SLOT(SendCardDrawn(int,CardData)));
	connect(tcpSocket,SIGNAL(TimerFinished(int)),this,SIGNAL(TimerFinished(int)));
	connect(tcpSocket,SIGNAL(TimerStopped(int)),this,SIGNAL(TimerStopped(int)));
	connect(tcpSocket,SIGNAL(TimerResumed(int)),this,SIGNAL(TimerResumed(int)));
	connect(this,SIGNAL(StopTimers()),tcpSocket,SLOT(SendStopTimers()));
	connect(this,SIGNAL(StopTurnTimer()),tcpSocket,SLOT(SendStopTurnTimer()));
	connect(this,SIGNAL(ResumeTurnTimer()),tcpSocket,SLOT(SendResumeTurnTimer()));
	connect(this,SIGNAL(ResumeStackTimer()),tcpSocket,SLOT(SendResumeStackTimer()));
	connect(this,SIGNAL(EffectAddedToStack(quint32,const EffectData&)),tcpSocket,SLOT(SendEffectAddedToStack(quint32,const EffectData&)));
}