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
	void ServerIsFull(int);
public slots:
	void StartListening();
private slots:
	void EraseThread(int a);
	void IncomingJoinRequest(int a,QString nam,QPixmap avat);
};
#endif