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
	bool Logicals;
	QString strings;
	QColor colors;
	qint32 int1,int2,int3,int4,int5;
	quint32 UnsInt1;
	QList<int> intlists;
	QHash<int,int> intHash;
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
		intHash.clear();
////////////////////////////////////////////////////////////////////////////
		switch(RequestType){
		case Comunications::TransmissionType::ChatMessage:
			incom >> strings;
			emit ChatMessageRecieved(strings);
			break;
		case Comunications::TransmissionType::SeverInformations:
			incom >> strings
				>>int1
				>>int2
				>>int3
				>>int4
				>>int5;
			emit ServerInfos(strings,int1,int2,int3,int4,int5);
			break;
		case Comunications::TransmissionType::ServerFull:
			emit ServerIsFull();
			break;
		case Comunications::TransmissionType::PlayerJoins:
			incom >> strings;
			emit UserJoined(strings);
			break;
		case Comunications::TransmissionType::PlayerLeave:
			incom >> strings;
			emit UserLeft(strings);
			break;
		case Comunications::TransmissionType::YourNameAndColor:
			incom >> strings;
			incom >> colors;
			MyName=strings;
			emit MyNameAndColor(strings,colors);
			break;
		case Comunications::TransmissionType::InvalidDeck:
			emit InvalidDeck();
			break;
		case Comunications::TransmissionType::GameStarted:
			emit GameHasStarted();
			break;
		case Comunications::TransmissionType::PlayersOrder:
			incom >> intlists;
			emit PlayOrder(intlists);
			break;
		case Comunications::TransmissionType::PlayesNameAndAvatar:
			incom >> stringIntMap;
			incom >> pixmapIntMap;
			emit PlayersNameAvatar(stringIntMap,pixmapIntMap);
			break;
		case Comunications::TransmissionType::AllCards:
			incom >> cardlists;
			emit AllCards(cardlists);
			break;
		case Comunications::TransmissionType::YourHand:
			incom >> intlists;
			emit MyHand(intlists);
			break;
		case Comunications::TransmissionType::PlayerLibrary:
			incom >> int1;
			incom >> int2;
			emit PlayerLibrary(int1,int2);
			break;
		case Comunications::TransmissionType::OthersHand:
			incom >> int1 >> int2;
			emit OtherHand(int1,int2);
			break;
		case Comunications::TransmissionType::WaitingFor:
			incom >> strings;
			emit WaitingFor(strings);
			break;
		case Comunications::TransmissionType::StopWaitingFor:
			emit StopWaitingFor();
			break;
		case Comunications::TransmissionType::PhaseChanged:
			incom >> int1;
			emit CurrentPhaseChanged(int1);
			break;
		case Comunications::TransmissionType::UntapCards:
			incom >> intlists;
			emit CardsToUntap(intlists);
			break;
		case Comunications::TransmissionType::YouDrawnCard:
			incom >> int1;
			emit CardDrawn(int1);
			break;
		case Comunications::TransmissionType::OtherDrawnCard:
			incom >> int1;
			emit OtherDrawn(int1);
			break;
		case Comunications::TransmissionType::StopTimers:
			emit StopTimer();
			break;
		case Comunications::TransmissionType::StopTurnTimer:
			emit StopTurnTimer();
			break;
		case Comunications::TransmissionType::ResumeTurnTimer:
			emit ResumeTurnTimer();
			break;
		case Comunications::TransmissionType::ResumeStackTimer:
			emit ResumeStackTimer();
			break;
		case Comunications::TransmissionType::EffectAddedStack:
			incom >> UnsInt1;
			incom >> effect;
			emit EffectAddedToStack(effect,UnsInt1);
			break;
		case Comunications::TransmissionType::EffectResolved:
			emit EffectResolved();
			break;
		case Comunications::TransmissionType::PlayableCards:
			incom >> intlists;
			emit PlayableCards(intlists);
			break;
		case Comunications::TransmissionType::PlayedCard:
			incom >> int1;
			incom >> int2;
			emit PlayedCard(int2,int1);
			break;
		case Comunications::TransmissionType::RemoveCardFromHand:
			incom >> int1;
			incom >> int2;
			emit RemoveFromHand(int1,int2);
			break;
		case Comunications::TransmissionType::PermanentResolved:
			incom >> int1;
			incom >> int2;
			emit PermanentResolved(int1,int2);
			break;
		case Comunications::TransmissionType::TapCards:
			incom >> intlists;
			emit CardsToTap(intlists);
			break;
		case Comunications::TransmissionType::CardsThatCanAttack:
			incom >> intlists;
			emit AttackAbleCards(intlists);
			break;
		case Comunications::TransmissionType::AttackingCards:
			incom >> intHash;
			emit AttackingCards(intHash);
			break;
		case Comunications::TransmissionType::YourTurn:
			incom >> Logicals;
			emit IsMyTurn(Logicals);
			break;
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
void LanClient::SendAttackingCards(const QHash<int,int>& crdIDs){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::AttackingCards) << crdIDs;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	tcpSocket->write(block);
}