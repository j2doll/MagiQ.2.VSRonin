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
	connect(tcpSocket,SIGNAL(RequestJoin(int)),this,SIGNAL(RequestJoin(int)));
	connect(this,SIGNAL(ServerIsFull(int)),tcpSocket,SLOT(SendServerIsFull(int)));
	connect(tcpSocket,SIGNAL(ReadyToPlay(int,bool)),this,SIGNAL(ReadyToPlay(int,bool)));
	connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(stop()));
	connect(this, SIGNAL(finished()), this, SLOT(SendReachedEnd()));
}