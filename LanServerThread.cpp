#include "LanServerThread.h"
#include <QSslSocket>
LanServerThread::LanServerThread(int SockDesc,QObject* parent)
	:QThread(parent)
	,SocketID(SockDesc)
	,nextBlockSize(0)
{
	tcpSocket=new QSslSocket(this);
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
	qint32 RequestType;
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
		/*if(TipoRichiesta==Comunicazioni::RichiediInfo){
			emit GotRichiediInfo();
		}
		else if(TipoRichiesta==Comunicazioni::PartecipaServer){
			emit IWantToJoin(socketDescriptor);
		}
		else if(TipoRichiesta==Comunicazioni::CambiateInfo){
			incom >> dato1 >> stringa1 >> dato2; // ID, Nuovo Nome, Nuovo Colore
			emit CambiateInfo(dato1,stringa1,dato2);
		}
		else if(TipoRichiesta==Comunicazioni::SonoPronto){
			incom >> dato1; //ID di chi è pronto
			emit IsReady(dato1);
		}
		else if(TipoRichiesta==Comunicazioni::NonSonoPronto){
			incom >> dato1; //ID di chi è pronto
			emit IsNotReady(dato1);
		}
		else if(TipoRichiesta==Comunicazioni::IDRicevuto){
			emit RicevutoID(socketDescriptor);
		}*/
		nextBlockSize = 0;
	}
}