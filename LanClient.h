#ifndef LANCLIENT_H
#define LANCLIENT_H
#include <QObject>
#include <QThread>
#include <QAbstractSocket>
class QSslSocket;
class LanClient : public QObject{
	Q_OBJECT
private:
	QString HostIP;
	int ListenPort;
	QSslSocket* tcpSocket;
	quint32 nextBlockSize;
public:
	const QString& GetHostIP() const {return HostIP;}
	void SetHostIP(const QString& a);
	int GetListenPort() const {return ListenPort;}
	void SetListenPort(int a){ListenPort=a;}
	LanClient(QObject* parent);
signals:
	void error(QAbstractSocket::SocketError socketError);
	void Connected();
	void Disconnected();
	void CantConnect();
	void CantSendData();
	void ChatMessageRecieved(QString Message);
private slots:
	void IncomingTransmission();
public slots:
	void ConnectToHost();
	void SendChatMessage(QString& Message);
};
#endif