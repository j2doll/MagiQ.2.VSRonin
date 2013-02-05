#include "LanServer.h"
#include "EffectsConstants.h"
#include <QTime>
LanServer::LanServer(QObject* parent)
	:QTcpServer(parent)
	,PortToListen(Comunications::DefaultTCPport)
	,GameMode(Constants::GameMode::FreeForAll)
	,DecksFormat(Constants::Legality::Any)
	,MinPlayer(2)
	,MaxPlayer(4)
	,ServerName("MagiQ Server")
	,GameStarted(false)
	,TurnNumber(-1)
	,CardIDCounter(0)
	,EffectsIDCounter(0)
	,PhaseTimeLimit(3000)
	,ResponseTimeLimit(5000)
	,TurnTimeLimit(300000)
	,Precombat(true)
	,WhoStoppedTheTimer(-1)
	,PhaseTimerRunning(false)
	,StackTimerRunning(false)
	,TimerTypeStopped(NoneT)
{
#ifdef _DEBUG
	qsrand(2);
#endif
#ifndef _DEBUG
	qsrand(QTime::currentTime().msec());
#endif
}
void LanServer::StartListening(){
	if (isListening()) return;
	if (!listen(QHostAddress::Any, PortToListen))
		emit CantBindPort();
}
void LanServer::incomingConnection(int socketDescriptor){
	if (GameStarted) return;
	if (clients.value(socketDescriptor,NULL))
		clients[socketDescriptor]->deleteLater();
	clients[socketDescriptor]=new LanServerThread(socketDescriptor,this);
	LanServerThread* TempPoint=clients[socketDescriptor];
	connect(this,SIGNAL(ServerInfos(QString,int,int,int,int,int)),TempPoint,SIGNAL(ServerInfos(QString,int,int,int,int,int)));
	connect(TempPoint,SIGNAL(RequestJoin(int,QString,QPixmap)),this,SLOT(IncomingJoinRequest(int,QString,QPixmap)));
	connect(this,SIGNAL(ServerIsFull(int)),TempPoint,SIGNAL(ServerIsFull(int)));
	connect(TempPoint,SIGNAL(ReachedEnd(int)),this,SLOT(EraseThread(int)));
	connect(this,SIGNAL(GameHasStarted()),TempPoint,SIGNAL(GameHasStarted()));
	TempPoint->start();
	SendServerInfos();
}
void LanServer::EraseThread(int a){
	QMap<int,MagiQPlayer*>::iterator playpoint=PlayersList.find(a);
	if (playpoint!=PlayersList.end()){
		QString Temp=playpoint.value()->GetPlayerName();
		PlayersList.erase(playpoint);
		SendServerInfos();
		SendLeftTheGame(Temp);
	}
	QMap<int,LanServerThread*>::iterator index=clients.find(a);
	if (index!=clients.end()){
		index.value()->deleteLater();
		clients.erase(index);
	}
	if (EverybodyReady() && !GameStarted) StartMatch();
}
void LanServer::IsReadyToPlay(int a,bool ready){
	if (PlayersList.find(a)==PlayersList.end()) return;
	PlayersList[a]->SetReadyToStartMatch(ready);
	if (EverybodyReady() && !GameStarted)
		StartMatch();	
}
bool LanServer::EverybodyReady() const{
	if (PlayersList.size()<MinPlayer) return false;
	foreach(MagiQPlayer* PlayPoint,PlayersList){
		if (!PlayPoint->GetReadyToStartMatch()) return false;
	}
	return true;
}
bool LanServer::EverybodyAcceptedHand() const{
	foreach(MagiQPlayer* PlayPoint,PlayersList){
		if (!PlayPoint->GetHasAcceptedHand()) return false;
	}
	return true;
}
bool LanServer::EverybodyFinishedTimer() const{
	foreach(MagiQPlayer* PlayPoint,PlayersList){
		if (!PlayPoint->GetHasFinishedTimer()) return false;
	}
	return true;
}
void LanServer::DeckSetUp(int socID,CardDeck deck){
	if (PlayersList.find(socID)==PlayersList.end()) return;
	if (deck.Legality().contains(DecksFormat)){
		PlayersList[socID]->SetLibrary(deck);
		for (QList<CardData>::iterator i=PlayersList[socID]->GetLibrary().begin();i!=PlayersList[socID]->GetLibrary().end();i++){
			for (int j=0;j<i->GetEffects().size();j++) i->SetEffectID(j,++EffectsIDCounter);
			i->SetCardID(++CardIDCounter);
			i->SetOwner(PlayersList[socID]);
		}
	}
	else{
		emit InvalidDeck(socID);
		PlayersList[socID]->SetReadyToStartMatch(false);
	}
}
void LanServer::StartMatch(){
	GameStarted=true;
	//Decide who plays first
	QMap<int,int> Randomizer;
	foreach(int socID,PlayersList.keys())
		Randomizer.insert(qrand()%100,socID);
	PlayersOrder=Randomizer.values();
	emit PlayOrder(PlayersOrder);
	QMap<int,QString> PlayerNames;
	QMap<int,QPixmap> PlayerAvatars;
	for (QMap<int,MagiQPlayer*>::const_iterator ite=PlayersList.constBegin();ite!=PlayersList.constEnd();ite++){
		PlayerNames.insert(ite.key(),ite.value()->GetPlayerName());
		PlayerAvatars.insert(ite.key(),ite.value()->GetAvatar());
	}
	emit PlayersNameAvatar(PlayerNames,PlayerAvatars);
	//Shuffle the libraries, draw 7 card for each player and send them the result 
	for(QMap<int,MagiQPlayer*>::iterator index=PlayersList.begin();index!=PlayersList.end();index++){
		index.value()->ShuffleLibrary();
		for(int i=0;i<7;i++) index.value()->DrawCard();
		emit PlayerLibrary(index.key(),index.value()->GetLibrary());
		emit PlayerHand(index.key(),index.value()->GetHand());
	}
	emit GameHasStarted();
}
void LanServer::DoMulligan(int socID){
	MagiQPlayer* TempPoint=PlayersList.value(socID,NULL);
	if (!TempPoint) return;
	int HandSize=TempPoint->GetHand().size();
	--HandSize;
	TempPoint->HandToBottomLibrary();
	TempPoint->ShuffleLibrary();
	for(int i=0;i<HandSize;i++) TempPoint->DrawCard();
	emit PlayerLibrary(socID,TempPoint->GetLibrary());
	emit PlayerHand(socID,TempPoint->GetHand());
}
void LanServer::AcceptedHand(int socID){
	MagiQPlayer* TempPoint=PlayersList.value(socID,NULL);
	if (!TempPoint) return;
	TempPoint->SetHasAcceptedHand(true);
	if (EverybodyAcceptedHand()){
		emit StopWaitingFor();
		return NextTurn();
	}
	else{
		emit WaitingFor(socID,tr("Waiting for other players to decide whether to take a mulligan"));
	}
}
void LanServer::SetCurrentPhase(int a){
	if (a<Constants::Phases::Untap || a>Constants::Phases::TurnEnd) return;
	CurrentPhase=a;
	emit CurrentPhaseChanged(CurrentPhase);
}
void LanServer::NextTurn(){
	++TurnNumber;
	Precombat=true;
	foreach(MagiQPlayer* plr,PlayersList)
		plr->SetHasFinishedTimer(false);
	PhaseTimerRunning=true;
	QList<int> UntappedCards;
	int WhosTurn=PlayersOrder.value(TurnNumber%PlayersOrder.size());
	SetCurrentPhase(Constants::Phases::Untap);
	NextPhase=Constants::Phases::Upkeep;
	for(QList<CardData>::const_iterator i=PlayersList.value(WhosTurn)->GetControlledCards().constBegin();i!=PlayersList.value(WhosTurn)->GetControlledCards().constEnd();i++){
		if (i->GetTapped()) UntappedCards.append(i->GetCardID());
		i->SetTapped(false);
	}
}
void LanServer::UpkeepStep(){
	PhaseTimerRunning=true;
	SetCurrentPhase(Constants::Phases::Upkeep);
	NextPhase=Constants::Phases::Draw;
}
void LanServer::DrawStep(){
	PhaseTimerRunning=true;
	SetCurrentPhase(Constants::Phases::Draw);
	NextPhase=Constants::Phases::PreCombatMain;
	int WhosTurn=PlayersOrder.value(TurnNumber%PlayersOrder.size());
	//if (TurnNumber!=0){
		EffectData* DrawEffect=new EffectData;
		DrawEffect->SetSelectedTargets(EffectsConstants::Targets::Player,PlayersList.value(WhosTurn)->GetPlayerID());
		DrawEffect->SetVariableValues(1);
		DrawEffect->SetEffectBody(EffectsConstants::Effects::DrawCards);
		DrawEffect->SetEffectText(tr("%1 draws a card").arg(PlayersList.value(WhosTurn)->GetPlayerName()));
		AddToStack(DrawEffect);
	//}
}
void LanServer::MainStep(){
	PhaseTimerRunning=false;
	if (Precombat){
		SetCurrentPhase(Constants::Phases::PreCombatMain);
		NextPhase=Constants::Phases::BeginOfCombat;
	}
	else{
		SetCurrentPhase(Constants::Phases::PostCombatMain);
		NextPhase=Constants::Phases::EndOfTurn;
	}
	CheckPlayableCards();
}
void LanServer::TimerFinished(int SocID){
	MagiQPlayer* TempPoint=PlayersList.value(SocID,NULL);
	if (!TempPoint || (!PhaseTimerRunning && !StackTimerRunning)) return;
	TempPoint->SetHasFinishedTimer(true);
	if (EverybodyFinishedTimer()){
		foreach(MagiQPlayer* plr,PlayersList)
			plr->SetHasFinishedTimer(false);
		if(StackTimerRunning){
			if (EffectInStack.pop())
				ResolveEffect(EffectsStack.pop());
			else
				ResolveCard(CardsStack.pop());
			if (!(EffectsStack.isEmpty() && CardsStack.isEmpty())){
				StackTimerRunning=true;
				emit ResumeStackTimer();
			}
			else StackTimerRunning=false;
		}
		else if (PhaseTimerRunning){
			PhaseTimerRunning=false;
			switch(NextPhase){
			case Constants::Phases::Upkeep: return UpkeepStep();
			case Constants::Phases::Draw: return DrawStep();
			case Constants::Phases::PreCombatMain:
			case Constants::Phases::PostCombatMain:	return MainStep();
			case Constants::Phases::Untap:
			default: return NextTurn();
			}
		}
	}
}
void LanServer::TimerStopped(int SocID){
	if (!PlayersOrder.contains(SocID) || WhoStoppedTheTimer!=-1) return;
	if (PhaseTimerRunning && StackTimerRunning) TimerTypeStopped=BothT;
	else if(PhaseTimerRunning) TimerTypeStopped=PhaseT;
	else if(StackTimerRunning) TimerTypeStopped=StackT;
	else {TimerTypeStopped=NoneT; return;}
	emit StopTimers();
	if (SocID!=PlayersOrder.value(TurnNumber%PlayersOrder.size()))
		emit StopTurnTimer();
	PhaseTimerRunning=false;
	StackTimerRunning=false;
	WhoStoppedTheTimer=SocID;
	foreach(MagiQPlayer* plr,PlayersList)
		plr->SetHasFinishedTimer(false);
}
void LanServer::ResumeTimer(int SocID){
	if (WhoStoppedTheTimer!=SocID || WhoStoppedTheTimer==-1 || TimerTypeStopped==NoneT) return;
	PhaseTimerRunning=TimerTypeStopped==PhaseT || TimerTypeStopped==BothT;
	StackTimerRunning=TimerTypeStopped==StackT || TimerTypeStopped==BothT;
	if (WhoStoppedTheTimer!=PlayersOrder.value(TurnNumber%PlayersOrder.size())) emit ResumeTurnTimer();
	WhoStoppedTheTimer=-1;
	TimerTypeStopped=NoneT;
	if(PhaseTimerRunning) SetCurrentPhase(CurrentPhase);
	if(StackTimerRunning) emit ResumeStackTimer();
}
QMap<int,int> LanServer::ManaAvailable(int PlayerCode) const{
	if(!PlayersList.contains(PlayerCode)) return QMap<int,int>();
	QMap<int,int> Result(PlayersList.value(PlayerCode)->GetManaPool());
	for(QList<CardData>::const_iterator crd=PlayersList.value(PlayerCode)->GetControlledCards().constBegin();crd!=PlayersList.value(PlayerCode)->GetControlledCards().constEnd();crd++){
		if(crd->GetSummoningSickness()) continue;
		for(QList<EffectData>::const_iterator eff=crd->GetEffects().constBegin();eff!=crd->GetEffects().constEnd();eff++){
			if(!eff->GetManaEffect()) continue;
			switch(eff->GetEffectBody()){
			case EffectsConstants::Effects::AddWToManaPool: Result[Constants::ManaCosts::W]+=eff->GetVariableValues().at(0); break;
			case EffectsConstants::Effects::AddUToManaPool: Result[Constants::ManaCosts::U]+=eff->GetVariableValues().at(0); break;
			case EffectsConstants::Effects::AddBToManaPool: Result[Constants::ManaCosts::B]+=eff->GetVariableValues().at(0); break;
			case EffectsConstants::Effects::AddRToManaPool: Result[Constants::ManaCosts::R]+=eff->GetVariableValues().at(0); break;
			case EffectsConstants::Effects::AddGToManaPool: Result[Constants::ManaCosts::G]+=eff->GetVariableValues().at(0); break;
				//TODO Add the others
			}
		}
	}
	return Result;
}
void LanServer::WantsToPlayCard(int who,int CrdID){
	CardData TheCard=PlayersList[who]->RemoveFromHand(CrdID);
	if (TheCard.GetCardID()==0) return;
	if (!CanPlayCard(TheCard,who)) return; //Double check, erase to speed up
	TheCard.SetController(PlayersList.value(who,NULL));
	PlayersList[who]->AddCardInStack(TheCard);
	if(!TheCard.GetCardType().contains(Constants::CardTypes::Land)){
		StackTimerRunning=true;
		AddToStack(PlayersList[who]->GetCardsInStack().last());
	}
	else{
		PlayersList[who]->SetCanPlayMana(false);
		ResolveCard(PlayersList[who]->GetCardsInStack().last());
	}	
	CheckPlayableCards();
}
bool LanServer::CanPlayCard(const CardData& crd, int PlayerCode, const QMap<int,int>& ManaAvai){
	if (crd.GetCardType().contains(Constants::CardTypes::Land) && !crd.GetHasManaCost() && PlayersList.value(PlayerCode)->GetCanPlayMana()) return true;
	//TODO implement
	return false;
}
void LanServer::CheckPlayableCards(){
	QList<int> PlayableCardsIDs;
	QMap<int,int> AvaiMana;
	foreach(int plid,PlayersOrder){
		PlayableCardsIDs.clear();
		AvaiMana.clear();
		AvaiMana=ManaAvailable(plid);
		if(plid==PlayersOrder.value(TurnNumber%PlayersOrder.size())){ //Player whos current turn
			switch(CurrentPhase){
			case Constants::Phases::TurnEnd: break;
			case Constants::Phases::PreCombatMain:
			case Constants::Phases::PostCombatMain: 
				//Check Hand
				for(QList<CardData>::const_iterator crd=PlayersList.value(plid)->GetHand().constBegin();crd!=PlayersList.value(plid)->GetHand().constEnd();crd++){
					if(CanPlayCard(*crd,plid,AvaiMana)) PlayableCardsIDs.append(crd->GetCardID());
				}
				break;
			case Constants::Phases::DeclareAttackers:
			case Constants::Phases::DeclareBlockers:
				if(!(PhaseTimerRunning || TimerTypeStopped==PhaseT || TimerTypeStopped==BothT)) break;
			default:
				for(QList<CardData>::const_iterator crd=PlayersList.value(plid)->GetHand().constBegin();crd!=PlayersList.value(plid)->GetHand().constEnd();crd++){
					if (crd->GetCardType().contains(Constants::CardTypes::Instant)){
						//TODO Flash
						if(CanPlayCard(*crd,plid,AvaiMana)) PlayableCardsIDs.append(crd->GetCardID());
					}
				}
			}
		}
		emit PlayableCards(plid,PlayableCardsIDs);
	}
}
void LanServer::AddToStack(EffectData* eff){
	foreach(EffectData* effp,EffectsStack){
		if (effp->GetEffectID()==eff->GetEffectID()) return;
	}
	EffectsStack.push(eff);
	EffectInStack.push(true);
	StackTimerRunning=true;
	CardData* tmpcrd=EffectsStack.top()->GetCardAttached();
	emit EffectAddedToStack(tmpcrd ? tmpcrd->GetCardID():0,*(EffectsStack.top()));
}
void LanServer::AddToStack(CardData crd){
	foreach(const CardData& crdp,CardsStack){
		if (crdp.GetCardID()==crd.GetCardID()) return;
	}
	CardsStack.push(crd);
	EffectInStack.push(false);
	StackTimerRunning=true;
	emit PlayedCard(crd.GetController()->GetPlayerID(),crd);
}
void LanServer::ResolveCard(CardData crd){
	//TODO Check if countered
}
void LanServer::ResolveEffect(EffectData* eff){
	if (!eff) return;
	emit EffectResolved();
	//TODO Check if countered
	switch(eff->GetEffectBody()){
	case EffectsConstants::Effects::DrawCards:
		const QList<int> TmpList=eff->GetSelectedTargets().values(EffectsConstants::Targets::Player);
		for(int drw=0;drw<TmpList.size();drw++){
			for (int j=0;j<eff->GetVariableValues().value(drw,0);j++){
				CardData CardToSend=PlayersList.value(TmpList.at(drw))->DrawCard();
				emit CardDrawn(TmpList.at(drw),CardToSend);
			}
		}
		break;
	}
	if (eff->GetEffectID()==0) delete eff;
}
void LanServer::IncomingJoinRequest(int a, QString nam, QPixmap avat){
	if (GameStarted) return;
	if (!clients.value(a,NULL)) return;
	if (PlayersList.size()>=MaxPlayer){
		emit ServerIsFull(a);
		return;
	}
	PlayersList[a]=new MagiQPlayer(this);
	MagiQPlayer* PlayPoint=PlayersList[a];
	PlayPoint->SetPlayerID(a);
	bool Duplicate;
	int Counter=1;
	QString adjName(nam);
	do{
		Duplicate=false;
		for(QMap<int,MagiQPlayer*>::const_iterator i=PlayersList.constBegin();i!=PlayersList.constEnd() && !Duplicate;++i){
			if (i.value()->GetPlayerName()==adjName){
				Duplicate=true;
				adjName=nam+QString(" (%1)").arg(Counter++);
			}
		}
	}while(Duplicate);
	PlayPoint->SetPlayerName(adjName);
	PlayPoint->SetAvatar(avat);
	int TempNum=PlayersList.size();
	while (TempNum>8) TempNum-=8;
	PlayPoint->SetPlayerColor(Constants::PlayerColors[--TempNum]);
	emit Joined(adjName);
	LanServerThread* TempPoint=clients[a];
	connect(this,SIGNAL(ChatMessage(QString)),TempPoint,SIGNAL(SendMessage(QString)));
	connect(TempPoint,SIGNAL(ChatMessageRecieved(QString)),this,SIGNAL(ChatMessage(QString)));
	connect(TempPoint,SIGNAL(ReadyToPlay(int,bool)),this,SLOT(IsReadyToPlay(int,bool)));
	connect(TempPoint,SIGNAL(DeckSetUp(int,CardDeck)),this,SLOT(DeckSetUp(int,CardDeck)));
	connect(this,SIGNAL(Joined(QString)),TempPoint,SIGNAL(Joined(QString)));
	connect(this,SIGNAL(LeftTheGame(QString)),TempPoint,SIGNAL(LeftTheGame(QString)));
	connect(this,SIGNAL(YourNameColor(int,QString,QColor)),TempPoint,SIGNAL(YourNameColor(int,QString,QColor)));
	connect(this,SIGNAL(InvalidDeck(int)),TempPoint,SIGNAL(InvalidDeck(int)));
	connect(this,SIGNAL(PlayOrder(QList<int>)),TempPoint,SIGNAL(PlayOrder(QList<int>)));
	connect(this,SIGNAL(PlayersNameAvatar(QMap<int,QString>,QMap<int,QPixmap>)),TempPoint,SIGNAL(PlayersNameAvatar(QMap<int,QString>,QMap<int,QPixmap>)));
	connect(this,SIGNAL(PlayerHand(int,QList<CardData>)),TempPoint,SIGNAL(PlayerHand(int,QList<CardData>)));
	connect(this,SIGNAL(PlayerLibrary(int,QList<CardData>)),TempPoint,SIGNAL(PlayerLibrary(int,QList<CardData>)));
	connect(TempPoint,SIGNAL(Mulligan(int)),this,SLOT(DoMulligan(int)));
	connect(TempPoint,SIGNAL(HandAccepted(int)),this,SLOT(AcceptedHand(int)));
	connect(this,SIGNAL(WaitingFor(int,QString)),TempPoint,SIGNAL(WaitingFor(int,QString)));
	connect(this,SIGNAL(StopWaitingFor()),TempPoint,SIGNAL(StopWaitingFor()));
	connect(this,SIGNAL(CurrentPhaseChanged(int)),TempPoint,SIGNAL(CurrentPhaseChanged(int)));
	connect(this,SIGNAL(CardsToUntap(QList<int>)),TempPoint,SIGNAL(CardsToUntap(QList<int>)));
	connect(this,SIGNAL(CardDrawn(int,CardData)),TempPoint,SIGNAL(CardDrawn(int,CardData)));
	connect(TempPoint,SIGNAL(TimerFinished(int)),this,SLOT(TimerFinished(int)));
	connect(TempPoint,SIGNAL(TimerStopped(int)),this,SLOT(TimerStopped(int)));
	connect(TempPoint,SIGNAL(TimerResumed(int)),this,SLOT(ResumeTimer(int)));
	connect(this,SIGNAL(StopTimers()),TempPoint,SIGNAL(StopTimers()));
	connect(this,SIGNAL(StopTurnTimer()),TempPoint,SIGNAL(StopTurnTimer()));
	connect(this,SIGNAL(ResumeTurnTimer()),TempPoint,SIGNAL(ResumeTurnTimer()));
	connect(this,SIGNAL(ResumeStackTimer()),TempPoint,SIGNAL(ResumeStackTimer()));
	connect(this,SIGNAL(EffectAddedToStack(quint32,const EffectData&)),TempPoint,SIGNAL(EffectAddedToStack(quint32,const EffectData&)));
	connect(this,SIGNAL(EffectResolved()),TempPoint,SIGNAL(EffectResolved()));
	connect(this,SIGNAL(PlayableCards(int,QList<int>)),TempPoint,SIGNAL(PlayableCards(int,QList<int>)));
	connect(TempPoint,SIGNAL(WantPlayCard(int,int)),this,SLOT(WantsToPlayCard(int,int)));
	connect(this,SIGNAL(PlayedCard(int,const CardData&)),TempPoint,SIGNAL(PlayedCard(int,const CardData&)));
	emit YourNameColor(a,adjName,PlayPoint->GetPlayerColor());
	SendServerInfos();
}