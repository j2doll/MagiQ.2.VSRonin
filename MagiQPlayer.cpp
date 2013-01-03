#include "MagiQPlayer.h"
#include "CostantsDefinition.h"
#include <QtAlgorithms>
#include "Card.h"
MagiQPlayer::MagiQPlayer(QObject* parent)
	:QObject(parent)
	,Life(20)
	,PrimaryOrdering(ByManaCost)
	,SecondaryOrdering(ByType)
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
bool MagiQPlayer::RandomOrder(Card* a,Card* b) {return qrand()%2==0;}
bool MagiQPlayer::ManaCostOrder(Card* a,Card* b) {return a->GetConvertedManaCost()<b->GetConvertedManaCost();}
bool MagiQPlayer::NameOrder(Card* a,Card* b) {return a->GetCardName()<b->GetCardName();}
bool MagiQPlayer::TypeOrder(Card* a,Card* b) {
	QList<int> TypeA=a->GetCardType();
	QList<int> TypeB=b->GetCardType();
	for (int i=0;i<Constants::CardTypes::END;i++){
		if (TypeA.contains(i)) return true;
		if (TypeB.contains(i)) return false;
	}
	return false;
}
bool MagiQPlayer::ColorOrder(Card* a,Card* b)  {
	QList<int> TypeA=a->GetCardColor();
	QList<int> TypeB=b->GetCardColor();
	for (int i=0;i<=Constants::CardColor::Green;i++){
		if (TypeA.contains(i)) return true;
		if (TypeB.contains(i)) return false;
	}
	return false;
}
void MagiQPlayer::ShuffleLibrary(){
	qSort(Library.begin(),Library.end(),RandomOrder);
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
	switch (SecondaryOrdering){
	case ByName:
		qSort(Hand.begin(),Hand.end(),NameOrder); break;
	case ByManaCost:
		qSort(Hand.begin(),Hand.end(),ManaCostOrder);
	case ByColor:
		qSort(Hand.begin(),Hand.end(),ColorOrder); break;
	case ByType:
	default:
		qSort(Hand.begin(),Hand.end(),TypeOrder); break;
	}
	switch (PrimaryOrdering){
		case ByName:
			qStableSort(Hand.begin(),Hand.end(),NameOrder); break;
		case ByType:
			qStableSort(Hand.begin(),Hand.end(),TypeOrder); break;
		case ByColor:
			qStableSort(Hand.begin(),Hand.end(),ColorOrder); break;
		case ByManaCost:
		default:
			qStableSort(Hand.begin(),Hand.end(),ManaCostOrder);
	}
	emit RequireUpdateAspect();
}