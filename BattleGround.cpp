#include "BattleGround.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include "MagiQPlayer.h"
#include "StyleSheets.h"
#include "SizeSliders.h"
#include "CostantsDefinition.h"
#include "HandLayout.h"
#include "CardViewer.h"
#include "Card.h"
BattleGround::BattleGround(QWidget* parent)
	:QWidget(parent)
	,GameStarted(false)
	,ScheduledRaise(NULL)
{
	setMinimumSize(1024,768);
	Board=new QFrame(this);
	Board->setObjectName("Board");
	QHBoxLayout* MainLayout=new QHBoxLayout(this);
	MainLayout->setMargin(0);
	MainLayout->setSpacing(0);
	MainLayout->addWidget(Board);
	TestStuff();
	hide();
}
void BattleGround::TestStuff(){
	MagiQPlayer Fantoccio;
	CardData Montagna;
	Montagna.SetCardName("Mountain");
	Montagna.SetHasManaCost(false);
	Montagna.SetAvailableBackgrounds(Constants::CardBacksrounds::LandRed);
	Montagna.SetCardBackground(Constants::CardBacksrounds::LandRed);
	Montagna.AddAvailableEditions(Constants::Editions::AVR);
	Montagna.AddCardType(Constants::CardTypes::Basic);
	Montagna.AddCardType(Constants::CardTypes::Land);
	Montagna.AddCardSubType(Constants::CardSubTypes::Mountain);
	QList<CardData> Grimorio;
	for(int i=0;i<60;i++) Grimorio.append(Montagna);
	Fantoccio.SetLibrary(Grimorio);
	AddPlayer(Fantoccio);
	AddPlayer(Fantoccio);
	StartGame();
	foreach(MagiQPlayer* playe,Players){
		for(int i=0;i<40;i++) playe->DrawCard();
	}
	UpdateAspect();
}
void BattleGround::resizeEvent(QResizeEvent* event){
	if (!GameStarted) return;
	switch (GetNumOfPlayers()){
		case 2:
			DeckLabels.at(0)->setGeometry(915*width()/1024,418*height()/768,60*width()/1024,83*height()/768);
			DeckLabels.at(1)->setGeometry(48*width()/1024,268*height()/768,60*width()/1024,83*height()/768);
			HandFrames.at(0)->setGeometry(
				(width()-qMin(HandsLay.at(0)->sizeHint().width(),1000*width()/1024))/2,
				661*height()/768,
				qMin(HandsLay.at(0)->sizeHint().width(),1000*width()/1024),
				92*height()/768);
			HandFrames.at(1)->setGeometry(
				(width()-qMin(HandsLay.at(1)->sizeHint().width(),1000*width()/1024))/2,
				10*height()/768,
				qMin(HandsLay.at(1)->sizeHint().width(),1000*width()/1024),
				92*height()/768);
			break;
	}
}
void BattleGround::UpdateAspect(){
	for (int i=0;i<Players.size();i++){
		if (Players.at(i)->GetLibrary().size()==0) DeckLabels.at(i)->hide();
		else DeckLabels.at(i)->setText(QString("<font color=\"white\" size=\"%2\">%1</font>").arg(Players.at(i)->GetLibrary().size()).arg(12));
		Players.at(i)->SortHand();
		foreach(CardData card,Players.at(i)->GetHand()){
			Card* TempCard=new Card(card,this);
			TempCard->hide();
			CardViewer* TempViewer=new CardViewer(this);
			connect(TempViewer,SIGNAL(LeftFocus()),this,SLOT(ResetHandOrder()));
			connect(TempViewer,SIGNAL(GainFocus()),TempViewer,SLOT(raise()));
			TempViewer->SetCardToDisplay(TempCard);
			TempViewer->UpdateAspect();
			HandsLay.at(i)->addWidget(TempViewer);
		}
	}
	setStyleSheet(StyleSheets::BoardCSS);
}
void BattleGround::SetPlayers(){
	foreach(MagiQPlayer* player,Players){
		player->deleteLater();
	}
	Players.clear();
}
void BattleGround::SetPlayers(const QList<MagiQPlayer*>& a){
	SetPlayers();
	foreach(MagiQPlayer* player,a){
		Players.append(new MagiQPlayer(*player,this));
	}
}
void BattleGround::AddPlayer(const MagiQPlayer& a){
	Players.append(new MagiQPlayer(a,this));
}
void BattleGround::StartGame(){
	foreach(QLabel* lab,DeckLabels)
		lab->deleteLater();
	DeckLabels.clear();
	for (int i=0;i<Players.size();i++){
		DeckLabels.append(new QLabel(this));
		DeckLabels.last()->setObjectName("DeckLabel");
		DeckLabels.last()->setAlignment(Qt::AlignCenter);
		HandFrames.append(new QFrame(this));
		HandFrames.last()->setObjectName("HandFrame");
		HandsLay.append(new HandLayout(HandFrames.last()));
		HandsLay.last()->setMargin(2);
		HandsLay.last()->setSpacing(2);
	}
	UpdateAspect();
	GameStarted=true;
	show();
}
void BattleGround::ResetHandOrder(){
	foreach(HandLayout* hanlay,HandsLay)
		hanlay->invalidate();
	if (ScheduledRaise) ScheduledRaise->raise();
	ScheduledRaise=NULL;
}

