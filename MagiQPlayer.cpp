#include "MagiQPlayer.h"
#include "CostantsDefinition.h"
#include <QtAlgorithms>
#include "Card.h"
MagiQPlayer::MagiQPlayer(QObject* parent)
	:QObject(parent)
	,Life(20)
	,PrimaryOrdering(CardData::ByManaCost)
	,SecondaryOrdering(CardData::ByType)
	,CanPlayMana(true)
	,PlayerColor(0,0,0)
	,ReadyToStartMatch(false)
	,HasAcceptedHand(false)
	,Avatar(":/Board/DefaultAvatar.png")
{
	ResetManaPool();
}
void MagiQPlayer::ResetManaPool(){
	ManaPool.clear();
	ManaPool.insert(Constants::ManaCosts::Colorless,0);
	ManaPool.insert(Constants::ManaCosts::W,0);
	ManaPool.insert(Constants::ManaCosts::U,0);
	ManaPool.insert(Constants::ManaCosts::B,0);
	ManaPool.insert(Constants::ManaCosts::R,0);
	ManaPool.insert(Constants::ManaCosts::G,0);
	emit RequireUpdateAspect();
}

void MagiQPlayer::ShuffleLibrary(){
	foreach(CardData card,Library)
		card.SetSortingMethod(CardData::Random);
	qSort(Library.begin(),Library.end());
}
void MagiQPlayer::DrawCard(){
	if (Library.isEmpty()){
		emit NoMoreCardsToDraw();
		return;
	}
	Hand.append(Library.takeFirst());
	emit CardDrawn();
	SortHand();
}

void MagiQPlayer::SortHand(){
	foreach(CardData card,Hand)
		card.SetSortingMethod(SecondaryOrdering);
	qSort(Hand.begin(),Hand.end());
	foreach(CardData card,Hand)
		card.SetSortingMethod(PrimaryOrdering);
	qStableSort(Hand.begin(),Hand.end());
	emit RequireUpdateAspect();
}
MagiQPlayer::MagiQPlayer(const MagiQPlayer& a,QObject* parent)
	:QObject(parent)
	,Avatar(a.Avatar)
	,PlayerName(a.PlayerName)
	,Life(a.Life)
	,PrimaryOrdering(a.PrimaryOrdering)
	,SecondaryOrdering(a.SecondaryOrdering)
	,CanPlayMana(a.CanPlayMana)
	,ManaPool(a.ManaPool)
	,Library(a.Library)
	,Hand(a.Hand)
	,Graveyard(a.Graveyard)
	,Exile(a.Exile)
	,Sideboard(a.Sideboard)
	,ControlledCards(a.ControlledCards)
{}

const MagiQPlayer& MagiQPlayer::operator=(const MagiQPlayer& a){
	Avatar=a.Avatar;
	PlayerName=a.PlayerName;
	Life=a.Life;
	PrimaryOrdering=a.PrimaryOrdering;
	SecondaryOrdering=a.SecondaryOrdering;
	CanPlayMana=a.CanPlayMana;
	ManaPool=a.ManaPool;
	Library=a.Library;
	Hand=a.Hand;
	Graveyard=a.Graveyard;
	Exile=a.Exile;
	Sideboard=a.Sideboard;
	ControlledCards=a.ControlledCards;
	return *this;
}

void MagiQPlayer::SetLibrary(const CardDeck& a){
	CardDeck Temp(a.SingleImagesDeck());
	Library.clear();
	Sideboard.clear();
	Library=Temp.GetMainBoard();
	Sideboard=Temp.GetSideBoard();
}
void MagiQPlayer::SetPlayerColor(const QString& a){
	PlayerColor.setNamedColor(a);
	if(!PlayerColor.isValid()) PlayerColor.setRgb(0,0,0);
}
void MagiQPlayer::SetLibrary(){
	Library.clear();
}
void MagiQPlayer::SetLibrary(const QList<CardData>& a){
	Library.clear(); 
	Library=a;
}