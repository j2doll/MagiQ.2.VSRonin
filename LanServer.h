#ifndef LANSERVER_H
#define LANSERVER_H
#include <QTcpServer>
#include <QMap>
#include "LanServerThread.h"
#include "ComunicationConstants.h"
class LanServer : public QTcpServer{
	Q_OBJECT
private:
	int PortToListen;
	QMap<int,LanServerThread*> clients;
protected:
	void incomingConnection(int socketDescriptor);
public:
	int GetPortToListen() const {return PortToListen;}
	void SetPortToListen(int a) {PortToListen=a;}
	LanServer(QObject* parent=0);
signals:
	void ChatMessage(QString Message);
	void CantBindPort();
public slots:
	void StartListening(){if (!listen(QHostAddress::Any, PortToListen))emit CantBindPort();}
private slots:
	void EraseThread(int a);
};
#endif