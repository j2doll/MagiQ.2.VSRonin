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
BattleGround::BattleGround(QWidget* parent)
	:QWidget(parent)
	,GameStarted(false)
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
	Montagna.AddAvailableBackground(Constants::CardBacksrounds::LandRed);
	Montagna.AddAvailableEditions(Constants::Editions::AVR);
	Montagna.AddCardType(Constants::CardTypes::Basic);
	Montagna.AddCardType(Constants::CardTypes::Land);
	Montagna.AddCardSubType(Constants::CardSubTypes::Mountain);
	QList<CardData> Grimorio;
	for(int i=0;i<60;i++) Grimorio.append(Montagna);
	Fantoccio.SetLibrary(Grimorio);
	AddPlayer(Fantoccio);
	AddPlayer(Fantoccio);

	QFrame* Proviamo=new QFrame;
	Proviamo->show();
	Proviamo->setMinimumSize(500,289);
	HandLayout* TestLay=new HandLayout(Proviamo);
	TestLay->setSpacing(10);
	TestLay->setMargin(5);
	for (int i=0;i<7;i++){
		QLabel* tmplab=new QLabel(Proviamo);
		tmplab->setStyleSheet("border-image: url(:/CardImage/Rear.png)");
		tmplab->setMinimumSize(200,279);
		TestLay->addWidget(tmplab);
	}
	StartGame();
}
void BattleGround::resizeEvent(QResizeEvent* event){
	if (!GameStarted) return;
	switch (GetNumOfPlayers()){
		case 2:
			DeckLabels.at(0)->setGeometry(915*width()/1024,418*height()/768,60*width()/1024,83*height()/768);
			DeckLabels.at(1)->setGeometry(48*width()/1024,268*height()/768,60*width()/1024,83*height()/768);
			break;
	}
}
void BattleGround::UpdateAspect(){
	for (int i=0;i<Players.size();i++){
		if (Players.at(i)->GetLibrary().size()==0) DeckLabels.at(i)->hide();
		else DeckLabels.at(i)->setText(QString("<font color=\"white\" size=\"%2\">%1</font>").arg(Players.at(i)->GetLibrary().size()).arg(12));
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
		Hands.append(new QFrame(this));
		Hands.last()->setObjectName("Hand");
		Players.at(i)->ShuffleLibrary();
		for (int j=0;j<7;j++)
			Players.at(i)->DrawCard();
	}
	UpdateAspect();
	GameStarted=true;
	show();
}

