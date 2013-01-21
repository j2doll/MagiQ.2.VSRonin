#include "Deck.h"
#include "CostantsDefinition.h"
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
	for (int i=0;i<Constants::Legality::END;i++){
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
CardDeck CardDeck::SingleImagesDeck() const{
	CardDeck Result(*this);
	foreach(CardData card,Result.MainBoard){
		if(card.GetAvailableImages().isEmpty()) continue;
		if (card.GetCardImage()<-1 || card.GetCardImage()>=card.GetAvailableImages().size())
			card.SetAvailableImages();
		else if (card.GetCardImage()==-1)
			card.SetAvailableImages(card.GetAvailableImages().at(qrand()%card.GetAvailableImages().size()));
		else
			card.SetAvailableImages(card.GetAvailableImages().at(card.GetCardImage()));
		card.SetCardImage(0);
	}
	foreach(CardData card,Result.SideBoard){
		if(card.GetAvailableImages().isEmpty()) continue;
		if (card.GetCardImage()<-1 || card.GetCardImage()>=card.GetAvailableImages().size())
			card.SetAvailableImages();
		else if (card.GetCardImage()==-1)
			card.SetAvailableImages(card.GetAvailableImages().at(qrand()%card.GetAvailableImages().size()));
		else
			card.SetAvailableImages(card.GetAvailableImages().at(card.GetCardImage()));
		card.SetCardImage(0);
	}
	return Result;
}
QDataStream& operator<<(QDataStream &out, const CardDeck &deck){out << deck.GetMainBoard() << deck.GetSideBoard(); return out;}
QDataStream& operator>>(QDataStream &in, CardDeck &deck){QList<CardData> TempList; in >> TempList; deck.SetMainBoard(TempList); TempList.clear(); in >> TempList; deck.SetSideBoard(TempList); return in;}