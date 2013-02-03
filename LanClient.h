#ifndef LANCLIENT_H
#define LANCLIENT_H
#include <QObject>
#include <QThread>
#include <QAbstractSocket>
#include "ComunicationConstants.h"
#include <QPixmap>
#include "Deck.h"
class QSslSocket;
class QTcpSocket;
class LanClient : public QObject{
	Q_OBJECT
private:
	QString HostIP;
	int ListenPort;
	bool IsReady;
	QPixmap MyAvatar;
	QString MyName;
	CardDeck MyDeck;
#ifdef USE_SSL
	QSslSocket* tcpSocket;
#endif
#ifndef USE_SSL
	QTcpSocket* tcpSocket;
#endif
	quint32 nextBlockSize;
public:
	void SetDeck(const CardDeck& a){MyDeck=a;}
	const CardDeck& GetDeck() const {return MyDeck;}
	void SetAvatar(const QPixmap& a){MyAvatar=a;}
	void SetUsername(const QString& a){MyName=a;}
	const QString& GetHostIP() const {return HostIP;}
	int GetListenPort() const {return ListenPort;}
	void SetListenPort(int a){ListenPort=a;}
	LanClient(QObject* parent);
	QString GetSocketErrorString() const;
signals:
	void error(QAbstractSocket::SocketError socketError);
	void Connected();
	void Disconnected();
	void CantConnect();
	void CantSendData();
	void ChatMessageRecieved(QString Message);
	void ServerInfos(QString,int,int,int,int,int);
	void ServerIsFull();
	void UserJoined(QString);
	void UserLeft(QString);
	void MyNameAndColor(QString,QColor);
	void InvalidDeck();
	void GameHasStarted();
	void PlayOrder(QList<int>);
	void PlayersNameAvatar(QMap<int,QString>,QMap<int,QPixmap>);
	void MyHand(QList<CardData>);
	void OtherHand(int whos,int numcards);
	void MyLibrary(QList<CardData>);
	void OtherLibrary(int whos,int numcards);
	void WaitingFor(QString);
	void StopWaitingFor();
	void CurrentPhaseChanged(int);
	void CardsToUntap(QList<int>);
	void CardDrawn(CardData);
	void OtherDrawn(int);
	void StopTimer();
	void StopTurnTimer();
	void ResumeTurnTimer();
	void ResumeStackTimer();
	void EffectAddedToStack(EffectData,quint32);
	void EffectResolved();
private slots:
	void IncomingTransmission();
public slots:
	void SetHostIP(const QString& a);
	void ConnectToHost();
	void SendChatMessage(QString Message);
	void SendJoinRequest();
	void disconnectFromHost();
	void SendReady();
	void SendMulligan();
	void SendHandAccepted();
	void SendFinishedTimer();
	void SendStoppedTimer();
	void SendResumeTimer();

};
#endif