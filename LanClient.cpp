#include "LanClient.h"
#ifdef USE_SSL
#include <QSslSocket>
#endif
#ifndef USE_SSL
#include <QTcpSocket>
#endif
#include <QRegExp>
LanClient::LanClient(QObject* parent)
	:QObject(parent)
	,HostIP("localhost")
	,ListenPort(Comunications::DefaultTCPport)
	,nextBlockSize(0)
	,IsReady(false)
{
#ifdef USE_SSL
	tcpSocket=new QSslSocket(this);
#endif
#ifndef USE_SSL
	tcpSocket=new QTcpSocket(this);
#endif
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
QString LanClient::GetSocketErrorString() const{return tcpSocket->errorString();}
void LanClient::disconnectFromHost(){tcpSocket->disconnectFromHost();}
void LanClient::ConnectToHost(){
	tcpSocket->connectToHost(HostIP,ListenPort);
#ifdef USE_SSL
	tcpSocket->startClientEncryption();
#endif
}
void LanClient::IncomingTransmission(){
	QDataStream incom(tcpSocket);
	incom.setVersion(QDataStream::Qt_4_7);
	quint32 RequestType;
	QString strings;
	qint32 int1,int2,int3,int4,int5;
	forever {
		if (nextBlockSize == 0) {
			if (tcpSocket->bytesAvailable() < sizeof(quint32))
				break;
			incom >> nextBlockSize;
		}
		if (tcpSocket->bytesAvailable() < nextBlockSize)
			break;
		incom >> RequestType;
		if(RequestType==Comunications::TransmissionType::ChatMessage){
			incom >> strings;
			emit ChatMessageRecieved(strings);
		}
		if(RequestType==Comunications::TransmissionType::SeverInformations){
			incom >> strings
				>>int1
				>>int2
				>>int3
				>>int4
				>>int5;
			emit ServerInfos(strings,int1,int2,int3,int4,int5);
		}
		if(RequestType==Comunications::TransmissionType::ServerFull){
			emit ServerIsFull();
		}
		nextBlockSize = 0;
	}
}
void LanClient::SendChatMessage(QString Message){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::ChatMessage) << Message;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	tcpSocket->write(block);
}
void LanClient::SendJoinRequest(){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::JoinRequest);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	tcpSocket->write(block);
}
void LanClient::SendReady(){
	IsReady=!IsReady;
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::ReadyToPlay) << IsReady;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	tcpSocket->write(block);
}