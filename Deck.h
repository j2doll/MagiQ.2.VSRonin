#ifndef DECK_H
#define DECK_H
#include <QObject>
#include <QList>
#include "CardData.h"
#include "CostantsDefinition.h"
class CardDeck: public QObject{
	Q_OBJECT
private:
	QList<CardData> MainBoard;
	QList<CardData> SideBoard;
public:
	CardDeck(QObject* parent=0);
	CardDeck(const CardDeck& a,QObject* parent=0);
	CardDeck& operator<< (const CardData& c){AddCard(c); return *this;}
	void AddCard(const CardData& c);
	void AddSideboard(const CardData& c);
	int NumMain() const {return MainBoard.size();}
	int NumSide() const {return SideBoard.size();}
	QList<int> Legality() const;
	bool IsCertified() const;
	QMap<int,int> ManaBreakDown();
};
#endif