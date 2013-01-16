#ifndef LANSERVER_H
#define LANSERVER_H
#include <QTcpServer>
#include <QMap>
#include "LanServerThread.h"
class LanServer : public QTcpServer{
	Q_OBJECT
private:
	QMap<int,LanServerThread*> clients;
protected:
	void incomingConnection(int socketDescriptor);
public:
	LanServer(QObject* parent=0);
};
#endif