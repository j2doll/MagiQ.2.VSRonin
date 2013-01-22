#ifndef LANCLIENT_H
#define LANCLIENT_H
#include <QObject>
#include <QThread>
#include <QAbstractSocket>
#include "ComunicationConstants.h"
#include "MagiQPlayer.h"
class QSslSocket;
class QTcpSocket;
class MagiQPlayer;
class LanClient : public QObject{
	Q_OBJECT
private:
	QString HostIP;
	int ListenPort;
	bool IsReady;
	MagiQPlayer ClientPlayer;
#ifdef USE_SSL
	QSslSocket* tcpSocket;
#endif
#ifndef USE_SSL
	QTcpSocket* tcpSocket;
#endif
	quint32 nextBlockSize;
public:
	void SetAvatar(const QPixmap& a){ClientPlayer.SetAvatar(a);}
	void SetUsername(const QString& a){ClientPlayer.SetPlayerName(a); emit UsernameChanged(a);}
	void SetUserColor(const QString& a){ClientPlayer.SetPlayerColor(a); emit UserColorChanged(ClientPlayer.GetPlayerColor());}
	void SetUserColor(const QColor& a){ClientPlayer.SetPlayerColor(a); emit UserColorChanged(a);}
	MagiQPlayer& GetClientPlayer() {return ClientPlayer;}
	const QString& GetHostIP() const {return HostIP;}
	int GetListenPort() const {return ListenPort;}
	void SetListenPort(int a){ListenPort=a;}
	LanClient(QObject* parent);
	QString GetSocketErrorString() const;
signals:
	void UsernameChanged(QString);
	void UserColorChanged(QColor);
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
private slots:
	void IncomingTransmission();
public slots:
	void SetHostIP(const QString& a);
	void ConnectToHost();
	void SendChatMessage(QString Message);
	void SendJoinRequest();
	void disconnectFromHost();
	void SendReady();
};
#endif