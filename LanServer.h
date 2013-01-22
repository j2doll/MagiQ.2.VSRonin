#ifndef LANSERVER_H
#define LANSERVER_H
#include <QTcpServer>
#include <QMap>
#include <QList>
#include "LanServerThread.h"
#include "ComunicationConstants.h"
#include "CostantsDefinition.h"
class LanServer : public QTcpServer{
	Q_OBJECT
private:
	int PortToListen;
	QString ServerName;
	int GameMode;
	int DecksFormat;
	int MinPlayer;
	int MaxPlayer;
	QMap<int,LanServerThread*> clients;
	QList<int> PlayersSockets;
	QList<QString> PlayerNames;
	QMap<int,bool> ReadyPlayers;
	bool EverybodyReady() const;
	void StartMatch(){}
protected:
	void incomingConnection(int socketDescriptor);
public:
	int GetPortToListen() const {return PortToListen;}
	void SetPortToListen(int a) {PortToListen=a;}
	LanServer(QObject* parent=0);
	const QString& GetServerName() const {return ServerName;}
	void SetServerName(const QString& a){ServerName=a;}
	int GetGameMode() const {return GameMode;}
	void SetGameMode(int a){if(a>=0 && a<Constants::GameMode::END) GameMode=a;}
	int GetDecksFormat() const {return DecksFormat;}
	void SetDecksFormat(int a){if(a>=0 && a<Constants::Legality::END) DecksFormat=a;}
	int GetMinPlayer() const {return MinPlayer;}
	void SetMinPlayer(int a){if(a>1 && a<5) MinPlayer=a;}
	int GetMaxPlayer() const {return MaxPlayer;}
	void SetMaxPlayer(int a){if(a>1 && a<5) MaxPlayer=a;}
signals:
	void ChatMessage(QString);
	void CantBindPort();
	void ServerInfos(QString,int,int,int,int,int);
	void ServerIsFull(int);
	void LeftTheGame(QString);
	void Joined(QString);
public slots:
	void StartListening(){if (!listen(QHostAddress::Any, PortToListen))emit CantBindPort();}
private slots:
	void EraseThread(int a);
	void SendServerInfos(){emit ServerInfos(ServerName,GameMode,DecksFormat,MinPlayer,MaxPlayer,PlayersSockets.size());}
	void IncomingJoinRequest(int a,QString nam);
	void SendLeftTheGame(QString a){emit LeftTheGame(a);}
	void IsReadyToPlay(int a,bool ready);
};
#endif