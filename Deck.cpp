#include "Deck.h"
CardDeck::CardDeck(QObject* parent)
	:QObject(parent)
{}
CardDeck::CardDeck(const CardDeck& a,QObject* parent)
	:QObject(parent)
	,MainBoard(a.MainBoard)
	,SideBoard(a.SideBoard)
{}
void CardDeck::AddCard(const CardData& c){
	MainBoard.append(c);
}
void CardDeck::AddSideboard(const CardData& c){
	SideBoard.append(c);
}
QList<int> CardDeck::Legality() const{
	QList<int> Result;
	if (NumMain()<60 || (NumSide()!=0 && NumSide()!=15)) return Result;
	for (int i=0;i<Constants::Legality::END_Legal;i++){
		Result.append(i);
	}
	//TODO Controlla le Edizioni e le carte
	return Result;
}
bool CardDeck::IsCertified() const{
	foreach(CardData card,MainBoard){
		if(!card.GetCertified()) return false;
	}
	foreach(CardData card,SideBoard){
		if(!card.GetCertified()) return false;
	}
	return true;
}
QMap<int,int> CardDeck::ManaBreakDown(){
	QMap<int,int> Result;
	foreach(CardData card,MainBoard){
		Result[card.GetConvertedManaCost()]=
			Result.value(card.GetConvertedManaCost(),0)+1;
	}
	return Result;
}