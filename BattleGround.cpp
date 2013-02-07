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
	UpdateAspect();
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
void BattleGround::SizePosFrames(){
	if (!isVisible()) return;
	int NextIndex=-1;
	switch (Players.size()){
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
	if (!isVisible()) return;
	int HeiForWid=(279*ZoommedCardWidth)/200;
	SortCardsInHand();
	SortCardsControlled();
	for(QMap<int,MagiQPlayer*>::iterator index=Players.begin();index!=Players.end();index++){
		if (index.value()->GetLibrary().size()==0)
			DeckLabels.value(index.key())->hide();
		else{
			DeckLabels.value(index.key())->show();
			DeckLabels.value(index.key())->setText(QString("%1").arg(index.value()->GetLibrary().size()));
		}
		if (index.value()->GetGraveyard().size()==0)
			GraveyardLabels.value(index.key())->hide();
		else{
			GraveyardLabels.value(index.key())->show();
			GraveyardLabels.value(index.key())->setText(QString("%1").arg(index.value()->GetGraveyard().size()));
		}
		GenericCard->setGeometry((width()-ZoommedCardWidth)/2,(height()-HeiForWid)/2,ZoommedCardWidth,HeiForWid);
/************************************************************************
* Update Hand                                                           *
*************************************************************************/
		foreach(int ks, Players.keys()){
			for(QList<Card*>::iterator crd=CardsInHand[ks].begin();crd!=CardsInHand[ks].end();crd++)
				(*crd)->setGeometry((width()-ZoommedCardWidth)/2,(height()-HeiForWid)/2,ZoommedCardWidth,HeiForWid);
		}
		while(index.value()->GetHand().size()>CardsInHandView.value(index.key()).size()){
			CardsInHandView[index.key()].append(new CardViewer(this));
			CardViewer* TempViewer=CardsInHandView[index.key()].last();
			connect(TempViewer,SIGNAL(LeftFocus()),this,SLOT(ResetHandOrder()));
			connect(TempViewer,SIGNAL(GainFocus()),TempViewer,SLOT(raise()),Qt::QueuedConnection);
			connect(TempViewer,SIGNAL(RequireZoom(Card*)),this,SLOT(ZoomAnimate(Card*)));
			connect(TempViewer,SIGNAL(DoubleClicked(int)),this,SLOT(WantToPlayCard(int)));
			HandsLay.value(index.key())->addWidget(TempViewer);
		}
		while(index.value()->GetHand().size()<CardsInHandView.value(index.key()).size()){
			CardsInHandView[index.key()].takeFirst()->deleteLater();
		}
		for (int j=0;j<index.value()->GetHand().size();j++){
			if (CardsInHand[index.key()].value(j,NULL)){
				if (!CardsInHand[index.key()].value(j,NULL)->GetIsNull()){
					CardsInHandView[index.key()].value(j,NULL)->SetCardToDisplay(CardsInHand[index.key()].value(j));
					CardsInHandView[index.key()].value(j,NULL)->SetCanBeZoom(true);
					CardsInHandView[index.key()].value(j,NULL)->SetShadable(true);
					CardsInHandView[index.key()].value(j,NULL)->SetCanBeClick(CardsInHand[index.key()].value(j,NULL)->GetActivable());
				}
				else{
					CardsInHandView[index.key()].value(j)->SetCardToDisplay(GenericCard);
					CardsInHandView[index.key()].value(j)->SetCanBeZoom(false);
					CardsInHandView[index.key()].value(j)->SetShadable(false);
				}
				CardsInHandView[index.key()].value(j)->UpdateAspect();
			}
		}
/************************************************************************
* Update Controlled Cards                                               *
*************************************************************************/
		foreach(int ks, Players.keys()){
			for(QList<Card*>::iterator crd=CardsControlled[ks].begin();crd!=CardsControlled[ks].end();crd++)
				(*crd)->setGeometry((width()-ZoommedCardWidth)/2,(height()-HeiForWid)/2,ZoommedCardWidth,HeiForWid);
		}
		//Lands
		while(LandsControlledView.value(index.key()).size()>NumberOfLands(index.key())){
			LandsControlledView[index.key()].takeFirst()->deleteLater();
		}
		while(LandsControlledView.value(index.key()).size()<NumberOfLands(index.key())){
			LandsControlledView[index.key()].append(new CardViewer(this));
			CardViewer* TempViewer=LandsControlledView[index.key()].last();
			connect(TempViewer,SIGNAL(LeftFocus()),this,SLOT(ResetLandOrder()));
			connect(TempViewer,SIGNAL(GainFocus()),TempViewer,SLOT(raise()),Qt::QueuedConnection);
			connect(TempViewer,SIGNAL(RequireZoom(Card*)),this,SLOT(ZoomAnimate(Card*)));
			LandsContainerLay.value(index.key())->addWidget(TempViewer);
		}
		int contLands=0;
		for (int j=0;j<index.value()->GetControlledCards().size();j++){
			if (CardsControlled[index.key()].value(j,NULL)){
				if (
					CardsControlled[index.key()].value(j)->GetCardType().contains(Constants::CardTypes::Land)
					&& (!CardsControlled[index.key()].value(j)->GetCardType().contains(Constants::CardTypes::Creature))
					)
				{
					LandsControlledView[index.key()].value(contLands,NULL)->SetCardToDisplay(CardsControlled[index.key()].value(j));
					LandsControlledView[index.key()].value(contLands,NULL)->SetCanBeZoom(true);
					LandsControlledView[index.key()].value(contLands,NULL)->SetShadable(false);
					LandsControlledView[index.key()].value(contLands,NULL)->SetCanBeClick(CardsControlled[index.key()].value(j,NULL)->GetActivable());
					LandsControlledView[index.key()].value(contLands++,NULL)->UpdateAspect();
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
			CardsInStack.value(stC,NULL)->setGeometry((width()-ZoommedCardWidth)/2,(height()-HeiForWid)/2,ZoommedCardWidth,HeiForWid);
			CardsInStackView.value(stC,NULL)->SetCardToDisplay(CardsInStack.value(stC,NULL));
			CardsInStackView.value(stC,NULL)->SetCanBeZoom(true);
			CardsInStackView.value(stC,NULL)->SetShadable(false);
			CardsInStackView.value(stC,NULL)->SetCanBeClick(false);
			CardsInStackView.value(stC,NULL)->UpdateAspect();
		}
	}
	foreach(PlayerInfoDisplayer* inf,PlayesInfos)
		inf->UpdateAspect();
	emit NeedResizeFrames();
	QuestionFrame->raise();
	setStyleSheet(StyleSheets::BoardCSS);
}
void BattleGround::SortCardsInHand(){
	int index;
	int MinimumIndex=-1;
	QMap<int,Card*> Sorter;
	foreach(int ks, Players.keys()){
		Sorter.clear();
		Players.value(ks)->SortHand();
		const QList<CardData>& TempList=Players.value(ks)->GetHand();
		foreach(Card* car,CardsInHand[ks]){
			index=-1;
			if (car->GetIsNull())
				Sorter.insert(MinimumIndex--,car);
			else{
				for(int j=0;j<TempList.size() && index<0;j++){
					if(TempList.value(j).GetCardID()==car->GetCardID())
						index=j;
				}
				Sorter.insert(index,car);
			}
		}
		CardsInHand[ks]=Sorter.values();
	}
}
void BattleGround::SortCardsControlled(){
	int index;
	int MinimumIndex=-1;
	QMap<int,Card*> Sorter;
	foreach(int ks, Players.keys()){
		Sorter.clear();
		Players.value(ks)->SortControlled();
		const QList<CardData>& TempList=Players.value(ks)->GetControlledCards();
		foreach(Card* car,CardsControlled[ks]){
			index=-1;
			if (car->GetIsNull())
				Sorter.insert(MinimumIndex--,car);
			else{
				for(int j=0;j<TempList.size() && index<0;j++){
					if(TempList.value(j).GetCardID()==car->GetCardID())
						index=j;
				}
				Sorter.insert(index,car);
			}
		}
		CardsControlled[ks]=Sorter.values();
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
		Players[PlayID]=new MagiQPlayer(this);
		HandFrames[PlayID]=new QFrame(this);
		HandsLay[PlayID]=new HandLayout(HandFrames[PlayID]);
		DeckLabels[PlayID]=new QLabel(this);
		DeckLabels[PlayID]->setObjectName("DeckLabel");
		DeckLabels[PlayID]->setAlignment(Qt::AlignCenter);
		GraveyardLabels[PlayID]=new QLabel(this);
		GraveyardLabels[PlayID]->setObjectName("GraveyardLabel");
		GraveyardLabels[PlayID]->setAlignment(Qt::AlignCenter);
		PlayesInfos[PlayID]=new PlayerInfoDisplayer(this);
		PlayesInfos[PlayID]->SetInfosToDisplay(Players[PlayID]);
		LandsContainer[PlayID]=new QFrame(this);
		LandsContainer[PlayID]->setObjectName("LandsContainer");
		LandsContainerLay[PlayID]=new HandLayout(LandsContainer[PlayID]);
	}
}
void BattleGround::SetPlayersNameAvatar(QMap<int,QString> nam,QMap<int,QPixmap> avt){
	for (QMap<int,QString>::const_iterator i=nam.constBegin();i!=nam.constEnd();i++)
		Players[i.key()]->SetPlayerName(i.value());
	for (QMap<int,QPixmap>::const_iterator i=avt.constBegin();i!=avt.constEnd();i++)
		Players[i.key()]->SetAvatar(i.value());
	UpdateAspect();
}
void BattleGround::SetMyHand(QList<CardData> hnd){
	Players[-1]->SetHand(hnd);
	while (!CardsInHand[-1].isEmpty()) CardsInHand[-1].takeAt(0)->deleteLater();
	foreach(CardData crd,Players[-1]->GetHand()){
		CardsInHand[-1].append(new Card(crd,this));
		AllCards.insert(CardsInHand[-1].last());
		CardsInHand[-1].last()->UpdateAspect();
		CardsInHand[-1].last()->hide();
	}
	UpdateAspect();
}
void BattleGround::SetOtherHand(int whos,int numcards){
	CardData NullCard;
	NullCard.SetIsNull(true);
	while (!CardsInHand[whos].isEmpty()) CardsInHand[whos].takeAt(0)->deleteLater();
	for(int i=0;i<numcards;i++) Players[whos]->AddHand(NullCard);
	foreach(CardData crd,Players[whos]->GetHand()){
		CardsInHand[whos].append(new Card(crd,this));
		CardsInHand[whos].last()->UpdateAspect();
		CardsInHand[whos].last()->hide();
	}
	UpdateAspect();
}
void BattleGround::SetMyLibrary(QList<CardData> libr){
	Players.value(-1,NULL)->SetLibrary(libr);
	UpdateAspect();
}
void BattleGround::SetOtherLibrary(int whos,int numcards){
	for(int i=0;i<numcards;i++) Players[whos]->AddLibrary(CardData());
	UpdateAspect();
}
void BattleGround::ClearQuestion(){
	QuestionButton1->disconnect();
	QuestionButton2->disconnect();
	QuestionFrame->hide();
}
void BattleGround::AskMulligan(){
	if(Players[-1]->GetHand().size()<1){
		emit KeepHand();
		return;
	}
	QuestionText->setText(tr("Do you want to Mulligan down to %1?").arg(Players[-1]->GetHand().size()-1));
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
	for(QSet<Card*>::iterator i=AllCards.begin();i!=AllCards.end();i++)
		(*i)->SetTapped(false);
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
void BattleGround::DrawCard(CardData crd){
	Players[-1]->RemoveTopLibrary();
	Players[-1]->AddHand(crd);
	CardsInHand[-1].append(new Card(crd,this));
	AllCards.insert(CardsInHand[-1].last());
	CardsInHand[-1].last()->UpdateAspect();
	CardsInHand[-1].last()->hide();
	AnimateDraw(-1);
}
void BattleGround::OtherDraw(int who){
	CardData NullCard;
	NullCard.SetIsNull(true);
	if (!PlayersOrder.contains(who)) return;
	Players[who]->AddHand(NullCard);
	CardsInHand[who].append(new Card(NullCard,this));
	CardsInHand[who].last()->UpdateAspect();
	CardsInHand[who].last()->hide();
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
	switch (Players.size()){
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
	switch (Players.size()){
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
	for(QSet<Card*>::iterator i=AllCards.begin();i!=AllCards.end();i++){
		(*i)->SetActivable(IDs.contains((*i)->GetCardID()));
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
void BattleGround::PlayedCard(CardData crd,int Who){
	if (!PlayersOrder.contains(Who)) return;

	//Remove Cards from Hand and add it to the stacked cards
	QList<Card*>::iterator ite;
	for (ite=CardsInHand[Who].begin();ite!=CardsInHand[Who].end();ite++){
		if((*ite)->GetCardID()==crd.GetCardID()){
			CardsInStack.append(*ite);
			CardsInHand[Who].erase(ite);
			break;
		}
	}
	if (ite==CardsInHand[Who].end()){
		CardsInStack.append(new Card(crd));
		AllCards.insert(CardsInStack.last());
	}
	if (Players[Who]->RemoveFromHand(crd.GetCardID()).GetCardID()==0){
		for(QList<Card*>::iterator i=CardsInHand[Who].begin();i!=CardsInHand[Who].end();i++){
			if((*i)->GetIsNull()){
				(*i)->deleteLater();
				CardsInHand[Who].erase(i);
				break;
			}
		}
	}
	if (!crd.GetCardType().contains(Constants::CardTypes::Land)) ResumeStackTimer();
	AnimatePlay(Who,CardsInStack.last());
}
void BattleGround::RemoveCardHand(int who,int crdID){
	int RemovedID=Players[who]->RemoveFromHand(crdID).GetCardID();
	for(QList<Card*>::iterator i=CardsInHand[who].begin();i!=CardsInHand[who].end();i++){
		if (RemovedID==0){
			if((*i)->GetIsNull()){
				(*i)->deleteLater();
				CardsInHand[who].erase(i);
				break;
			}
		}
		else{
			if((*i)->GetCardID()==RemovedID){
				(*i)->deleteLater();
				CardsInHand[who].erase(i);
				break;
			}
		}
	}
	UpdateAspect();
}
void BattleGround::ResolveCard(int Who, CardData crd){
	for(QList<Card*>::const_iterator i=CardsControlled[Who].constBegin();i!=CardsControlled[Who].constEnd();i++){
		if ((*i)->GetCardID()==crd.GetCardID()) return;
	}	
	CardsControlled[Who].append(new Card(crd,this));
	AllCards.insert(CardsControlled[Who].last());
	CardsControlled[Who].last()->SetController(Players[Who]);
	CardsControlled[Who].last()->SetActivable(false);
	Players[Who]->AddControlledCard(crd);
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