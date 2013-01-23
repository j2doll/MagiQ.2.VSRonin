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
	connect(this,SIGNAL(PlayerHand(int,QList<CardData>)),tcpSocket,SLOT(SendPlayerHand(int,QList<CardData>)));
	connect(this,SIGNAL(PlayerLibrary(int,QList<CardData>)),tcpSocket,SLOT(SendPlayerLibrary(int,QList<CardData>)));
	connect(tcpSocket,SIGNAL(Mulligan(int)),this,SIGNAL(Mulligan(int)));
	connect(tcpSocket,SIGNAL(HandAccepted(int)),this,SIGNAL(HandAccepted(int)));
}