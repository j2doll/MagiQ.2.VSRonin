#include "LanClient.h"
#include "ComunicationConstants.h"
#include <QSslSocket>
LanClient::LanClient(QObject* parent)
	:QObject(parent)
	,HostIP("localhost")
	,ListenPort(Comunications::DefaultTCPport)
{
	tcpSocket=new QSslSocket(this);
	connect(tcpSocket,SIGNAL(connected()),this,SIGNAL(Connected()));
	connect(tcpSocket,SIGNAL(disconnected()),this,SIGNAL(Disconnected()));
	connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(IncomingTransmission()));
	connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this,SIGNAL(error(QAbstractSocket::SocketError)));
}
void LanClient::SetHostIP(const QString& a){
	QRegExp IPValidator("^([1-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.([1-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])$");
	if (IPValidator.exactMatch(a) || a=="localhost")
		HostIP=a;
}