#ifndef LANSERVERSOCKET_H
#define LANSERVERSOCKET_H
#include "ComunicationConstants.h"
#ifdef USE_SSL
#include <QSslSocket>
#endif
#ifndef USE_SSL
#include <QTcpSocket>
#endif
class LanServerSocket : public
#ifndef USE_SSL
QTcpSocket
#endif
#ifdef USE_SSL
QSslSocket
#endif
{
	Q_OBJECT
public:
	LanServerSocket(int soketDescriptor, QObject *parent = 0);
private:
	int SocketID;
	quint32 nextBlockSize;
private slots:
	void readClient();
public slots:
	void SendMessage(QString msg);
	void SendServerInfos(QString nam,int gamemode,int legal,int minp,int maxp,int currp);
	void SendServerIsFull(int SocID);
	void SendJoined(QString nam);
	void SendLeftTheGame(QString nam);
signals:
	void ChatMessageRecieved(QString);
	void RequestJoin(int,QString);
	void ReadyToPlay(int,bool);
};
#endif