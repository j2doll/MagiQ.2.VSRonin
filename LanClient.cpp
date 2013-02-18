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
	,MyName("Player")
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
	if(tcpSocket->state()==QAbstractSocket::ConnectedState) return;
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
	QColor colors;
	qint32 int1,int2,int3,int4,int5;
	quint32 UnsInt1;
	QList<int> intlists;
	QList<CardData> cardlists;
	QMap<int,QString> stringIntMap;
	QMap<int,QPixmap> pixmapIntMap;
	CardData card;
	EffectData effect;
	forever {
		if (nextBlockSize == 0) {
			if (tcpSocket->bytesAvailable() < sizeof(quint32))
				break;
			incom >> nextBlockSize;
		}
		if (tcpSocket->bytesAvailable() < nextBlockSize)
			break;
		incom >> RequestType;
		stringIntMap.clear();
		pixmapIntMap.clear();
		cardlists.clear();
		intlists.clear();
////////////////////////////////////////////////////////////////////////////
		if(RequestType==Comunications::TransmissionType::ChatMessage){
			incom >> strings;
			emit ChatMessageRecieved(strings);
		}
		else if(RequestType==Comunications::TransmissionType::SeverInformations){
			incom >> strings
				>>int1
				>>int2
				>>int3
				>>int4
				>>int5;
			emit ServerInfos(strings,int1,int2,int3,int4,int5);
		}
		else if(RequestType==Comunications::TransmissionType::ServerFull){
			emit ServerIsFull();
		}
		else if(RequestType==Comunications::TransmissionType::PlayerJoins){
			incom >> strings;
			emit UserJoined(strings);
		}
		else if(RequestType==Comunications::TransmissionType::PlayerLeave){
			incom >> strings;
			emit UserLeft(strings);
		}
		else if(RequestType==Comunications::TransmissionType::YourNameAndColor){
			incom >> strings;
			incom >> colors;
			MyName=strings;
			emit MyNameAndColor(strings,colors);
		}
		else if(RequestType==Comunications::TransmissionType::InvalidDeck){
			emit InvalidDeck();
		}
		else if(RequestType==Comunications::TransmissionType::GameStarted){
			emit GameHasStarted();
		}
		else if(RequestType==Comunications::TransmissionType::PlayersOrder){
			incom >> intlists;
			emit PlayOrder(intlists);
		}
		else if(RequestType==Comunications::TransmissionType::PlayesNameAndAvatar){
			incom >> stringIntMap;
			incom >> pixmapIntMap;
			emit PlayersNameAvatar(stringIntMap,pixmapIntMap);
		}
		else if(RequestType==Comunications::TransmissionType::AllCards){
			incom >> cardlists;
			emit AllCards(cardlists);
		}
		else if(RequestType==Comunications::TransmissionType::YourHand){
			incom >> intlists;
			emit MyHand(intlists);
		}
		else if(RequestType==Comunications::TransmissionType::PlayerLibrary){
			incom >> int1;
			incom >> int2;
			emit PlayerLibrary(int1,int2);
		}
		else if(RequestType==Comunications::TransmissionType::OthersHand){
			incom >> int1 >> int2;
			emit OtherHand(int1,int2);
		}
		else if(RequestType==Comunications::TransmissionType::WaitingFor){
			incom >> strings;
			emit WaitingFor(strings);
		}
		else if(RequestType==Comunications::TransmissionType::StopWaitingFor){
			emit StopWaitingFor();
		}
		else if(RequestType==Comunications::TransmissionType::PhaseChanged){
			incom >> int1;
			emit CurrentPhaseChanged(int1);
		}
		else if(RequestType==Comunications::TransmissionType::UntapCards){
			incom >> intlists;
			emit CardsToUntap(intlists);
		}
		else if(RequestType==Comunications::TransmissionType::YouDrawnCard){
			incom >> int1;
			emit CardDrawn(int1);
		}
		else if(RequestType==Comunications::TransmissionType::OtherDrawnCard){
			incom >> int1;
			emit OtherDrawn(int1);
		}
		else if(RequestType==Comunications::TransmissionType::StopTimers){
			emit StopTimer();
		}
		else if(RequestType==Comunications::TransmissionType::StopTurnTimer){
			emit StopTurnTimer();
		}
		else if(RequestType==Comunications::TransmissionType::ResumeTurnTimer){
			emit ResumeTurnTimer();
		}
		else if(RequestType==Comunications::TransmissionType::ResumeStackTimer){
			emit ResumeStackTimer();
		}
		else if(RequestType==Comunications::TransmissionType::EffectAddedStack){
			incom >> UnsInt1;
			incom >> effect;
			emit EffectAddedToStack(effect,UnsInt1);
		}
		else if(RequestType==Comunications::TransmissionType::EffectResolved){
			emit EffectResolved();
		}
		else if(RequestType==Comunications::TransmissionType::PlayableCards){
			incom >> intlists;
			emit PlayableCards(intlists);
		}
		else if(RequestType==Comunications::TransmissionType::PlayedCard){
			incom >> int1;
			incom >> int2;
			emit PlayedCard(int2,int1);
		}
		else if(RequestType==Comunications::TransmissionType::RemoveCardFromHand){
			incom >> int1;
			incom >> int2;
			emit RemoveFromHand(int1,int2);
		}
		else if(RequestType==Comunications::TransmissionType::PermanentResolved){
			incom >> int1;
			incom >> int2;
			emit PermanentResolved(int1,int2);
		}
		else if(RequestType==Comunications::TransmissionType::TapCards){
			incom >> intlists;
			emit CardsToTap(intlists);
		}
		else if(RequestType==Comunications::TransmissionType::CardsThatCanAttack){
			incom >> intlists;
			emit AttackAbleCards(intlists);
		}
		else if(RequestType==Comunications::TransmissionType::AttackingCards){
			incom >> intlists;
			emit AttackingCards(intlists);
		}
////////////////////////////////////////////////////////////////////////////

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
	out << quint32(0)
		<< quint32(Comunications::TransmissionType::JoinRequest)
		<< MyName
		<< MyAvatar
		;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	tcpSocket->write(block);
}
void LanClient::SendReady(){
	IsReady=!IsReady;
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0)
		<< quint32(Comunications::TransmissionType::ReadyToPlay)
		<< IsReady
		<< MyDeck
		;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	tcpSocket->write(block);
}
void LanClient::SendMulligan(){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::Mulligan);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	tcpSocket->write(block);
}
void LanClient::SendHandAccepted(){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::HandAccepted);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	tcpSocket->write(block);
}
void LanClient::SendFinishedTimer(){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::TimerFinished);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	tcpSocket->write(block);
}
void LanClient::SendStoppedTimer(){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::TimerStopped);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	tcpSocket->write(block);
}
void LanClient::SendResumeTimer(){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::TimerResumed);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	tcpSocket->write(block);
}
void LanClient::SendWantToPlayCard(int CardID,QList<int> PayedWithIDs){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::WantToPlayCard) << qint32(CardID) << PayedWithIDs;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	tcpSocket->write(block);
}
void LanClient::SendContinueToNextPhase(){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::ContinueToNextPhase);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	tcpSocket->write(block);
}
void LanClient::SendAttackingCards(const QList<int>& crdIDs){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::AttackingCards) << crdIDs;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	tcpSocket->write(block);
}