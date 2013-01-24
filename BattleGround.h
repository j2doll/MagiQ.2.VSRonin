#ifndef CAMPODIGIOCO_H
#define CAMPODIGIOCO_H
#include <QWidget>
#include <QList>
#include <QMap>
#include <QPixmap>
#include "CardData.h"
class HandLayout;
class MagiQPlayer;
class QFrame;
class QLabel;
class CardViewer;
class Card;
class BattleGround : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(int NumOfPlayers READ GetNumOfPlayers)
private:
	const int LibaryCountFontSize;
//Visual Elements
	QFrame* Board;
	QMap<int,QFrame*> HandFrames;
	QMap<int,HandLayout*> HandsLay;
	QMap<int,QLabel*> DeckLabels;
	QMap<int,QList<CardViewer*>> CardsInHandView;
	QList<Card*> CardsInHand;
	Card* GenericCard;
//Game Properties
	QMap<int,MagiQPlayer*> Players;
	QList<int> PlayersOrder;
//Functions
	void UpdateAspect();
	void SortCardsInHand();

	void TestStuff();
private slots:
	void ResetHandOrder();
public slots:
	void SetPlayersOrder(QList<int> ord);
	void SetPlayersNameAvatar(QMap<int,QString> nam,QMap<int,QPixmap> avt);
	void SetMyHand(QList<CardData> hnd);
	void SetOtherHand(int whos,int numcards);
	void SetMyLibrary(QList<CardData> libr);
	void SetOtherLibrary(int whos,int numcards);
public:
	BattleGround(QWidget* parent=0);
	int GetNumOfPlayers() const {return Players.size();}
protected:
	void resizeEvent(QResizeEvent* event);
};
#endif

