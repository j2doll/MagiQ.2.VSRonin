#include "BattleGround.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include "MagiQPlayer.h"
#include "StyleSheets.h"
#include "SizeSliders.h"
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
	AddPlayer(MagiQPlayer());
	AddPlayer(MagiQPlayer());
	
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
	while(DeckLabels.size()<Players.size()){
		DeckLabels.append(new QLabel(this));
		DeckLabels.last()->setObjectName("DeckLabel");
	}
	UpdateAspect();
	GameStarted=true;
	show();
}

