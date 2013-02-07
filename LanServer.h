#ifndef LANSERVER_H
#define LANSERVER_H
#include <QTcpServer>
#include "LanServerThread.h"
class Judge;
class LanServer : public QTcpServer{
	Q_OBJECT
private:
	Judge* Ruler;
	int PortToListen;
	QMap<int,LanServerThread*> clients;
protected:
	void incomingConnection(int socketDescriptor);
public:
	int GetPortToListen() const {return PortToListen;}
	void SetPortToListen(int a) {PortToListen=a;}
	LanServer(QObject* parent=0);
signals:
	void ChatMessage(QString);
	void CantBindPort();
	void ServerInfos(QString,int,int,int,int,int);
	void ServerIsFull(int);
	void LeftTheGame(QString);
	void Joined(QString);
	void YourNameColor(int,QString,QColor);
	void InvalidDeck(int);
	void PlayerHand(int,QList<CardData>);
	void PlayerLibrary(int,QList<CardData>);
	void PlayOrder(QList<int>);
	void PlayersNameAvatar(QMap<int,QString>,QMap<int,QPixmap>);
	void GameHasStarted();
	void WaitingFor(int,QString);
	void StopWaitingFor();
	void CurrentPhaseChanged(int);
	void CardsToUntap(QList<int>);
	void CardDrawn(int,CardData);
	void StopTimers();
	void ResumeTimers();
	void StopTurnTimer();
	void ResumeTurnTimer();
	void ResumeStackTimer();
	void EffectAddedToStack(quint32,const EffectData&);
	void EffectResolved();
	void PlayableCards(int,QList<int>);
	void PlayedCard(int Who,const CardData& crd);
public slots:
	void StartListening();
private slots:
	void EraseThread(int a);
	void IncomingJoinRequest(int a,QString nam,QPixmap avat);
};
#endif