#ifndef LANSERVERSOCKET_H
#define LANSERVERSOCKET_H
#include "ComunicationConstants.h"
#include "Deck.h"
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
	bool IsWaiting;
private slots:
	void readClient();
public slots:
	void SendMessage(QString msg);
	void SendServerInfos(QString nam,int gamemode,int legal,int minp,int maxp,int currp);
	void SendServerIsFull(int SocID);
	void SendJoined(QString nam);
	void SendLeftTheGame(QString nam);
	void SendYourNameColor(int SocID,QString nam,QColor col);
	void SendInvalidDeck(int SocID);
	void SendGameHasStarted();
	void SendPlayerHand(int SocID,QList<CardData> hand);
	void SendPlayerLibrary(int SocID,QList<CardData> libr);
	void SendPlayOrder(QList<int> Order);
	void SendPlayersNameAvatar(QMap<int,QString> nam,QMap<int,QPixmap> avt);
	void SendWaitingFor(int socID,QString msg);
	void SendStopWaitingFor();
	void SendCurrentPhaseChanged(int newphase);
	void SendCardsToUntap(QList<int> crds);
	void SendCardDrawn(int socID,CardData crd);
signals:
	void ChatMessageRecieved(QString);
	void RequestJoin(int,QString,QPixmap);
	void ReadyToPlay(int,bool);
	void DeckSetUp(int,CardDeck);
	void Mulligan(int);
	void HandAccepted(int);
};
#endif