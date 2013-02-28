#ifndef LANSERVERSOCKET_H
#define LANSERVERSOCKET_H
#include "ComunicationConstants.h"
#include "Deck.h"
#include <QHash>
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
	void SendPlayerHand(int SocID,QList<int> hand);
	void SendPlayerLibrary(int SocID,int libr);
	void SendPlayOrder(QList<int> Order);
	void SendPlayersNameAvatar(QMap<int,QString> nam,QMap<int,QPixmap> avt);
	void SendWaitingFor(int socID,QString msg);
	void SendStopWaitingFor();
	void SendCurrentPhaseChanged(int newphase);
	void SendCardsToUntap(QList<int> crds);
	void SendCardDrawn(int socID,int crd);
	void SendStopTimers();
	void SendStopTurnTimer();
	void SendResumeTurnTimer();
	void SendResumeStackTimer();
	void SendEffectAddedToStack(quint32 crd,const EffectData& eff);
	void SendEffectResolved();
	void SendPlayableCards(int socID,QList<int> IDs);
	void SendPlayedCard(int who,int crd);
	void SendRemoveFromHand(int who,int crdID);
	void SendPermanentResolved(int who,int crd);
	void SendAllCards(QList<CardData> crds);
	void SendCardsToTap(QList<int> crdIDs);
	void SendAttackAbleCards(int who,QList<int> crdIDs);
	void SendAttackingCards(const QHash<int,int>& crdIDs);
	void SendIsMyTurn(int socID);
signals:
	void ChatMessageRecieved(QString);
	void RequestJoin(int,QString,QPixmap);
	void ReadyToPlay(int,bool);
	void DeckSetUp(int,CardDeck);
	void Mulligan(int);
	void HandAccepted(int);
	void TimerFinished(int);
	void TimerStopped(int);
	void TimerResumed(int);
	void WantPlayCard(int socID,int crdID,QList<int>);
	void ContinueToNextPhase(int);
	void AttackingCards(int,QHash<int,int>);
};
#endif