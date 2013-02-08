#include "BattleGround.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFont>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QTimer>
#include <QtAlgorithms>
#include <QApplication>
#include "MagiQPlayer.h"
#include "StyleSheets.h"
#include "CostantsDefinition.h"
#include "HandLayout.h"
#include "CardViewer.h"
#include "Card.h"
#include "PlayerInfoDisplayer.h"
#include "PhasesDisplayer.h"
#ifdef _DEBUG
	#include "SizeSliders.h"
#endif
BattleGround::BattleGround(QWidget* parent)
	:QWidget(parent)
	,ZoomedCard(NULL)
	,PhaseTimeLimit(3000)
	,ResponseTimeLimit(5000)
	,TurnTimeLimit(300000)
	,HandPrimarySorting(CardData::ByManaCost)
	,HandSecondarySorting(CardData::ByType)
{
	setMinimumSize(1024,768);
	Board=new QFrame(this);
	Board->setObjectName("Board");
	QHBoxLayout* MainLayout=new QHBoxLayout(this);
	MainLayout->setMargin(0);
	MainLayout->setSpacing(0);
	MainLayout->addWidget(Board);
	GenericCard=new Card(this);
	GenericCard->SetCovered(true);
	GenericCard->UpdateAspect();
	GenericCard->hide();

	QuestionFrame=new QFrame(this);
	QuestionFrame->setObjectName("QuestionFrame");
	QGridLayout* QuestionLayout=new QGridLayout(QuestionFrame);
	QuestionText=new QLabel(QuestionFrame);
	QuestionText->setScaledContents(true);
	QuestionText->setObjectName("QuestionText");
	QuestionText->setAlignment(Qt::AlignCenter);
	QuestionLayout->addWidget(QuestionText,0,0,1,2);
	QuestionButton1=new QPushButton(QuestionFrame);
	QuestionButton1->setObjectName("QuestionButton");
	QuestionLayout->addWidget(QuestionButton1,1,0);
	QuestionButton2=new QPushButton(QuestionFrame);
	QuestionButton2->setObjectName("QuestionButton");
	QuestionLayout->addWidget(QuestionButton2,1,1);

	PhaseTimer=new QTimer(this);
	PhaseTimer->setInterval(TimerUpdateIntervall);
	connect(PhaseTimer,SIGNAL(timeout()),this,SLOT(PhaseTimeUpdate()));
	ResponseTimer=new QTimer(this);
	ResponseTimer->setInterval(TimerUpdateIntervall);
	TurnTimer=new QTimer(this);
	TurnTimer->setInterval(TimerUpdateIntervall);
	connect(TurnTimer,SIGNAL(timeout()),this,SLOT(TurnTimeUpdate()));

	PhaseDisp=new PhasesDisplayer(this);
	PhaseDisp->SetTurnTimeLimit(TurnTimeLimit);
	PhaseDisp->SetPhaseTimeLimit(PhaseTimeLimit);
	PhaseDisp->SetPhase(CurrentPhase);
	PhaseDisp->hide();

	StackCardsFrame=new QFrame(this);
	StackCardsFrame->setObjectName("StackCardsFrame");
	StackCardsFrameLay=new HandLayout(StackCardsFrame);

	AnimationCard=new CardViewer(this);
	AnimationCard->SetCanBeZoom(false);
	AnimationCard->SetShadable(false);
	AnimationCard->hide();

	connect(this,SIGNAL(NeedResizeFrames()),this,SLOT(SizePosFrames()),Qt::QueuedConnection);
	connect(this,SIGNAL(NeedResizeCards()),this,SLOT(SizePosAllCards()),Qt::QueuedConnection);
	UpdateAspect();
}
void BattleGround::SetAllCards(QList<CardData> a){
	foreach(const CardData& crd,a){
		AllCards.insert(crd.GetCardID(),new Card(crd,this));
		AllCards[crd.GetCardID()]->hide();
	}
	emit NeedResizeCards();
}
void BattleGround::TurnTimeUpdate(){
	CurrentTurnTime+=TimerUpdateIntervall; 
	PhaseDisp->SetTurnTime(CurrentTurnTime); 
	/*TODO if(CurrentTurnTime>=TurnTimeLimit)*/
}
void BattleGround::PhaseTimeUpdate(){
	if (ResponseTimer->isActive()){
		CurrentResponseTime+=TimerUpdateIntervall;
		PhaseDisp->SetPhaseTime(CurrentResponseTime);
		if(CurrentResponseTime>=ResponseTimeLimit){
			ResponseTimer->stop();
			CurrentPhaseTime=0;
			PhaseDisp->SetStackTimerActivated(false);
			PhaseDisp->SetPhaseTimeLimit(PhaseTimeLimit);
			disconnect(ResponseTimer,SIGNAL(timeout()),this,SLOT(PhaseTimeUpdate()));
			connect(PhaseTimer,SIGNAL(timeout()),this,SLOT(PhaseTimeUpdate()));
			emit TimerFinished();
		}
	}
	else{
		CurrentPhaseTime+=TimerUpdateIntervall;
		PhaseDisp->SetPhaseTime(CurrentPhaseTime);
		if(CurrentPhaseTime>=PhaseTimeLimit){
			PhaseTimer->stop();
			emit TimerFinished();
		}
	}
}
void BattleGround::resizeEvent(QResizeEvent* event){
	if (!isVisible()) return;
	emit NeedResizeFrames();
}
void BattleGround::SizePosAllCards(){
	int HeiForWid=(279*ZoommedCardWidth)/200;
	GenericCard->setGeometry((width()-ZoommedCardWidth)/2,(height()-HeiForWid)/2,ZoommedCardWidth,HeiForWid);
	for(QMap<int,Card*>::iterator crd=AllCards.begin();crd!=AllCards.end();crd++)
		(*crd)->setGeometry((width()-ZoommedCardWidth)/2,(height()-HeiForWid)/2,ZoommedCardWidth,HeiForWid);
	UpdateAspect();
}
void BattleGround::SizePosFrames(){
	if (!isVisible()) return;
	int NextIndex=-1;
	switch (GetNumOfPlayers()){
	case 2:
		StackCardsFrame->setGeometry(
			(width()-qMin(StackCardsFrame->sizeHint().width(),1000*width()/1024))/2,
			(height()-92*height()/768)/2,
			qMin(StackCardsFrame->sizeHint().width(),1000*width()/1024),
			92*height()/768);
		PhaseDisp->setGeometry(125*width()/1024,359*height()/768,150*width()/1024,50*height()/768);
		QuestionFrame->setGeometry((width()-QuestionFrame->sizeHint().width())/2,30*height()/768,QuestionFrame->sizeHint().width(),QuestionFrame->sizeHint().height());
		DeckLabels.value(NextIndex)->setGeometry(915*width()/1024,418*height()/768,60*width()/1024,83*height()/768);
		GraveyardLabels.value(NextIndex)->setGeometry(915*width()/1024,538*height()/768,60*width()/1024,83*height()/768);
		HandFrames.value(NextIndex)->setGeometry(
			118+(qMax(910*width()/1024-HandsLay.value(NextIndex)->sizeHint().width(),0)/2),
			661*height()/768,
			qMin(HandsLay.value(NextIndex)->sizeHint().width(),910*width()/1024),
			92*height()/768);
		LandsContainer.value(NextIndex)->setGeometry(
			118+(qMax(910*width()/1024-LandsContainerLay.value(NextIndex)->sizeHint().width(),0)/2),
			561*height()/768,
			qMin(LandsContainerLay.value(NextIndex)->sizeHint().width(),910*width()/1024),
			92*height()/768);
		PlayesInfos.value(NextIndex)->setGeometry(
			10*width()/1024
			,389*height()/768
			,90*width()/1024
			,368*height()/768
			);
		if(PlayersOrder.indexOf(NextIndex)+1>=PlayersOrder.size())
			NextIndex=PlayersOrder.value(0);
		else
			NextIndex=PlayersOrder.value(PlayersOrder.indexOf(NextIndex)+1);
		DeckLabels.value(NextIndex)->setGeometry(48*width()/1024,268*height()/768,60*width()/1024,83*height()/768);
		GraveyardLabels.value(NextIndex)->setGeometry(48*width()/1024,148*height()/768,60*width()/1024,83*height()/768);
		HandFrames.value(NextIndex)->setGeometry(
			24+(qMax(910*width()/1024-HandsLay.value(NextIndex)->sizeHint().width(),0)/2),
			10*height()/768,
			qMin(HandsLay.value(NextIndex)->sizeHint().width(),910*width()/1024),
			92*height()/768);
		LandsContainer.value(NextIndex)->setGeometry(
			24+(qMax(910*width()/1024-LandsContainerLay.value(NextIndex)->sizeHint().width(),0)/2),
			110*height()/768,
			qMin(LandsContainerLay.value(NextIndex)->sizeHint().width(),1000*width()/1024),
			92*height()/768);
		PlayesInfos.value(NextIndex)->setGeometry(
			924*width()/1024
			,10*height()/768
			,90*width()/1024
			,368*height()/768
			);
		break;
	}
}
void BattleGround::UpdateAspect(){
	//if (!isVisible()) return;
	SortCardsInHand();
	SortCardsControlled();
	for(QList<int>::iterator index=PlayersOrder.begin();index!=PlayersOrder.end();index++){
		if (LibrarySizes.value(*index)==0)
			DeckLabels.value(*index)->hide();
		else{
			DeckLabels.value(*index)->show();
			DeckLabels.value(*index)->setText(QString("%1").arg(LibrarySizes.value(*index)));
		}
		if (CardsInGraveyard.value(*index).size()==0)
			GraveyardLabels.value(*index)->hide();
		else{
			GraveyardLabels.value(*index)->show();
			GraveyardLabels.value(*index)->setText(QString("%1").arg(CardsInGraveyard.value(*index).size()));
		}
/************************************************************************
* Update Hand                                                           *
*************************************************************************/
		while(CardsInHand.value(*index).size()>CardsInHandView.value(*index).size()){
			CardsInHandView[*index].append(new CardViewer(this));
			CardViewer* TempViewer=CardsInHandView[*index].last();
			connect(TempViewer,SIGNAL(LeftFocus()),this,SLOT(ResetHandOrder()));
			connect(TempViewer,SIGNAL(GainFocus()),TempViewer,SLOT(raise()),Qt::QueuedConnection);
			connect(TempViewer,SIGNAL(RequireZoom(Card*)),this,SLOT(ZoomAnimate(Card*)));
			connect(TempViewer,SIGNAL(DoubleClicked(int)),this,SLOT(WantToPlayCard(int)));
			HandsLay.value(*index)->addWidget(TempViewer);
		}
		while(CardsInHand.value(*index).size()<CardsInHandView.value(*index).size()){
			CardsInHandView[*index].takeFirst()->deleteLater();
		}
		for (int j=0;j<CardsInHand.value(*index).size();j++){
			if (CardsInHand[*index].value(j)){
				CardsInHandView[*index].value(j,NULL)->SetCardToDisplay(CardsInHand[*index].value(j));
				CardsInHandView[*index].value(j,NULL)->SetCanBeZoom(true);
				CardsInHandView[*index].value(j,NULL)->SetShadable(true);
				CardsInHandView[*index].value(j,NULL)->SetCanBeClick(CardsInHand[*index].value(j,NULL)->GetActivable());
			}
			else{
				CardsInHandView[*index].value(j)->SetCardToDisplay(GenericCard);
				CardsInHandView[*index].value(j)->SetCanBeZoom(false);
				CardsInHandView[*index].value(j)->SetShadable(false);
			}
			CardsInHandView[*index].value(j)->UpdateAspect();
		}
/************************************************************************
* Update Controlled Cards                                               *
*************************************************************************/
		//Lands
		while(LandsControlledView.value(*index).size()>NumberOfLands(*index)){
			LandsControlledView[*index].takeFirst()->deleteLater();
		}
		while(LandsControlledView.value(*index).size()<NumberOfLands(*index)){
			LandsControlledView[*index].append(new CardViewer(this));
			CardViewer* TempViewer=LandsControlledView[*index].last();
			connect(TempViewer,SIGNAL(LeftFocus()),this,SLOT(ResetLandOrder()));
			connect(TempViewer,SIGNAL(GainFocus()),TempViewer,SLOT(raise()),Qt::QueuedConnection);
			connect(TempViewer,SIGNAL(RequireZoom(Card*)),this,SLOT(ZoomAnimate(Card*)));
			LandsContainerLay.value(*index)->addWidget(TempViewer);
		}
		int contLands=0;
		for (int j=0;j<CardsControlled.value(*index).size();j++){
			if (CardsControlled[*index].value(j)){
				if (
					CardsControlled[*index].value(j)->GetCardType().contains(Constants::CardTypes::Land)
					&& (!CardsControlled[*index].value(j)->GetCardType().contains(Constants::CardTypes::Creature))
					)
				{
					LandsControlledView[*index].value(contLands,NULL)->SetCardToDisplay(CardsControlled[*index].value(j));
					LandsControlledView[*index].value(contLands,NULL)->SetCanBeZoom(true);
					LandsControlledView[*index].value(contLands,NULL)->SetShadable(false);
					LandsControlledView[*index].value(contLands,NULL)->SetCanBeClick(CardsControlled[*index].value(j,NULL)->GetActivable());
					LandsControlledView[*index].value(contLands++,NULL)->UpdateAspect();
				}
			}
		}
	}
/************************************************************************
* Update Stack Frame                                                    *
*************************************************************************/
	if(!AnimationCard->isVisible()){
		while(CardsInStackView.size()>CardsInStack.size()){
			CardsInStackView.takeFirst()->deleteLater();
		}
		while(CardsInStackView.size()<CardsInStack.size()){
			CardsInStackView.append(new CardViewer(this));
			CardViewer* TempViewer=CardsInStackView.last();
			connect(TempViewer,SIGNAL(LeftFocus()),this,SLOT(ResetStackLayOrder()));
			connect(TempViewer,SIGNAL(GainFocus()),TempViewer,SLOT(raise()),Qt::QueuedConnection);
			connect(TempViewer,SIGNAL(RequireZoom(Card*)),this,SLOT(ZoomAnimate(Card*)));
			StackCardsFrameLay->addWidget(TempViewer);
		}
		for(int stC=0;stC<CardsInStack.size();stC++){
			CardsInStackView.value(stC,NULL)->SetCardToDisplay(CardsInStack.value(stC));
			CardsInStackView.value(stC,NULL)->SetCanBeZoom(true);
			CardsInStackView.value(stC,NULL)->SetShadable(false);
			CardsInStackView.value(stC,NULL)->SetCanBeClick(false);
			CardsInStackView.value(stC,NULL)->UpdateAspect();
		}
	}
/************************************************************************
* Update Player Informations                                            *
*************************************************************************/
	for(QMap<int,PlayerInfoDisplayer*>::iterator inf=PlayesInfos.begin();inf!=PlayesInfos.end();inf++){
		inf.value()->SetLibrarySize(LibrarySizes.value(inf.key(),0));
		inf.value()->SetHandSize(CardsInHand.value(inf.key()).size());
		inf.value()->SetGraveyardSize(CardsInGraveyard.value(inf.key()).size());
		inf.value()->SetExileSize(CardsExiled.value(inf.key()).size());
		inf.value()->SetLifeValue(PlayerLife.value(inf.key()));
		inf.value()->UpdateAspect();
	}
	emit NeedResizeFrames();
	QuestionFrame->raise();
	setStyleSheet(StyleSheets::BoardCSS);
}
void BattleGround::SortCardsInHand(){
	foreach(int ks, PlayersOrder){
		foreach(Card* crd, CardsInHand[ks]){
			if(crd) crd->SetSortingMethod(HandSecondarySorting);
		}
		qSort(CardsInHand[ks].begin(),CardsInHand[ks].end(),Card::PointLessThan);
		foreach(Card* crd, CardsInHand[ks]){
			if(crd) crd->SetSortingMethod(HandPrimarySorting);
		}
		qStableSort(CardsInHand[ks].begin(),CardsInHand[ks].end(),Card::PointLessThan);
	}
}
void BattleGround::SortCardsControlled(){
	foreach(int ks, PlayersOrder){
		foreach(Card* crd, CardsControlled[ks]){
			if(crd) crd->SetSortingMethod(CardData::ByName);
		}
		qSort(CardsControlled[ks].begin(),CardsControlled[ks].end(),Card::PointLessThan);
		foreach(Card* crd, CardsInHand[ks]){
			if(crd) crd->SetSortingMethod(CardData::ByType);
		}
		qStableSort(CardsControlled[ks].begin(),CardsControlled[ks].end(),Card::PointLessThan);
	}
}
void BattleGround::ResetHandOrder(){
	foreach(HandLayout* hanlay,HandsLay)
		hanlay->invalidate();
}
void BattleGround::ResetLandOrder(){
	foreach(HandLayout* hanlay,LandsContainerLay)
		hanlay->invalidate();
}
void BattleGround::ResetStackLayOrder(){StackCardsFrameLay->invalidate();}

