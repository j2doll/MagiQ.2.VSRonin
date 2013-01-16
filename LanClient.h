#ifndef LANCLIENT_H
#define LANCLIENT_H
#include <QObject>
class QSslSocket;
class LanClient : public QObject{
	Q_OBJECT
private:
	QString HostIP;
	int ListenPort;
	QSslSocket* tcpSocket;
	int nextBlockSize;
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
private slots:
	void IncomingTransmission();
};
#endif