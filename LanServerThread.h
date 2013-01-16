#ifndef LANSERVERTHREAD_H
#define LANSERVERTHREAD_H
#include <QThread>
#include <QAbstractSocket>
class QSslSocket;
class LanServerThread : public QThread{
	Q_OBJECT
private:
	quint32 nextBlockSize;
	int SocketID;
	QSslSocket* tcpSocket;
public:
	LanServerThread(int SockDesc,QObject* parent=0);
	void run();
signals:
	void error(QAbstractSocket::SocketError socketError);
	void Disconnected(int a);
	void ChatMessageRecieved(QString Message);
	void CantSendData();
private slots:
	void ClientDisconnected(){emit Disconnected(SocketID);}
	void readData();
public slots:
	void SendChatMessage(QString& Message);
};
#endif
