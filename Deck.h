#ifndef DECK_H
#define DECK_H
#include <QObject>
#include <QList>
#include "Card.h"
class CardDeck: public QObject{
	Q_OBJECT
private:
	QList<Card*> MainBoard;
	QList<Card*> SideBoard;
public:
	CardDeck(QObject* parent=0);
	CardDeck(const CardDeck& a);
	CardDeck& operator<< (const Card& c){AddCard(c); return *this;}
	void AddCard(const Card& c);
	void AddSideboard(const Card& c);
	int NumMain() const {return MainBoard.size();}
	int NumSide() const {return SideBoard.size();}
	enum{Legacy,Modern,Standard,Extended,Commander,END_Legal};
	QList<int> Legality() const;
};
#endif