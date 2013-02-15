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
	,HasFinishedTimer(false)
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
	for (QList<CardData>::const_iterator i=Library.constBegin();i!=Library.constEnd();i++)
		i->SetSortingMethod(CardData::Random);
	qSort(Library.begin(),Library.end());
}
void MagiQPlayer::RemoveTopLibrary(){
	if (Library.isEmpty())
		emit NoMoreCardsToDraw();
	Library.takeAt(0);
}
const CardData& MagiQPlayer::DrawCard(){
	if (Library.isEmpty()){
		emit NoMoreCardsToDraw();
		return Hand.last();
	}
	Hand.append(Library.takeFirst());
	emit CardDrawn();
	return Hand.last();
}

void MagiQPlayer::SortHand(){
	for (QList<CardData>::const_iterator i=Hand.constBegin();i!=Hand.constEnd();i++)
		i->SetSortingMethod(SecondaryOrdering);
	qSort(Hand.begin(),Hand.end());
	for (QList<CardData>::const_iterator i=Hand.constBegin();i!=Hand.constEnd();i++)
		i->SetSortingMethod(PrimaryOrdering);
	qStableSort(Hand.begin(),Hand.end());
}
void MagiQPlayer::SortControlled(){
	for (QList<CardData>::const_iterator i=ControlledCards.constBegin();i!=ControlledCards.constEnd();i++)
		i->SetSortingMethod(CardData::ByName);
	qSort(ControlledCards.begin(),ControlledCards.end());
	for (QList<CardData>::const_iterator i=ControlledCards.constBegin();i!=ControlledCards.constEnd();i++)
		i->SetSortingMethod(CardData::ByType);
	qStableSort(ControlledCards.begin(),ControlledCards.end());
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
CardData MagiQPlayer::RemoveFromHand(int CardID){
	CardData Result;
	QList<CardData>::iterator Alternative=Hand.end();
	for(QList<CardData>::iterator i=Hand.begin();i!=Hand.end();i++){
		if(i->GetCardID()==CardID){
			Result=*i;
			Hand.erase(i);
			return Result;
		}
		if (i->GetCardID()==0) Alternative=i;
	}
	if (Alternative!=Hand.end()){
		Result=*Alternative;
		Hand.erase(Alternative);
	}
	return Result;
}
void MagiQPlayer::TapCard(int CardID,bool Tapped){
	for(QList<CardData>::iterator i=ControlledCards.begin();i!=ControlledCards.end();i++){
		if(i->GetCardID()==CardID) return i->SetTapped(Tapped);
	}
}