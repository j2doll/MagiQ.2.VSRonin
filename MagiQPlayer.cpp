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