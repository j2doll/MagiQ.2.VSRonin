#include "LanServerThread.h"
#include "ComunicationConstants.h"
#include <QSslSocket>
LanServerThread::LanServerThread(int SockDesc,QObject* parent)
	:QThread(parent)
	,SocketID(SockDesc)
	,nextBlockSize(0)
{
	tcpSocket=new QSslSocket(this);
	connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(ClientDisconnected()));
	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
	connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this,SIGNAL(error(QAbstractSocket::SocketError)));
}
void LanServerThread::run(){
	if (!tcpSocket->setSocketDescriptor(SocketID)) {
		emit error(tcpSocket->error());
		return;
	}
	tcpSocket->startServerEncryption();
	exec();
}
void LanServerThread::readData(){
	QDataStream incom(tcpSocket);
	incom.setVersion(QDataStream::Qt_4_7);
	quint32 RequestType;
	QString strings;
	forever {
		if (nextBlockSize == 0) {
			if (tcpSocket->bytesAvailable() < sizeof(quint32))
				break;
			incom >> nextBlockSize;
		}
		if (nextBlockSize == 0xFFFF) {
			nextBlockSize = 0;
			continue;
		}
		if (tcpSocket->bytesAvailable() < nextBlockSize)
			break;
		incom >> RequestType;
		if(RequestType==Comunications::TransmissionType::ChatMessage){
			incom >> strings;
			emit ChatMessageRecieved(strings);
		}
		nextBlockSize = 0;
	}
}
void LanServerThread::SendChatMessage(QString& Message){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::ChatMessage) << Message << quint32(0xFFFF);
	out.device()->seek(0);
	out << quint32(block.size() - 2*sizeof(quint32));
	tcpSocket->write(block);
	if(!tcpSocket->waitForBytesWritten(5000)) emit CantSendData();
}