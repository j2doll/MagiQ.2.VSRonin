#ifndef LANSERVERTHREAD_H
#define LANSERVERTHREAD_H
#include <QThread>
#include <QAbstractSocket>
#include <QColor>
#include <QPixmap>
#include <QMap>
#include "Deck.h"
#include "ComunicationConstants.h"
#ifdef USE_SSL
#include <QSslError>
#endif
class LanServerSocket;
class LanServerThread : public QThread{
	Q_OBJECT
private:
	int SocketID;
	LanServerSocket* tcpSocket;
public:
	LanServerThread(int SockDesc,QObject* parent=0);
public slots:
	void run(){exec();}
	void stop(){exit(0);}
signals:
	void ChatMessageRecieved(QString msg);
	void SendMessage(QString msg);
	void ReachedEnd(int SocID);
	void ServerInfos(QString,int,int,int,int,int);
	void RequestJoin(int,QString,QPixmap);
	void ServerIsFull(int);
	void ReadyToPlay(int,bool);
	void Joined(QString);
	void LeftTheGame(QString);
	void DeckSetUp(int,CardDeck);
	void YourNameColor(int,QString,QColor);
	void InvalidDeck(int);
	void GameHasStarted();
	void PlayerHand(int,QList<CardData>);
	void PlayerLibrary(int,QList<CardData>);
	void PlayOrder(QList<int>);
	void Mulligan(int);
	void HandAccepted(int);
	void PlayersNameAvatar(QMap<int,QString>,QMap<int,QPixmap>);
#ifdef USE_SSL
	void sslErrors(const QList<QSslError>& errors);
#endif
private slots:
	void SendReachedEnd(){emit ReachedEnd(SocketID);}
};
#endif
