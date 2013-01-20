#ifndef LANSERVERSOCKET_H
#define LANSERVERSOCKET_H
#include "ComunicationConstants.h"
#ifdef USE_SSL
#include <QSslSocket>
#endif
#ifndef USE_SSL
#include <QTcpSocket>
#endif
#ifndef USE_SSL
class LanServerSocket : public QTcpSocket{
#endif
#ifdef USE_SSL
class LanServerSocket : public QSslSocket{
#endif
	Q_OBJECT
public:
	LanServerSocket(int soketDescriptor, QObject *parent = 0);
private:
	int SocketID;
	quint32 nextBlockSize;
private slots:
	void readClient();
public slots:
	void SendMessage(QString msg);
signals:
	void ChatMessageRecieved(QString msg);
};
#endif