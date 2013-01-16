#ifndef LANSERVERTHREAD_H
#define LANSERVERTHREAD_H
#include <QThread>
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
private slots:
	void readData();
};
#endif
