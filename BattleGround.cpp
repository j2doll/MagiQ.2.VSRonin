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
#include "ControlledLayout.h"
#include "CardViewer.h"
#include "Card.h"
#include "Effect.h"
#include "PlayerInfoDisplayer.h"
#include "PhasesDisplayer.h"
#include "ManaCostLabel.h"
#include "SignalerArrow.h"
#ifdef _DEBUG
	#include "SizeSliders.h"
#endif
BattleGround::BattleGround(QWidget* parent)
	:QWidget(parent)
	,ZoomedCard(NULL)
	,PhaseTimeLimit(1000)
	,ResponseTimeLimit(1500)
	,TurnTimeLimit(300000)
	,HandPrimarySorting(CardData::ByManaCost)
	,HandSecondarySorting(CardData::ByType)
	,ManaSelectionModeON(false)
	,CardRequiringMana(NULL)
	,AttackTargetsSelectionModeON(false)
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
	QuestionFrame->hide();
	QuestionLowFrame=new QFrame(this);
	QuestionLowFrame->setObjectName("QuestionFrame");
	QuestionLayout=new QGridLayout(QuestionLowFrame);
	QuestionLowText=new QLabel(QuestionLowFrame);
	QuestionLowText->setScaledContents(true);
	QuestionLowText->setObjectName("QuestionText");
	QuestionLowText->setAlignment(Qt::AlignCenter);
	QuestionLowText->setWordWrap(true);
	QuestionLayout->addWidget(QuestionLowText,0,0,1,2);
	QuestionLowButton1=new QPushButton(QuestionLowFrame);
	QuestionLowButton1->setObjectName("QuestionButton");
	QuestionLayout->addWidget(QuestionLowButton1,1,0);
	QuestionLowFrame->hide();

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
	

	RequiredCostFrame=new QFrame(this);
	RequiredCostFrame->setObjectName("CostFrame");
	QVBoxLayout* TempLay=new QVBoxLayout(RequiredCostFrame);
	TempLay->setMargin(4);
	TempLay->setSpacing(2);
	RequiredCostText=new QLabel(RequiredCostFrame);
	RequiredCostText->setObjectName("CostText");
	RequiredCostText->setText(tr("Cost:"));
	TempLay->addWidget(RequiredCostText);
	RequiredCostLabel=new ManaCostLabel(RequiredCostFrame);
	RequiredCostLabel->setObjectName("CostLabel");
	TempLay->addWidget(RequiredCostLabel);
	RequiredCostFrame->hide();
	PayedCostFrame=new QFrame(this);
	PayedCostFrame->setObjectName("CostFrame");
	TempLay=new QVBoxLayout(PayedCostFrame);
	TempLay->setMargin(4);
	TempLay->setSpacing(2);
	PayedCostText=new QLabel(PayedCostFrame);
	PayedCostText->setObjectName("CostText");
	PayedCostText->setText(tr("Payed:"));
	TempLay->addWidget(PayedCostText);
	PayedCostLabel=new ManaCostLabel(this);
	PayedCostLabel->setObjectName("CostLabel");
	TempLay->addWidget(PayedCostLabel);
	PayedCostFrame->hide();

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
			if(CurrentPhase==Constants::Phases::PreCombatMain || CurrentPhase==Constants::Phases::PostCombatMain)
				PhaseDisp->ShowButton(true);
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
void BattleGround::SizePosCosts(){
	RequiredCostFrame->resize(
		50*(RequiredCostLabel->GetNumberOfSymbols()>0 ? RequiredCostLabel->GetNumberOfSymbols() : 1) *width()/1024,
		60*height()/768
	);
	RequiredCostFrame->move(
		(width()-RequiredCostFrame->width())/2,
		218*height()/768
	);
	PayedCostFrame->resize(
		50*(PayedCostLabel->GetNumberOfSymbols()>0 ? PayedCostLabel->GetNumberOfSymbols() : 1) *width()/1024,
		60*height()/768
	);
	PayedCostFrame->move(
		(width()-PayedCostFrame->width())/2,
		282*height()/768
	);
	QuestionLowFrame->setGeometry(
		737*width()/1024,
		608*height()/768,
		200*width()/1024,
		105*height()/768
		);
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
		CreaturesContainer.value(NextIndex)->setGeometry(
			118+(qMax(910*width()/1024-CreaturesContainerLay.value(NextIndex)->sizeHint().width(),0)/2),
			461*height()/768,
			qMin(CreaturesContainerLay.value(NextIndex)->sizeHint().width(),910*width()/1024),
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
		CreaturesContainer.value(NextIndex)->setGeometry(
			24+(qMax(910*width()/1024-CreaturesContainerLay.value(NextIndex)->sizeHint().width(),0)/2),
			210*height()/768,
			qMin(CreaturesContainerLay.value(NextIndex)->sizeHint().width(),1000*width()/1024),
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
				CardsInHandView[*index].value(j,NULL)->SetCanBeClick(ManaSelectionModeON ? false : CardsInHand[*index].value(j,NULL)->GetActivable());
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
		int numOfLands=NumberOfLands(*index);
		while(LandsControlledView.value(*index).size()>numOfLands){
			LandsControlledView[*index].takeFirst()->deleteLater();
		}
		while(LandsControlledView.value(*index).size()<numOfLands){
			LandsControlledView[*index].append(new CardViewer(this));
			CardViewer* TempViewer=LandsControlledView[*index].last();
			connect(TempViewer,SIGNAL(LeftFocus()),this,SLOT(ResetLandOrder()));
			connect(TempViewer,SIGNAL(GainFocus()),TempViewer,SLOT(raise()),Qt::QueuedConnection);
			connect(TempViewer,SIGNAL(RequireZoom(Card*)),this,SLOT(ZoomAnimate(Card*)));
			LandsContainerLay.value(*index)->addWidget(TempViewer);
		}
		//Creatures
		int numOfCreatures=NumberOfCreatures(*index);
		while(CreaturesControlledView.value(*index).size()>numOfCreatures){
			CreaturesControlledView[*index].takeFirst()->deleteLater();
		}
		while(CreaturesControlledView.value(*index).size()<numOfCreatures){
			CreaturesControlledView[*index].append(new CardViewer(this));
			CardViewer* TempViewer=CreaturesControlledView[*index].last();
			connect(TempViewer,SIGNAL(LeftFocus()),this,SLOT(ResetLandOrder()));
			connect(TempViewer,SIGNAL(GainFocus()),TempViewer,SLOT(raise()),Qt::QueuedConnection);
			connect(TempViewer,SIGNAL(RequireZoom(Card*)),this,SLOT(ZoomAnimate(Card*)));
			CreaturesContainerLay.value(*index)->addWidget(TempViewer);
		}
		int contLands=0;
		int contCreatures=0;
		bool ManaSourceCheck;
		for (int j=0;j<CardsControlled.value(*index).size();j++){
			if (CardsControlled[*index].value(j)){
				ManaSourceCheck=
					ManaSelectionModeON
					&& (!CardsControlled[*index].value(j,NULL)->IsTapped())
					&& CardsControlled[*index].value(j,NULL)->IsManaSource()
					&& (!CardsUsedToPay.contains(CardsControlled[*index].value(j,NULL)->GetCardID()))
					;
				if ( //Is a Land
					CardsControlled[*index].value(j)->GetCardType().contains(Constants::CardTypes::Land)
					&& (!CardsControlled[*index].value(j)->GetCardType().contains(Constants::CardTypes::Creature))
					)
				{
					LandsControlledView[*index].value(contLands,NULL)->SetCardToDisplay(CardsControlled[*index].value(j));
					LandsControlledView[*index].value(contLands,NULL)->SetCanBeZoom(true);
					LandsControlledView[*index].value(contLands,NULL)->SetShadable(ManaSourceCheck);
					LandsControlledView[*index].value(contLands,NULL)->SetCanBeClick(
						ManaSourceCheck
						||
						((!ManaSelectionModeON) && CardsControlled[*index].value(j,NULL)->GetActivable())
					);
					if (CardsControlled[*index].value(j)->IsTapped()){
						if(CardsControlled[*index].value(j)->GetTapAnimationDone()) LandsControlledView[*index].value(contLands,NULL)->SetCardRotation(90);
						else{
							LandsControlledView[*index].value(contLands,NULL)->TapAnimationStart();
							CardsControlled[*index].value(j)->SetTapAnimationDone(true);
						}
					}
					else{ //Not Tapped
						if(CardsControlled[*index].value(j)->GetTapAnimationDone()) LandsControlledView[*index].value(contLands,NULL)->SetCardRotation(0);
						else{
							LandsControlledView[*index].value(contLands,NULL)->UnTapAnimationStart();
							CardsControlled[*index].value(j)->SetTapAnimationDone(true);
						}
					}
					++contLands;
				}
				else if(CardsControlled[*index].value(j)->GetCardType().contains(Constants::CardTypes::Creature)){ //Is a Creature
					CreaturesControlledView[*index].value(contCreatures,NULL)->SetCardToDisplay(CardsControlled[*index].value(j));
					CreaturesControlledView[*index].value(contCreatures,NULL)->SetCanBeZoom(true);
					CreaturesControlledView[*index].value(contCreatures,NULL)->SetShadable(
						ManaSourceCheck
						|| (CurrentPhase==Constants::Phases::DeclareAttackers && CardsThatCanAttack.contains(CardsControlled[*index].value(j)->GetCardID()))
						);
					CreaturesControlledView[*index].value(contCreatures,NULL)->SetCanBeClick(
						(
							(!ManaSelectionModeON)
							&& CurrentPhase!=Constants::Phases::DeclareAttackers
							&& CurrentPhase!=Constants::Phases::DeclareBlockers
							&& CardsControlled[*index].value(j,NULL)->GetActivable()
						)
						||
						(
							CurrentPhase==Constants::Phases::DeclareAttackers
							&& CardsThatCanAttack.contains(CardsControlled[*index].value(j,NULL)->GetCardID())
						)
						);
					CreaturesControlledView[*index].value(contCreatures,NULL)->SetAttacking(CardsControlled[*index].value(j,NULL)->GetAttacking());
					if (CardsControlled[*index].value(j)->IsTapped()){
						if(CardsControlled[*index].value(j)->GetTapAnimationDone()) CreaturesControlledView[*index].value(contCreatures,NULL)->SetCardRotation(90);
						else{
							CreaturesControlledView[*index].value(contCreatures,NULL)->TapAnimationStart();
							CardsControlled[*index].value(j)->SetTapAnimationDone(true);
						}
					}
					else{ //Not Tapped
						if(CardsControlled[*index].value(j)->GetTapAnimationDone()) CreaturesControlledView[*index].value(contCreatures,NULL)->SetCardRotation(0);
						else{
							CreaturesControlledView[*index].value(contCreatures,NULL)->UnTapAnimationStart();
							CardsControlled[*index].value(j)->SetTapAnimationDone(true);
						}
					}
					++contCreatures;
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
* Update Arrows															*
*************************************************************************/
	int ContArrows=0;
//Attack Arrows
	for(QHash<int,int>::const_iterator atckIter=AttackingCards.constBegin();atckIter!=AttackingCards.constEnd();atckIter++){
		if(atckIter.value()==NeedsATarget) continue;
		++ContArrows;
		while(Arrows.size()<ContArrows){
			Arrows.append(new SignalerArrow(this));
			Arrows.last()->setObjectName("SignalArrow");
		}
		bool CardNotFound=true;
		for(QMap<int,QList<CardViewer*>>::const_iterator creatIter=CreaturesControlledView.constBegin();CardNotFound && creatIter!=CreaturesControlledView.constEnd();creatIter++){
			for(QList<CardViewer*>::const_iterator viewIter=creatIter.value().constBegin();CardNotFound && viewIter!=creatIter.value().constEnd();viewIter++){
				if((*viewIter)->GetCardToDisplay()->GetCardID()==atckIter.key()){
					CardNotFound=false;
					QPoint tempPoint((*viewIter)->width()/2,(*viewIter)->height()/2);
					QWidget* CurrentWidget=(*viewIter);
					do{
						tempPoint.rx()+=CurrentWidget->pos().x();
						tempPoint.ry()+=CurrentWidget->pos().y();
						CurrentWidget=CurrentWidget->parentWidget();
					}while(CurrentWidget && CurrentWidget!=this);
					Arrows.at(ContArrows-1)->SetFrom(tempPoint);
					connect(*viewIter,SIGNAL(RaisedWidget()),Arrows.at(ContArrows-1),SLOT(raise()));
				}
			}
		}
		if(CardNotFound){--ContArrows; continue;}
		CardNotFound=true;
		if(atckIter.value()<0){
			CardNotFound=false;
			PlayerInfoDisplayer* const& CurrentWidget=PlayesInfos[atckIter.value()==-1 ? -1:-atckIter.value()];
			QPoint tempPoint(CurrentWidget->PlayerAvatarCenter());
			tempPoint.rx()+=CurrentWidget->pos().x();
			tempPoint.ry()+=CurrentWidget->pos().y();
			Arrows.at(ContArrows-1)->SetTo(tempPoint);
			Arrows.at(ContArrows-1)->SetArrowColor(SignalerArrow::ArrowRed);
		}
		else{
			for(QMap<int,QList<CardViewer*>>::const_iterator creatIter=CreaturesControlledView.constBegin();CardNotFound && creatIter!=CreaturesControlledView.constEnd();creatIter++){
				for(QList<CardViewer*>::const_iterator viewIter=creatIter.value().constBegin();CardNotFound && viewIter!=creatIter.value().constEnd();viewIter++){
					if((*viewIter)->GetCardToDisplay()->GetCardID()==atckIter.value()){
						CardNotFound=false;
						QPoint tempPoint((*viewIter)->pos());
						tempPoint.rx()+=CreaturesContainer.value(creatIter.key())->pos().x();
						tempPoint.ry()+=CreaturesContainer.value(creatIter.key())->pos().y();
						Arrows.at(ContArrows-1)->SetTo(tempPoint);
						Arrows.at(ContArrows-1)->SetArrowColor(SignalerArrow::ArrowYellow);
					}
				}
			}
		}
		if(CardNotFound) {--ContArrows; continue;}
		Arrows.at(ContArrows-1)->show();
		Arrows.at(ContArrows-1)->raise();
	}
	while(Arrows.size()>ContArrows){
		Arrows.takeLast()->deleteLater();
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
		foreach(Card* crd, CardsControlled[ks]){
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
	foreach(ControlledLayout* hanlay,LandsContainerLay)
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
		PlayesInfos[PlayID]->SetPlayerID(PlayID);
		LandsContainer[PlayID]=new QFrame(this);
		LandsContainer[PlayID]->setObjectName("LandsContainer");
		LandsContainerLay[PlayID]=new ControlledLayout(LandsContainer[PlayID]);
		CreaturesContainer[PlayID]=new QFrame(this);
		CreaturesContainer[PlayID]->setObjectName("CreaturesContainer");
		CreaturesContainerLay[PlayID]=new ControlledLayout(CreaturesContainer[PlayID]);
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
void BattleGround::ClearLowerQuestion(){
	QuestionLowButton1->disconnect();
	QuestionLowFrame->hide();
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
	crd->raise();
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
	foreach(int crdID,crds){
		if(!AllCards[crdID]->IsTapped()) continue;
		AllCards[crdID]->SetTapped(false);
		AllCards[crdID]->SetTapAnimationDone(false);
	}
	UpdateAspect();
}
void BattleGround::TapCards(QList<int> crds){
	foreach(int crdID,crds){
		if(AllCards[crdID]->IsTapped()) continue;
		AllCards[crdID]->SetTapped(true);
		AllCards[crdID]->SetTapAnimationDone(false);
	}
	UpdateAspect();
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
	disconnect(PhaseDisp,SIGNAL(Continued()));
	if(CurrentPhase==Constants::Phases::PreCombatMain || CurrentPhase==Constants::Phases::PostCombatMain){
		PhaseDisp->ShowButton(true);
		PhaseDisp->SetButtonString(tr("Continue"));
		connect(PhaseDisp,SIGNAL(Continued()),this,SIGNAL(ContinueToNextPhase()),Qt::UniqueConnection);
	}
	else if(CurrentPhase==Constants::Phases::DeclareAttackers){
		PhaseDisp->ShowButton(true);
		PhaseDisp->SetButtonString(tr("Skip Attack"));
		connect(PhaseDisp,SIGNAL(Continued()),this,SLOT(WantsToAttack()),Qt::UniqueConnection);
	}
	else if(CurrentPhase==Constants::Phases::DeclareBlockers){
		PhaseDisp->ShowButton(true);
		PhaseDisp->SetButtonString(tr("Skip Block"));
	}
	else{
		PhaseDisp->ShowButton(false);
		PhaseTimer->start();
	}
}
void BattleGround::WantsToAttack(){
	emit SendAttackingCards(AttackingCards);
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
void BattleGround::SetAttackAbleCards(QList<int> crdIDs){
	CardsThatCanAttack.clear();
	AttackingCards.clear();
	if(crdIDs.isEmpty()){
		emit SendAttackingCards(AttackingCards);
		return;
	}
	CardsThatCanAttack=crdIDs;
	foreach(CardViewer* crd,CreaturesControlledView[-1]){
		if(crdIDs.contains(crd->GetCardToDisplay()->GetCardID())){
			connect(crd,SIGNAL(clicked(int)),this,SLOT(NewAttacker(int)));
		}
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
		ManaSelectionMode(TheCard);
	}
	else if(TheCard->GetCardType().contains(Constants::CardTypes::Land)) emit WantPlayCard(crdID,QList<int>());
}
void BattleGround::ManaSelectionMode(Card* const& TheCard){
	if (TheCard->GetConvertedManaCost()==0){
		emit WantPlayCard(CardRequiringMana->GetCardID(),CardsUsedToPay);
		return;
	}
	ManaSelectionModeON=true;
	foreach(CardViewer* crd,CreaturesControlledView[-1]){
		if(!crd) continue;
		if(crd->GetCardToDisplay()->IsManaSource() && (!crd->GetCardToDisplay()->IsTapped()))
			connect(crd,SIGNAL(clicked(int)),this,SLOT(NewManaPayed(int)));
	}
	foreach(CardViewer* crd,LandsControlledView[-1]){
		if(!crd) continue;
		if(crd->GetCardToDisplay()->IsManaSource() && (!crd->GetCardToDisplay()->IsTapped()))
			connect(crd,SIGNAL(clicked(int)),this,SLOT(NewManaPayed(int)));
	}
	ManaToTap.clear();
	CardsUsedToPay.clear();
	CardRequiringMana=TheCard;
	RequiredCostLabel->SetCostMap(TheCard->GetCardCost());
	RequiredCostFrame->show();
	RequiredCostFrame->raise();
	PayedCostLabel->SetCostMap(QMap<int,int>());
	PayedCostFrame->show();
	PayedCostFrame->raise();
	QuestionLowText->setText(tr("Select mana sources to pay for the spell cost"));
	QuestionLowButton1->setText(tr("Cancel","Button that abort the paying of mana"));
	connect(QuestionLowButton1,SIGNAL(clicked()),this,SLOT(CancelManaSelectionMode()));
	connect(QuestionLowButton1,SIGNAL(clicked()),this,SLOT(ClearLowerQuestion()));
	QuestionLowFrame->show();
	QuestionLowFrame->raise();
	SizePosCosts();
	UpdateAspect();
}
void BattleGround::NewManaPayed(int crdID){
	if((!ManaSelectionModeON) || (!CardRequiringMana)) return;
	const QList<Effect*>& TempList=AllCards[crdID]->GetEffects();
	for(QList<Effect*>::const_iterator i=TempList.constBegin();i!=TempList.constEnd();i++){
		if((*i)->GetManaEffect()){
			switch((*i)->GetEffectBody()){
			case EffectsConstants::Effects::AddWToManaPool: ManaToTap[Constants::ManaCosts::W]+=(*i)->GetVariableValues().at(0)*(CardsUsedToPay.contains(crdID) ? -1:1); break;
			case EffectsConstants::Effects::AddUToManaPool: ManaToTap[Constants::ManaCosts::U]+=(*i)->GetVariableValues().at(0)*(CardsUsedToPay.contains(crdID) ? -1:1); break;
			case EffectsConstants::Effects::AddBToManaPool: ManaToTap[Constants::ManaCosts::B]+=(*i)->GetVariableValues().at(0)*(CardsUsedToPay.contains(crdID) ? -1:1); break;
			case EffectsConstants::Effects::AddRToManaPool: ManaToTap[Constants::ManaCosts::R]+=(*i)->GetVariableValues().at(0)*(CardsUsedToPay.contains(crdID) ? -1:1); break;
			case EffectsConstants::Effects::AddGToManaPool: ManaToTap[Constants::ManaCosts::G]+=(*i)->GetVariableValues().at(0)*(CardsUsedToPay.contains(crdID) ? -1:1); break;
			}
			if (CardsUsedToPay.contains(crdID)){
				CardsUsedToPay.removeAt(CardsUsedToPay.indexOf(crdID));
			}
			else CardsUsedToPay.append(crdID);
			PayedCostLabel->SetCostMap(ManaToTap);
			if (CheckPayedCard(CardRequiringMana->GetCardCost(),ManaToTap)){
				emit WantPlayCard(CardRequiringMana->GetCardID(),CardsUsedToPay);
				CancelManaSelectionMode();
				ClearLowerQuestion();
			}
			return UpdateAspect();
		}
	}
	UpdateAspect();
}
void BattleGround::NewAttackTaget(int trgID){
	if(AttackingCards.key(NeedsATarget,-1)==-1) return;
	AttackingCards[AttackingCards.key(NeedsATarget)]=trgID;
	CancelAttackTaget();
}
void BattleGround::CancelManaSelectionMode(){
	ManaSelectionModeON=false;
	ManaToTap.clear();
	CardsUsedToPay.clear();
	CardRequiringMana=NULL;
	PayedCostFrame->hide();
	RequiredCostFrame->hide();
	foreach(CardViewer* crd,CreaturesControlledView[-1])
		disconnect(crd,SIGNAL(clicked(int)));
	foreach(CardViewer* crd,LandsControlledView[-1])
		disconnect(crd,SIGNAL(clicked(int)));
	UpdateAspect();
}
void BattleGround::NewAttacker(int crdID){
	if(AttackingCards.contains(crdID)){
		AllCards[crdID]->SetAttacking(false);
		AttackingCards.remove(crdID);
		if(AttackingCards.isEmpty())
			PhaseDisp->SetButtonString(tr("Skip Attack"));
		return UpdateAspect();
	}
	AllCards[crdID]->SetAttacking(true);
	AttackingCards.insert(crdID,NeedsATarget);
	bool HasPlaneswalkers=false;
	if(PlayersOrder.size()==2){
		int OpponentID=-1;
		foreach(const int& plID,PlayersOrder){
			if(plID!=-1){
				OpponentID=plID;
				break;
			}
		}
		const QList<Card*>& TmpCrdLst=CardsControlled.value(OpponentID);
		for(QList<Card*>::const_iterator i=TmpCrdLst.constBegin();i!=TmpCrdLst.constEnd() && (!HasPlaneswalkers);i++){
			if((*i)->GetCardType().contains(Constants::CardTypes::Planeswalker))
				HasPlaneswalkers=true;
		}
		if(!HasPlaneswalkers){
			AttackingCards[crdID]=-OpponentID;
			PhaseDisp->SetButtonString(tr("Attack"));
			return UpdateAspect();
		}
	}
	if(PlayersOrder.size()>2 || HasPlaneswalkers){
		AttackTargetsSelectionMode();
	}
	return UpdateAspect();
}
void BattleGround::AttackTargetsSelectionMode(){
	AttackTargetsSelectionModeON=true;
	PhaseDisp->DisableButton();
	AvailableTargetCards.clear();
	for(QMap<int,QList<Card*>>::const_iterator i=CardsControlled.constBegin();i!=CardsControlled.constEnd();i++){
		if(i.key()==-1) continue;
		const QList<Card*>& tmpList=i.value();
		foreach(Card* const& crd,tmpList){
			if(crd->GetCardType().contains(Constants::CardTypes::Planeswalker)){
				AvailableTargetCards.append(crd->GetCardID());
				foreach(CardViewer* j,CreaturesControlledView[i.key()]){
					if(j->GetCardToDisplay()->GetCardID()==crd->GetCardID()){
						connect(j,SIGNAL(clicked(int)),this,SLOT(NewAttackTaget(int)));
						break;
					}
				}
			}
		}
		connect(PlayesInfos[i.key()],SIGNAL(AvatarClicked(int)),this,SLOT(NewAttackTaget(int)));
	}
	
}

void BattleGround::CancelAttackTaget(){
	AttackTargetsSelectionModeON=false;
	PhaseDisp->EnableButton();
	AvailableTargetCards.clear();
	for(QList<int>::const_iterator i=PlayersOrder.constBegin();i!=PlayersOrder.constEnd();i++){
		if(*i==-1) continue;
		foreach(CardViewer* j,CreaturesControlledView[*i]){
			disconnect(j,SIGNAL(clicked(int)));
		}
		disconnect(PlayesInfos[*i],SIGNAL(AvatarClicked(int)));
	}
}
void BattleGround::SetAttackingCards(QHash<int,int> crdIDs){
	AttackingCards.clear();
	for(QHash<int,int>::const_iterator i=crdIDs.constBegin();i!=crdIDs.constEnd();i++){
		AllCards[i.key()]->SetAttacking(true);
		AttackingCards.insert(i.key(),i.value());
	}
	UpdateAspect();
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
	for(QList<Card*>::iterator i=CardsInStack.begin();i!=CardsInStack.end();i++){
		if((*i)->GetCardID()==crd){
			CardsInStack.erase(i);
			break;
		}
	}
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
int BattleGround::NumberOfCreatures(int who){
	int Result=0;
	foreach(Card* crd,CardsControlled[who]){
		if(crd->GetCardType().contains(Constants::CardTypes::Creature))
			++Result;
	}
	return Result;
}
bool BattleGround::CheckPayedCard(const QMap<int,int>& ManaCost,const QMap<int,int>& ManaPay) const{
	QMap<int,int> ManAv(ManaPay);
	int index;
	int TempValue=ManAv.value(Constants::ManaCosts::WU);
	if(TempValue>0){
		ManAv[Constants::ManaCosts::WU]=0;
		for(index=0;index<=TempValue;index++){
			ManAv[Constants::ManaCosts::W]+=TempValue-index;
			ManAv[Constants::ManaCosts::U]+=index;
			if (CheckPayedCard(ManaCost,ManAv)) return true;
			ManAv[Constants::ManaCosts::W]-=TempValue-index;
			ManAv[Constants::ManaCosts::U]-=index;
		}
		return false;
	}
	TempValue=ManAv.value(Constants::ManaCosts::WB);
	if(TempValue>0){
		ManAv[Constants::ManaCosts::WB]=0;
		for(index=0;index<=TempValue;index++){
			ManAv[Constants::ManaCosts::W]+=TempValue-index;
			ManAv[Constants::ManaCosts::B]+=index;
			if (CheckPayedCard(ManaCost,ManAv)) return true;
			ManAv[Constants::ManaCosts::W]-=TempValue-index;
			ManAv[Constants::ManaCosts::B]-=index;
		}
		return false;
	}
	TempValue=ManAv.value(Constants::ManaCosts::WR);
	if(TempValue>0){
		ManAv[Constants::ManaCosts::WR]=0;
		for(index=0;index<=TempValue;index++){
			ManAv[Constants::ManaCosts::W]+=TempValue-index;
			ManAv[Constants::ManaCosts::R]+=index;
			if (CheckPayedCard(ManaCost,ManAv)) return true;
			ManAv[Constants::ManaCosts::W]-=TempValue-index;
			ManAv[Constants::ManaCosts::R]-=index;
		}
		return false;
	}
	TempValue=ManAv.value(Constants::ManaCosts::WG);
	if(TempValue>0){
		ManAv[Constants::ManaCosts::WG]=0;
		for(index=0;index<=TempValue;index++){
			ManAv[Constants::ManaCosts::W]+=TempValue-index;
			ManAv[Constants::ManaCosts::G]+=index;
			if (CheckPayedCard(ManaCost,ManAv)) return true;
			ManAv[Constants::ManaCosts::W]-=TempValue-index;
			ManAv[Constants::ManaCosts::G]-=index;
		}
		return false;
	}
	TempValue=ManAv.value(Constants::ManaCosts::UB);
	if(TempValue>0){
		ManAv[Constants::ManaCosts::UB]=0;
		for(index=0;index<=TempValue;index++){
			ManAv[Constants::ManaCosts::U]+=TempValue-index;
			ManAv[Constants::ManaCosts::U]+=index;
			if (CheckPayedCard(ManaCost,ManAv)) return true;
			ManAv[Constants::ManaCosts::U]-=TempValue-index;
			ManAv[Constants::ManaCosts::B]-=index;
		}
		return false;
	}
	TempValue=ManAv.value(Constants::ManaCosts::UR);
	if(TempValue>0){
		ManAv[Constants::ManaCosts::UR]=0;
		for(index=0;index<=TempValue;index++){
			ManAv[Constants::ManaCosts::U]+=TempValue-index;
			ManAv[Constants::ManaCosts::R]+=index;
			if (CheckPayedCard(ManaCost,ManAv)) return true;
			ManAv[Constants::ManaCosts::U]-=TempValue-index;
			ManAv[Constants::ManaCosts::R]-=index;
		}
		return false;
	}
	TempValue=ManAv.value(Constants::ManaCosts::UG);
	if(TempValue>0){
		ManAv[Constants::ManaCosts::UG]=0;
		for(index=0;index<=TempValue;index++){
			ManAv[Constants::ManaCosts::U]+=TempValue-index;
			ManAv[Constants::ManaCosts::G]+=index;
			if (CheckPayedCard(ManaCost,ManAv)) return true;
			ManAv[Constants::ManaCosts::U]-=TempValue-index;
			ManAv[Constants::ManaCosts::G]-=index;
		}
		return false;
	}
	TempValue=ManAv.value(Constants::ManaCosts::BR);
	if(TempValue>0){
		ManAv[Constants::ManaCosts::BR]=0;
		for(index=0;index<=TempValue;index++){
			ManAv[Constants::ManaCosts::B]+=TempValue-index;
			ManAv[Constants::ManaCosts::R]+=index;
			if (CheckPayedCard(ManaCost,ManAv)) return true;
			ManAv[Constants::ManaCosts::B]-=TempValue-index;
			ManAv[Constants::ManaCosts::R]-=index;
		}
		return false;
	}
	TempValue=ManAv.value(Constants::ManaCosts::BG);
	if(TempValue>0){
		ManAv[Constants::ManaCosts::BG]=0;
		for(index=0;index<=TempValue;index++){
			ManAv[Constants::ManaCosts::B]+=TempValue-index;
			ManAv[Constants::ManaCosts::G]+=index;
			if (CheckPayedCard(ManaCost,ManAv)) return true;
			ManAv[Constants::ManaCosts::B]-=TempValue-index;
			ManAv[Constants::ManaCosts::G]-=index;
		}
		return false;
	}
	TempValue=ManAv.value(Constants::ManaCosts::RG);
	if(TempValue>0){
		ManAv[Constants::ManaCosts::RG]=0;
		for(index=0;index<=TempValue;index++){
			ManAv[Constants::ManaCosts::R]+=TempValue-index;
			ManAv[Constants::ManaCosts::G]+=index;
			if (CheckPayedCard(ManaCost,ManAv)) return true;
			ManAv[Constants::ManaCosts::R]-=TempValue-index;
			ManAv[Constants::ManaCosts::G]-=index;
		}
		return false;
	}

	//Linear trials of combinations of bicolor mana cost
	QMap<int,int> TmpCard(ManaCost);
	TempValue=TmpCard.value(Constants::ManaCosts::WU);
	if(TempValue>0){
		TmpCard[Constants::ManaCosts::WU]=0;
		for(index=0;index<=TempValue;index++){
			TmpCard[Constants::ManaCosts::W]+=TempValue-index;
			TmpCard[Constants::ManaCosts::U]+=index;
			if (CheckPayedCard(TmpCard,ManAv)) return true;
			TmpCard[Constants::ManaCosts::W]-=TempValue-index;
			TmpCard[Constants::ManaCosts::U]-=index;
		}
		return false;
	}
	TempValue=TmpCard.value(Constants::ManaCosts::WB);
	if(TempValue>0){
		TmpCard[Constants::ManaCosts::WB]=0;
		for(index=0;index<=TempValue;index++){
			TmpCard[Constants::ManaCosts::W]+=TempValue-index;
			TmpCard[Constants::ManaCosts::B]+=index;
			if (CheckPayedCard(TmpCard,ManAv)) return true;
			TmpCard[Constants::ManaCosts::W]-=TempValue-index;
			TmpCard[Constants::ManaCosts::B]-=index;
		}
		return false;
	}
	TempValue=TmpCard.value(Constants::ManaCosts::WR);
	if(TempValue>0){
		TmpCard[Constants::ManaCosts::WR]=0;
		for(index=0;index<=TempValue;index++){
			TmpCard[Constants::ManaCosts::W]+=TempValue-index;
			TmpCard[Constants::ManaCosts::R]+=index;
			if (CheckPayedCard(TmpCard,ManAv)) return true;
			TmpCard[Constants::ManaCosts::W]-=TempValue-index;
			TmpCard[Constants::ManaCosts::R]-=index;
		}
		return false;
	}
	TempValue=TmpCard.value(Constants::ManaCosts::WG);
	if(TempValue>0){
		TmpCard[Constants::ManaCosts::WG]=0;
		for(index=0;index<=TempValue;index++){
			TmpCard[Constants::ManaCosts::W]+=TempValue-index;
			TmpCard[Constants::ManaCosts::G]+=index;
			if (CheckPayedCard(TmpCard,ManAv)) return true;
			TmpCard[Constants::ManaCosts::W]-=TempValue-index;
			TmpCard[Constants::ManaCosts::G]-=index;
		}
		return false;
	}
	TempValue=TmpCard.value(Constants::ManaCosts::UB);
	if(TempValue>0){
		TmpCard[Constants::ManaCosts::UB]=0;
		for(index=0;index<=TempValue;index++){
			TmpCard[Constants::ManaCosts::U]+=TempValue-index;
			TmpCard[Constants::ManaCosts::B]+=index;
			if (CheckPayedCard(TmpCard,ManAv)) return true;
			TmpCard[Constants::ManaCosts::U]-=TempValue-index;
			TmpCard[Constants::ManaCosts::B]-=index;
		}
		return false;
	}
	TempValue=TmpCard.value(Constants::ManaCosts::UR);
	if(TempValue>0){
		TmpCard[Constants::ManaCosts::UR]=0;
		for(index=0;index<=TempValue;index++){
			TmpCard[Constants::ManaCosts::U]+=TempValue-index;
			TmpCard[Constants::ManaCosts::R]+=index;
			if (CheckPayedCard(TmpCard,ManAv)) return true;
			TmpCard[Constants::ManaCosts::U]-=TempValue-index;
			TmpCard[Constants::ManaCosts::R]-=index;
		}
		return false;
	}
	TempValue=TmpCard.value(Constants::ManaCosts::UG);
	if(TempValue>0){
		TmpCard[Constants::ManaCosts::UG]=0;
		for(index=0;index<=TempValue;index++){
			TmpCard[Constants::ManaCosts::U]+=TempValue-index;
			TmpCard[Constants::ManaCosts::G]+=index;
			if (CheckPayedCard(TmpCard,ManAv)) return true;
			TmpCard[Constants::ManaCosts::U]-=TempValue-index;
			TmpCard[Constants::ManaCosts::G]-=index;
		}
		return false;
	}
	TempValue=TmpCard.value(Constants::ManaCosts::BR);
	if(TempValue>0){
		TmpCard[Constants::ManaCosts::BR]=0;
		for(index=0;index<=TempValue;index++){
			TmpCard[Constants::ManaCosts::B]+=TempValue-index;
			TmpCard[Constants::ManaCosts::R]+=index;
			if (CheckPayedCard(TmpCard,ManAv)) return true;
			TmpCard[Constants::ManaCosts::B]-=TempValue-index;
			TmpCard[Constants::ManaCosts::R]-=index;
		}
		return false;
	}
	TempValue=TmpCard.value(Constants::ManaCosts::BG);
	if(TempValue>0){
		TmpCard[Constants::ManaCosts::BG]=0;
		for(index=0;index<=TempValue;index++){
			TmpCard[Constants::ManaCosts::B]+=TempValue-index;
			TmpCard[Constants::ManaCosts::G]+=index;
			if (CheckPayedCard(TmpCard,ManAv)) return true;
			TmpCard[Constants::ManaCosts::B]-=TempValue-index;
			TmpCard[Constants::ManaCosts::G]-=index;
		}
		return false;
	}
	TempValue=TmpCard.value(Constants::ManaCosts::RG);
	if(TempValue>0){
		TmpCard[Constants::ManaCosts::RG]=0;
		for(index=0;index<=TempValue;index++){
			TmpCard[Constants::ManaCosts::R]+=TempValue-index;
			TmpCard[Constants::ManaCosts::G]+=index;
			if (CheckPayedCard(TmpCard,ManAv)) return true;
			TmpCard[Constants::ManaCosts::R]-=TempValue-index;
			TmpCard[Constants::ManaCosts::G]-=index;
		}
		return false;
	}

	//Deal with elementary mana
	if(ManaCost.value(Constants::ManaCosts::W)>ManAv.value(Constants::ManaCosts::W)) return false;
	ManAv[Constants::ManaCosts::W]-=ManaCost.value(Constants::ManaCosts::W);
	if(ManaCost.value(Constants::ManaCosts::W)>ManAv.value(Constants::ManaCosts::U)) return false;
	ManAv[Constants::ManaCosts::U]-=ManaCost.value(Constants::ManaCosts::U);
	if(ManaCost.value(Constants::ManaCosts::W)>ManAv.value(Constants::ManaCosts::B)) return false;
	ManAv[Constants::ManaCosts::B]-=ManaCost.value(Constants::ManaCosts::B);
	if(ManaCost.value(Constants::ManaCosts::W)>ManAv.value(Constants::ManaCosts::R)) return false;
	ManAv[Constants::ManaCosts::R]-=ManaCost.value(Constants::ManaCosts::R);
	if(ManaCost.value(Constants::ManaCosts::G)>ManAv.value(Constants::ManaCosts::G)) return false;
	ManAv[Constants::ManaCosts::G]-=ManaCost.value(Constants::ManaCosts::G);
	if(ManaCost.value(Constants::ManaCosts::Colorless)>
		ManAv.value(Constants::ManaCosts::W)+
		ManAv.value(Constants::ManaCosts::U)+
		ManAv.value(Constants::ManaCosts::B)+
		ManAv.value(Constants::ManaCosts::R)+
		ManAv.value(Constants::ManaCosts::G)+
		ManAv.value(Constants::ManaCosts::Colorless)
		)return false;
	return true;
}