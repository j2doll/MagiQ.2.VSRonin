#include "Deck.h"
CardDeck::CardDeck(QObject* parent/* =0 */)
	:QObject(parent)
{
	
}
void CardDeck::AddCard(const Card& c){
	MainBoard.append(new Card(c));
}
void CardDeck::AddSideboard(const Card& c){
	SideBoard.append(new Card(c));
}
QList<int> CardDeck::Legality() const{
	QList<int> Result;
	if (NumMain()<60 || (NumSide()!=0 && NumSide()!=15)) return Result;
	for (int i=0;i<END_Legal;i++){
		Result.append(i);
	}
	//TODO Controlla le Edizioni e le carte
	return Result;
}