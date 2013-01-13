#ifndef DECK_H
#define DECK_H
#include <QObject>
#include <QList>
#include "CardData.h"
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
	void SetMainBoard(){MainBoard.clear();}
	void SetMainBoard(const QList<CardData>& a){MainBoard.clear(); MainBoard=a;}
	void SetMainBoard(const CardData& c){MainBoard.clear(); AddCard(c);}
	void AddSideboard(const CardData& c);
	void SetSideBoard(){SideBoard.clear();}
	void SetSideBoard(const QList<CardData>& a){SideBoard.clear(); SideBoard=a;}
	void SetSideBoard(const CardData& c){SideBoard.clear(); AddSideboard(c);}
	int NumMain() const {return MainBoard.size();}
	int NumSide() const {return SideBoard.size();}
	QList<int> Legality() const;
	bool IsCertified() const;
	QMap<int,int> ManaBreakDown();
	CardDeck SingleImagesDeck() const;
	const QList<CardData>& GetMainBoard() const{return MainBoard;}
	const QList<CardData>& GetSideBoard() const{return SideBoard;}
};
QDataStream &operator<<(QDataStream &out, const CardDeck &deck);
QDataStream &operator>>(QDataStream &in, CardDeck &deck);
#endif