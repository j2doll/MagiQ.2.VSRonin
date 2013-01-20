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
	forever {
		if (nextBlockSize == 0) {
			if (bytesAvailable() < sizeof(quint32))
				break;
			incom >> nextBlockSize;
		}
		if (bytesAvailable() < nextBlockSize)
			break;
		incom >> RequestType;
		if(RequestType==Comunications::TransmissionType::ChatMessage){
			incom >> strings;
			emit ChatMessageRecieved(strings);
		}
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