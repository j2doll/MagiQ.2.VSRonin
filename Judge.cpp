#include "Judge.h"
#include "EffectsConstants.h"
#include <QTime>
Judge::Judge(QObject* parent)
	:QObject(parent)
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
void Judge::SomebodyDisconnected(int a){
	QMap<int,MagiQPlayer*>::iterator playpoint=PlayersList.find(a);
	if (playpoint==PlayersList.end()) return;
	QString Temp=playpoint.value()->GetPlayerName();
	PlayersList.erase(playpoint);
	SendServerInfos();
	SendLeftTheGame(Temp);
	if (EverybodyReady() && !GameStarted) StartMatch();
}
void Judge::IsReadyToPlay(int a,bool ready){
	if (PlayersList.find(a)==PlayersList.end()) return;
	PlayersList[a]->SetReadyToStartMatch(ready);
	if (EverybodyReady() && !GameStarted)
		StartMatch();	
}
bool Judge::EverybodyReady() const{
	if (PlayersList.size()<MinPlayer) return false;
	foreach(MagiQPlayer* PlayPoint,PlayersList){
		if (!PlayPoint->GetReadyToStartMatch()) return false;
	}
	return true;
}

bool Judge::EverybodyAcceptedHand() const{
	foreach(MagiQPlayer* PlayPoint,PlayersList){
		if (!PlayPoint->GetHasAcceptedHand()) return false;
	}
	return true;
}
bool Judge::EverybodyFinishedTimer() const{
	foreach(MagiQPlayer* PlayPoint,PlayersList){
		if (!PlayPoint->GetHasFinishedTimer()) return false;
	}
	return true;
}
void Judge::DeckSetUp(int socID,CardDeck deck){
	if (PlayersList.find(socID)==PlayersList.end()) return;
	if (deck.Legality().contains(DecksFormat)){
		PlayersList[socID]->SetLibrary(deck);
		for (QList<CardData>::iterator i=PlayersList[socID]->GetLibrary().begin();i!=PlayersList[socID]->GetLibrary().end();i++){
			for (int j=0;j<i->GetEffects().size();j++) i->SetEffectID(j,++EffectsIDCounter);
			i->SetCardID(++CardIDCounter);
			i->SetOwner(PlayersList[socID]);
		}
		for (QList<CardData>::iterator i=PlayersList[socID]->GetSideboard().begin();i!=PlayersList[socID]->GetSideboard().end();i++){
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
void Judge::StartMatch(){
	GameStarted=true;
	//Decide who plays first
	QMap<int,int> Randomizer;
	QList<CardData> AllCardsToSend;
	foreach(int socID,PlayersList.keys()){
		AllCardsToSend.append(PlayersList[socID]->GetLibrary());
		AllCardsToSend.append(PlayersList[socID]->GetSideboard());
	}
	emit AllCards(AllCardsToSend);
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
	QList<int> IDsInHand;
	for(QMap<int,MagiQPlayer*>::iterator index=PlayersList.begin();index!=PlayersList.end();index++){
		index.value()->ShuffleLibrary();
		for(int i=0;i<7;i++) index.value()->DrawCard();
		emit PlayerLibrary(index.key(),index.value()->GetLibrary().size());
		IDsInHand.clear();
		foreach(const CardData& crd,index.value()->GetHand()) IDsInHand.append(crd.GetCardID());
		emit PlayerHand(index.key(),IDsInHand);
	}
	emit GameHasStarted();
}
void Judge::DoMulligan(int socID){
	MagiQPlayer* TempPoint=PlayersList.value(socID,NULL);
	if (!TempPoint) return;
	int HandSize=TempPoint->GetHand().size();
	--HandSize;
	TempPoint->HandToBottomLibrary();
	TempPoint->ShuffleLibrary();
	for(int i=0;i<HandSize;i++) TempPoint->DrawCard();
	emit PlayerLibrary(socID,TempPoint->GetLibrary().size());
	QList<int> IDsInHand;
	foreach(const CardData& crd,TempPoint->GetHand()) IDsInHand.append(crd.GetCardID());
	emit PlayerHand(socID,IDsInHand);
}
void Judge::AcceptedHand(int socID){
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
void Judge::SetCurrentPhase(int a){
	if (a<Constants::Phases::Untap || a>Constants::Phases::TurnEnd) return;
	CurrentPhase=a;
	emit CurrentPhaseChanged(CurrentPhase);
}
void Judge::NextTurn(){
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
void Judge::UpkeepStep(){
	PhaseTimerRunning=true;
	SetCurrentPhase(Constants::Phases::Upkeep);
	NextPhase=Constants::Phases::Draw;
}
void Judge::DrawStep(){
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
void Judge::MainStep(){
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
void Judge::TimerFinished(int SocID){
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
void Judge::TimerStopped(int SocID){
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
void Judge::ResumeTimer(int SocID){
	if (WhoStoppedTheTimer!=SocID || WhoStoppedTheTimer==-1 || TimerTypeStopped==NoneT) return;
	PhaseTimerRunning=TimerTypeStopped==PhaseT || TimerTypeStopped==BothT;
	StackTimerRunning=TimerTypeStopped==StackT || TimerTypeStopped==BothT;
	if (WhoStoppedTheTimer!=PlayersOrder.value(TurnNumber%PlayersOrder.size())) emit ResumeTurnTimer();
	WhoStoppedTheTimer=-1;
	TimerTypeStopped=NoneT;
	if(PhaseTimerRunning) SetCurrentPhase(CurrentPhase);
	if(StackTimerRunning) emit ResumeStackTimer();
}
QMap<int,int> Judge::ManaAvailable(int PlayerCode) const{
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
void Judge::WantsToPlayCard(int who,int CrdID){
	CardData TheCard=PlayersList[who]->RemoveFromHand(CrdID);
	if (TheCard.GetCardID()==0) return;
	if (!CanPlayCard(TheCard,who)) return; //Double check, erase to speed up
	TheCard.SetController(PlayersList.value(who,NULL));
	PlayersList[who]->AddCardInStack(TheCard);
	if(!TheCard.GetCardType().contains(Constants::CardTypes::Land)){
		StackTimerRunning=true;
		emit RemoveFromHand(who,CrdID);
		AddToStack(PlayersList[who]->GetCardsInStack().last());
	}
	else{
		PlayersList[who]->SetCanPlayMana(false);
		emit RemoveFromHand(who,CrdID);
		ResolveCard(PlayersList[who]->GetCardsInStack().last());
	}	
	CheckPlayableCards();
}
bool Judge::CanPlayCard(const CardData& crd, int PlayerCode, const QMap<int,int>& ManaAvai){
	if (crd.GetCardType().contains(Constants::CardTypes::Land) && !crd.GetHasManaCost() && PlayersList.value(PlayerCode)->GetCanPlayMana()) return true;
	//TODO implement
	return false;
}
void Judge::CheckPlayableCards(){
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
void Judge::AddToStack(EffectData* eff){
	foreach(EffectData* effp,EffectsStack){
		if (effp->GetEffectID()==eff->GetEffectID()) return;
	}
	EffectsStack.push(eff);
	EffectInStack.push(true);
	StackTimerRunning=true;
	CardData* tmpcrd=EffectsStack.top()->GetCardAttached();
	emit EffectAddedToStack(tmpcrd ? tmpcrd->GetCardID():0,*(EffectsStack.top()));
}
void Judge::AddToStack(CardData crd){
	foreach(const CardData& crdp,CardsStack){
		if (crdp.GetCardID()==crd.GetCardID()) return;
	}
	CardsStack.push(crd);
	EffectInStack.push(false);
	StackTimerRunning=true;
	emit PlayedCard(crd.GetController()->GetPlayerID(),crd.GetCardID());
}
void Judge::ResolveCard(CardData crd){
	//TODO Check if countered
	if((!crd.GetCardType().contains(Constants::CardTypes::Instant)) && (!crd.GetCardType().contains(Constants::CardTypes::Instant))){
		PlayersList[PlayersList.key(crd.GetController())]->AddControlledCard(crd);
		emit PermanentResolved(PlayersList.key(crd.GetController()),crd.GetCardID());
	}
	CheckPlayableCards();
}
void Judge::ResolveEffect(EffectData* eff){
	if (!eff) return;
	emit EffectResolved();
	//TODO Check if countered
	switch(eff->GetEffectBody()){
	case EffectsConstants::Effects::DrawCards:
		const QList<int> TmpList=eff->GetSelectedTargets().values(EffectsConstants::Targets::Player);
		for(int drw=0;drw<TmpList.size();drw++){
			for (int j=0;j<eff->GetVariableValues().value(drw,0);j++){
				int CardToSend=PlayersList.value(TmpList.at(drw))->DrawCard().GetCardID();
				emit CardDrawn(TmpList.at(drw),CardToSend);
			}
		}
		break;
	}
	if (eff->GetEffectID()==0) delete eff;
}
void Judge::IncomingJoinRequest(int a, QString nam, QPixmap avat){
	if (GameStarted) return;
	if (GetNumPlayers()>=MaxPlayer)	return;
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
	emit YourNameColor(a,adjName,PlayPoint->GetPlayerColor());
	SendServerInfos();
}