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
	,IsWaiting(false)
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
	CardDeck deck;
	QPixmap images;
	qint32 Numbers;
	QList<int> intlist;
	forever {
		if (nextBlockSize == 0) {
			if (bytesAvailable() < sizeof(quint32))
				break;
			incom >> nextBlockSize;
		}
		if (bytesAvailable() < nextBlockSize)
			break;
		incom >> RequestType;

		intlist.clear();
///////////////////////////////////////////////////////////////////////////
		if(RequestType==Comunications::TransmissionType::ChatMessage){
			incom >> strings;
			emit ChatMessageRecieved(strings);
		}
		else if(RequestType==Comunications::TransmissionType::JoinRequest){
			incom >> strings;
			incom >> images;
			emit RequestJoin(SocketID,strings,images);
		}
		else if(RequestType==Comunications::TransmissionType::ReadyToPlay){
			incom >> booleans;
			incom >> deck;
			emit DeckSetUp(SocketID,deck);
			emit ReadyToPlay(SocketID,booleans);
		}
		else if(RequestType==Comunications::TransmissionType::Mulligan){
			emit Mulligan(SocketID);
		}
		else if(RequestType==Comunications::TransmissionType::HandAccepted){
			emit HandAccepted(SocketID);
		}
		else if(RequestType==Comunications::TransmissionType::TimerFinished){
			emit TimerFinished(SocketID);
		}
		else if(RequestType==Comunications::TransmissionType::TimerStopped){
			emit TimerStopped(SocketID);
		}
		else if(RequestType==Comunications::TransmissionType::TimerResumed){
			emit TimerResumed(SocketID);
		}
		else if(RequestType==Comunications::TransmissionType::WantToPlayCard){
			incom >> Numbers;
			incom >> intlist;
			emit WantPlayCard(SocketID,Numbers,intlist);
		}
		else if(RequestType==Comunications::TransmissionType::ContinueToNextPhase){
			emit ContinueToNextPhase(SocketID);
		}
		else if(RequestType==Comunications::TransmissionType::AttackingCards){
			incom >> intlist;
			emit AttackingCards(SocketID,intlist);
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
void LanServerSocket::SendYourNameColor(int SocID,QString nam,QColor col){
	if (SocketID!=SocID) return;
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::YourNameAndColor) << nam << col;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendInvalidDeck(int SocID){
	if (SocketID!=SocID) return;
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::InvalidDeck);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendGameHasStarted(){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::GameStarted);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendPlayOrder(QList<int> Order){
	if (Order.contains(SocketID))
		Order.replace(Order.indexOf(SocketID),-1);
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::PlayersOrder) << Order;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendPlayerHand(int SocID,QList<int> hand){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	if (SocID==SocketID){
		out << quint32(0) << quint32(Comunications::TransmissionType::YourHand) << hand;
	}
	else{
		out << quint32(0) << quint32(Comunications::TransmissionType::OthersHand) << qint32(SocID) << qint32(hand.size());
	}
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendPlayerLibrary(int SocID,int libr){
	if (SocID==SocketID) SocID=-1;
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::PlayerLibrary) << qint32(SocID) << qint32(libr);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendPlayersNameAvatar(QMap<int,QString> nam,QMap<int,QPixmap> avt){
	if (!(nam.keys().contains(SocketID) && avt.keys().contains(SocketID))) return;
	nam.insert(-1,nam.value(SocketID));
	nam.erase(nam.find(SocketID));
	avt.insert(-1,avt.value(SocketID));
	avt.erase(avt.find(SocketID));
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::PlayesNameAndAvatar) << nam << avt;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendWaitingFor(int socID,QString msg){
	if (socID!=SocketID) return;
	IsWaiting=true;
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::WaitingFor) << msg;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendStopWaitingFor(){
	if(!IsWaiting) return;
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::StopWaitingFor);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendCurrentPhaseChanged(int newphase){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::PhaseChanged) << qint32(newphase);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendCardsToUntap(QList<int> crds){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::UntapCards) << crds;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendCardDrawn(int socID,int crd){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	if (socID==SocketID)
		out << quint32(0) << quint32(Comunications::TransmissionType::YouDrawnCard) << crd;
	else
		out << quint32(0) << quint32(Comunications::TransmissionType::OtherDrawnCard) << qint32(socID);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendStopTimers(){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::StopTimers);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendStopTurnTimer(){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::StopTurnTimer);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendResumeTurnTimer(){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::ResumeTurnTimer);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendResumeStackTimer(){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::ResumeStackTimer);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendEffectAddedToStack(quint32 crd,const EffectData& eff){
	EffectData EffToSend(eff);
	if (EffToSend.GetSelectedTargets().contains(EffectsConstants::Targets::Player)){
		EffToSend.ReplaceSpecificSelectedTarget(EffectsConstants::Targets::Player,SocketID,-1);
	}
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::EffectAddedStack) << crd << EffToSend;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendEffectResolved(){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::EffectResolved);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendPlayableCards(int socID,QList<int> IDs){
	if (socID!=SocketID) return;
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::PlayableCards) << IDs;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendPlayedCard(int who,int crd){
	if (who==SocketID) who=-1;
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::PlayedCard) << qint32(who) << qint32(crd);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendRemoveFromHand(int who,int crdID){
	if (who==SocketID) who=-1;
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::RemoveCardFromHand) << qint32(who) << qint32(crdID);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendPermanentResolved(int who,int crd){
	if (who==SocketID) who=-1;
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::PermanentResolved) << qint32(who) << qint32(crd);
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendAllCards(QList<CardData> crds){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::AllCards) << crds;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendCardsToTap(QList<int> crdIDs){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::TapCards) << crdIDs;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendAttackAbleCards(int who,QList<int> crdIDs){
	if(who!=SocketID) return;
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::CardsThatCanAttack) << crdIDs;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}
void LanServerSocket::SendAttackingCards(const QList<int>& crdIDs){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << quint32(0) << quint32(Comunications::TransmissionType::AttackingCards) << crdIDs;
	out.device()->seek(0);
	out << quint32(block.size() - sizeof(quint32));
	write(block);
}