void BattleGround::SetPlayersOrder(QList<int> ord){
	PlayersOrder.clear(); PlayersOrder=ord;
	foreach(int PlayID,PlayersOrder){
		PlayerLife[PlayID]=20;
		HandFrames[PlayID]=new QFrame(this);
		HandsLay[PlayID]=new HandLayout(HandFrames[PlayID]);
		DeckLabels[PlayID]=new QLabel(this);
		DeckLabels[PlayID]->setObjectName("DeckLabel");
		DeckLabels[PlayID]->setAlignment(Qt::AlignCenter);
		GraveyardLabels[PlayID]=new QLabel(this);
		GraveyardLabels[PlayID]->setObjectName("GraveyardLabel");
		GraveyardLabels[PlayID]->setAlignment(Qt::AlignCenter);
		PlayesInfos[PlayID]=new PlayerInfoDisplayer(this);
		LandsContainer[PlayID]=new QFrame(this);
		LandsContainer[PlayID]->setObjectName("LandsContainer");
		LandsContainerLay[PlayID]=new HandLayout(LandsContainer[PlayID]);
	}
}
void BattleGround::SetPlayersNameAvatar(QMap<int,QString> nam,QMap<int,QPixmap> avt){
	for (QMap<int,QString>::const_iterator i=nam.constBegin();i!=nam.constEnd();i++){
		PlayerNames[i.key()]=i.value();
		PlayesInfos[i.key()]->SetPlayerName(i.value());
	}
	for (QMap<int,QPixmap>::const_iterator i=avt.constBegin();i!=avt.constEnd();i++){
		PlayerAvatars[i.key()]=i.value();
		PlayesInfos[i.key()]->SetPlayerAvatar(i.value());
	}
	UpdateAspect();
}
void BattleGround::SetMyHand(QList<int> hnd){
	while (!CardsInHand[-1].isEmpty()) CardsInHand[-1].takeAt(0)->deleteLater();
	foreach(int crd,hnd){
		CardsInHand[-1].append(AllCards[crd]);
		CardsInHand[-1].last()->UpdateAspect();
		CardsInHand[-1].last()->hide();
	}
	UpdateAspect();
}
void BattleGround::SetOtherHand(int whos,int numcards){
	while (!CardsInHand[whos].isEmpty()) CardsInHand[whos].takeAt(0)->deleteLater();
	for(int i=0;i<numcards;i++){
		CardsInHand[whos].append(NULL);
	}
	UpdateAspect();
}
void BattleGround::SetPlayerLibrary(int whos,int numcards){
	LibrarySizes[whos]=numcards;
	UpdateAspect();
}
void BattleGround::ClearQuestion(){
	QuestionButton1->disconnect();
	QuestionButton2->disconnect();
	QuestionFrame->hide();
}
void BattleGround::AskMulligan(){
	if(CardsInHand[-1].size()<1){
		emit KeepHand();
		return;
	}
	QuestionText->setText(tr("Do you want to Mulligan down to %1?").arg(CardsInHand[-1].size()-1));
	QuestionButton1->show();
	QuestionButton1->setText(tr("Mulligan"));
	connect(QuestionButton1,SIGNAL(clicked()),this,SIGNAL(Mulligan()));
	connect(QuestionButton1,SIGNAL(clicked()),this,SLOT(ClearQuestion()));
	QuestionButton2->show();
	QuestionButton2->setText(tr("Keep Hand"));
	connect(QuestionButton2,SIGNAL(clicked()),this,SIGNAL(KeepHand()));
	connect(QuestionButton2,SIGNAL(clicked()),this,SLOT(ClearQuestion()));
	QuestionFrame->show();
	emit NeedResizeFrames();
}
void BattleGround::DispalyWaitingFor(QString a){
	QuestionText->setText(a);
	QuestionFrame->show();
	QuestionButton1->hide();
	QuestionButton2->hide();
	emit NeedResizeFrames();
}
void BattleGround::HideWaitingFor(){
	QuestionFrame->hide();
}
void BattleGround::ZoomAnimate(Card* crd){
	if (ZoomedCard) ZoomedCard->hide();
	ZoomedCard=crd;
	if (!crd) return;
	crd->show();
	int HeiForWid=(279*ZoommedCardWidth)/200;
	QParallelAnimationGroup* Animations=new QParallelAnimationGroup;
	QPropertyAnimation* AnimPos=new QPropertyAnimation(crd,"pos",this);
	AnimPos->setDuration(AnimationDuration);
	AnimPos->setEasingCurve(QEasingCurve::InCubic);
	AnimPos->setKeyValueAt(0.0,QPoint(width()/2,height()/2));
	AnimPos->setKeyValueAt(1.0,QPoint((width()-ZoommedCardWidth)/2,(height()-HeiForWid)/2));
	Animations->addAnimation(AnimPos);
	QPropertyAnimation* AnimSize=new QPropertyAnimation(crd,"size",this);
	AnimSize->setDuration(AnimationDuration);
	AnimSize->setEasingCurve(QEasingCurve::InCubic);
	AnimSize->setKeyValueAt(0.0,QSize(0,0));
	AnimSize->setKeyValueAt(1.0,QSize(ZoommedCardWidth,HeiForWid));
	Animations->addAnimation(AnimSize);
	Animations->start(QAbstractAnimation::DeleteWhenStopped);
}
void BattleGround::UntapCards(QList<int> crds){
	foreach(int crd,crds)
		AllCards[crd]->SetTapped(false);
}
void BattleGround::SetCurrentPhase(int ph){
	if (ph<Constants::Phases::Untap || ph>Constants::Phases::TurnEnd) return;
	if(PhaseDisp->isHidden()) PhaseDisp->show();
	PhaseTimer->stop();
	CurrentPhaseTime=0;
	if(ph==Constants::Phases::Untap && ph!=CurrentPhase){
		TurnTimer->stop();
		CurrentTurnTime=0;
		TurnTimer->start();
	}
	CurrentPhase=ph;
	PhaseDisp->SetPhase(CurrentPhase);
	if (
		CurrentPhase!=Constants::Phases::PreCombatMain
		&& CurrentPhase!=Constants::Phases::PostCombatMain
		&& CurrentPhase!=Constants::Phases::DeclareAttackers
		&& CurrentPhase!=Constants::Phases::DeclareBlockers
	)
	{
		PhaseDisp->ShowButton(false);
		PhaseTimer->start();
	}
	else{
		PhaseDisp->ShowButton(true);
	}
}
void BattleGround::ResumeStackTimer(){
	PhaseDisp->ShowButton(false);
	disconnect(PhaseTimer,SIGNAL(timeout()),this,SLOT(PhaseTimeUpdate()));
	connect(ResponseTimer,SIGNAL(timeout()),this,SLOT(PhaseTimeUpdate()));
	CurrentResponseTime=0;
	PhaseDisp->SetStackTimerActivated(true);
	PhaseDisp->SetPhaseTimeLimit(ResponseTimeLimit);
	ResponseTimer->start();
}
void BattleGround::EffectAddedToStack(quint32 crd,EffectData eff){
	//TODO Visualize it
	ResumeStackTimer();
}
void BattleGround::DrawCard(int crd){
	LibrarySizes[-1]-=1;
	CardsInHand[-1].append(AllCards[crd]);
	CardsInHand[-1].last()->UpdateAspect();
	CardsInHand[-1].last()->hide();
	AnimateDraw(-1);
}
void BattleGround::OtherDraw(int who){
	if (!PlayersOrder.contains(who)) return;
	CardsInHand[who].append(NULL);
	AnimateDraw(who);
}
void BattleGround::AnimateDraw(int whos){
	if (!PlayersOrder.contains(whos)) return;
	int HeiForWid=(279*ZoommedCardWidth)/200;
	GenericCard->setGeometry((width()-ZoommedCardWidth)/2,(height()-HeiForWid)/2,ZoommedCardWidth,HeiForWid);
	AnimationCard->SetCardToDisplay(GenericCard);
	AnimationCard->UpdateAspect();
	AnimationCard->resize(DeckLabels[whos]->size());
	AnimationCard->show();
	QPropertyAnimation* AnimDraw=new QPropertyAnimation(AnimationCard,"pos",this);
	AnimDraw->setDuration(AnimationDuration);
	AnimDraw->setEasingCurve(QEasingCurve::Linear);
	AnimDraw->setKeyValueAt(0.0,DeckLabels[whos]->pos());
	switch (GetNumOfPlayers()){
	case 2:
		AnimDraw->setKeyValueAt(1.0,QPoint(
			HandFrames[whos]->pos().x()+(HandFrames[whos]->width()/2),
			whos==-1 ? height()+AnimationCard->height() : -AnimationCard->height()
			));
		break;
	}
	connect(AnimDraw,SIGNAL(finished()),AnimationCard,SLOT(hide()));
	connect(AnimDraw,SIGNAL(finished()),this,SLOT(UpdateAspect()));
	AnimDraw->start(QAbstractAnimation::DeleteWhenStopped);
}
void BattleGround::AnimatePlay(int whos,Card* ToShow){
	if (!PlayersOrder.contains(whos)) return;
	int HeiForWid=(279*ZoommedCardWidth)/200;
	ToShow->setGeometry((width()-ZoommedCardWidth)/2,(height()-HeiForWid)/2,ZoommedCardWidth,HeiForWid);
	AnimationCard->SetCardToDisplay(ToShow);
	AnimationCard->UpdateAspect();
	AnimationCard->resize(DeckLabels[whos]->size());
	AnimationCard->show();
	QPropertyAnimation* AnimPlay=new QPropertyAnimation(AnimationCard,"pos",this);
	AnimPlay->setDuration(AnimationDuration);
	AnimPlay->setEasingCurve(QEasingCurve::Linear);
	AnimPlay->setKeyValueAt(1.0,QPoint((width()-DeckLabels[whos]->width())/2,(height()-DeckLabels[whos]->height())/2));
	switch (GetNumOfPlayers()){
	case 2:
		AnimPlay->setKeyValueAt(0.0,QPoint(
			HandFrames[whos]->pos().x()+(HandFrames[whos]->width()/2),
			whos==-1 ? height()+AnimationCard->height() : -AnimationCard->height()
			));
		break;
	}
	connect(AnimPlay,SIGNAL(finished()),AnimationCard,SLOT(hide()));
	connect(AnimPlay,SIGNAL(finished()),this,SLOT(UpdateAspect()));
	AnimPlay->start(QAbstractAnimation::DeleteWhenStopped);
}
void BattleGround::StopTimer(){
	ResponseTimer->stop();
	PhaseTimer->stop();
	PhaseDisp->PausePhaseTimer();
}
void BattleGround::StopTurnTimer(){TurnTimer->stop();}
void BattleGround::ResumeTurnTimer(){TurnTimer->start();}
void BattleGround::SetPlayableCards(QList<int> IDs){
	for(QMap<int,Card*>::iterator i=AllCards.begin();i!=AllCards.end();i++){
		(*i)->SetActivable(IDs.contains(i.key()));
	}
	UpdateAspect();
}
void BattleGround::WantToPlayCard(int crdID){
	Card* TheCard=NULL;
	for(QList<Card*>::iterator i=CardsInHand[-1].begin();i!=CardsInHand[-1].end();i++){
		if((*i)->GetCardID()==crdID){
			TheCard=*i;
			break;
		}
	}
	if (!TheCard) return;
	if (TheCard->GetHasManaCost()){
		//TODO
	}
	emit WantPlayCard(crdID);
}
void BattleGround::PlayedCard(int crd,int Who){
	if (!PlayersOrder.contains(Who)) return;
	CardsInStack.append(AllCards[crd]);
	ResumeStackTimer();
	AnimatePlay(Who,CardsInStack.last());
}
void BattleGround::RemoveCardHand(int who,int crdID){
	if (!CardsInHand[who].contains(AllCards.value(crdID))) return;
	CardsInHand[who].erase(CardsInHand[who].begin()+CardsInHand[who].indexOf(AllCards.value(crdID)));
	UpdateAspect();
}
void BattleGround::ResolveCard(int Who, int crd){
	if(CardsControlled[Who].contains(AllCards.value(crd))) return;
	CardsControlled[Who].append(AllCards[crd]);
	CardsControlled[Who].last()->SetActivable(false);
	UpdateAspect();
}
int BattleGround::NumberOfLands(int who){
	int Result=0;
	foreach(Card* crd,CardsControlled[who]){
		if(
			crd->GetCardType().contains(Constants::CardTypes::Land)
			&& (!crd->GetCardType().contains(Constants::CardTypes::Creature))
		)++Result;
	}
	return Result;
}