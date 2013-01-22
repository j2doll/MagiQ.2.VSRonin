#include "LanServerSocket.h"
LanServerSocket::LanServerSocket(int soketDescriptor, QObject *parent)
#ifdef USE_SSL
	:QSslSocket(parent)
#endif
#ifndef USE_SSL
	:QTcpSocket(parent)
#endif
	,SocketID(soketDescriptor)
	,nextBlockSize(0)
{
	if (!setSocketDescriptor(SocketID))
		emit error(error());
#ifdef USE_SSL
	startServerEncryption();
#endif
	connect(this, SIGNAL(readyRead()), this, SLOT(readClient()));
}
void LanServerSocket::readClient(){
	QDataStream incom(this);
	incom.setVersion(QDataStream::Qt_4_7);
	quint32 RequestType;
	QString strings;
	bool booleans;
	forever {
		if (nextBlockSize == 0) {
			if (bytesAvailable() < sizeof(quint32))
				break;
			incom >> nextBlockSize;
		}
		if (bytesAvailable() < nextBlockSize)
			break;
		incom >> RequestType;

///////////////////////////////////////////////////////////////////////////
		if(RequestType==Comunications::TransmissionType::ChatMessage){
			incom >> strings;
			emit ChatMessageRecieved(strings);
		}
		if(RequestType==Comunications::TransmissionType::JoinRequest){
			incom >> strings;
			emit RequestJoin(SocketID,strings);
		}
		if(RequestType==Comunications::TransmissionType::ReadyToPlay){
			incom >> booleans;
			emit ReadyToPlay(SocketID,booleans);
		}
///////////////////////////////////////////////////////////////////////////

		nextBlockSize = 0;
	}
}
void LanServerSocket::SendMessage(QString msg){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::ChatMessage) << msg;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendServerInfos(QString nam,int gamemode,int legal,int minp,int maxp,int currp){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) 
		<< quint32(Comunications::TransmissionType::SeverInformations)
		<< nam
		<< qint32(gamemode)
		<< qint32(legal)
		<< qint32(minp)
		<< qint32(maxp)
		<< qint32(currp)
		;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendServerIsFull(int SocID){
	if (SocketID!=SocID) return;
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::ServerFull);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendJoined(QString nam){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::PlayerJoins) << nam;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendLeftTheGame(QString nam){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::PlayerLeave) << nam;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}