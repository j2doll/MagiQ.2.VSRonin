#include "LanServerThread.h"
#include "LanServerSocket.h"
LanServerThread::LanServerThread(int SockDesc,QObject* parent)
	:QThread(parent)
	,SocketID(SockDesc)
{
	tcpSocket=new LanServerSocket(SockDesc,this);
	connect(tcpSocket,SIGNAL(ChatMessageRecieved(QString)),this,SIGNAL(ChatMessageRecieved(QString)));
	connect(this,SIGNAL(SendMessage(QString)),tcpSocket,SLOT(SendMessage(QString)));
	connect(this, SIGNAL(finished()), this, SLOT(SendReachedEnd()));
}