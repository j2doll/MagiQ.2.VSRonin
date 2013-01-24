#include "BattleGround.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QResizeEvent>
#include "MagiQPlayer.h"
#include "StyleSheets.h"
#include "SizeSliders.h"
#include "CostantsDefinition.h"
#include "HandLayout.h"
#include "CardViewer.h"
#include "Card.h"
BattleGround::BattleGround(QWidget* parent)
	:QWidget(parent)
	,LibaryCountFontSize(12)
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

	TestStuff();
}
#include "Deck.h" //test
void BattleGround::TestStuff(){
	CardData Montagna;
	Montagna.SetCardName("Mountain");
	Montagna.SetHasManaCost(false);
	Montagna.SetAvailableBackgrounds(Constants::CardBacksrounds::LandRed);
	Montagna.SetCardBackground(Constants::CardBacksrounds::LandRed);
	Montagna.AddAvailableEditions(Constants::Editions::AVR);
	Montagna.AddCardType(Constants::CardTypes::Basic);
	Montagna.AddCardType(Constants::CardTypes::Land);
	Montagna.AddCardSubType(Constants::CardSubTypes::Mountain);
	CardDeck Grimorio;
	for(int i=0;i<60;i++) Grimorio.AddCard(Montagna);
	
	UpdateAspect();
}
void BattleGround::resizeEvent(QResizeEvent* event){
	if (!isVisible()) return;
	int NextIndex=-1;
	switch (Players.size()){
		case 2:
			DeckLabels.value(NextIndex)->setGeometry(915*width()/1024,418*height()/768,60*width()/1024,83*height()/768);
			HandFrames.value(NextIndex)->setGeometry(
				(width()-qMin(HandsLay.value(-1)->sizeHint().width(),1000*width()/1024))/2,
				661*height()/768,
				qMin(HandsLay.value(-1)->sizeHint().width(),1000*width()/1024),
				92*height()/768);
			if(PlayersOrder.indexOf(NextIndex)+1>=PlayersOrder.size())
				NextIndex=PlayersOrder.value(0);
			else
				NextIndex=PlayersOrder.value(PlayersOrder.indexOf(NextIndex)+1);
			DeckLabels.value(NextIndex)->setGeometry(48*width()/1024,268*height()/768,60*width()/1024,83*height()/768);
			HandFrames.value(NextIndex)->setGeometry(
				(width()-qMin(HandsLay.value(NextIndex)->sizeHint().width(),1000*width()/1024))/2,
				10*height()/768,
				qMin(HandsLay.value(NextIndex)->sizeHint().width(),1000*width()/1024),
				92*height()/768);
			break;
	}
}
void BattleGround::UpdateAspect(){
	if (!isVisible()) return;
	SortCardsInHand();
	for(QMap<int,MagiQPlayer*>::iterator index=Players.begin();index!=Players.end();index++){
		if (index.value()->GetLibrary().size()==0)
			DeckLabels.value(index.key())->hide();
		else{
			DeckLabels.value(index.key())->show();
			DeckLabels.value(index.key())->setText(QString("<font color=\"white\" size=\"%2\">%1</font>").arg(index.value()->GetLibrary().size()).arg(LibaryCountFontSize));
		}
		while(index.value()->GetHand().size()>CardsInHandView.value(index.key()).size()){
			CardsInHandView[index.key()].append(new CardViewer(this));
			CardViewer* TempViewer=CardsInHandView[index.key()].last();
			connect(TempViewer,SIGNAL(LeftFocus()),this,SLOT(ResetHandOrder()));
			connect(TempViewer,SIGNAL(GainFocus()),TempViewer,SLOT(raise()),Qt::QueuedConnection);
			HandsLay.value(index.key())->addWidget(TempViewer);
		}
		while(index.value()->GetHand().size()<CardsInHandView.value(index.key()).size()){
			CardsInHandView[index.key()].takeFirst()->deleteLater();
		}
		for (int j=0;j<index.value()->GetHand().size();j++){
			if (index.key()==-1)
				//CardsInHandView[index.key()].at(j)->SetCardToDisplay(index.value()->GetHand().value(j));
				CardsInHandView[index.key()].at(j)->SetCardToDisplay(CardsInHand.value(j));
			else
				CardsInHandView[index.key()].at(j)->SetCardToDisplay(GenericCard);
			CardsInHandView[index.key()].at(j)->UpdateAspect();
		}
	}
	setStyleSheet(StyleSheets::BoardCSS);
	resizeEvent(&QResizeEvent(QSize(1,1),QSize(1,1)));
}
void BattleGround::SortCardsInHand(){
	Players.value(-1)->SortHand();
	const QList<CardData>& TempList=Players.value(-1)->GetHand();
	int index;
	QMap<int,Card*> Sorter;
	foreach(Card* car,CardsInHand){
		index=-1;
		for(int j=0;j<TempList.size() && index<0;j++){
			if(TempList.value(j).GetCardID()==car->GetCardID()) index=j;
		}
		Sorter.insert(index,car);
	}
	CardsInHand=Sorter.values();
}
void BattleGround::ResetHandOrder(){
	foreach(HandLayout* hanlay,HandsLay)
		hanlay->invalidate();
}
void BattleGround::SetPlayersOrder(QList<int> ord){
	PlayersOrder.clear(); PlayersOrder=ord;
	foreach(int PlayID,PlayersOrder){
		Players[PlayID]=new MagiQPlayer(this);
		HandFrames[PlayID]=new QFrame(this);
		HandsLay[PlayID]=new HandLayout(HandFrames[PlayID]);
		DeckLabels[PlayID]=new QLabel(this);
		DeckLabels[PlayID]->setObjectName("DeckLabel");
		DeckLabels[PlayID]->setAlignment(Qt::AlignCenter);
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
	while (!CardsInHand.isEmpty()) CardsInHand.takeAt(0)->deleteLater();
	foreach(CardData crd,Players[-1]->GetHand()){
		CardsInHand.append(new Card(crd,this));
		CardsInHand.last()->UpdateAspect();
		CardsInHand.last()->hide();
	}
	UpdateAspect();
}
void BattleGround::SetOtherHand(int whos,int numcards){
	for(int i=0;i<numcards;i++) Players[whos]->AddHand(CardData());
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
