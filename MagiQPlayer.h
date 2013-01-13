#ifndef PLAYER_H
#define PLAYER_H
#include <QObject>
#include <QMap>
#include <QPixmap>
#include "CardData.h"
class Card;
class MagiQPlayer : public QObject{
	Q_OBJECT
private:
	QPixmap Avatar;
	QString PlayerName;
	int Life;
	int PrimaryOrdering;
	int SecondaryOrdering;
	bool CanPlayMana;
	QMap<int,int> ManaPool;
	QList<CardData> Library;
	QList<CardData> Hand;
	QList<CardData> Graveyard;
	QList<CardData> Exile;
	QList<CardData> OwnedCards;
	QList<CardData> ControlledCards;
signals:
	void LifeChanged(int l);
	void CardDrawn();
	void NoMoreCardsToDraw();
	void RequireUpdateAspect();
public:
	void SetLibrary(const QList<CardData>& a){Library.clear(); Library=a;}
	const QList<CardData>& GetLibrary() const {return Library;}
	const QList<CardData>& GetHand() const {return Hand;}
	const QList<CardData>& GetGraveyard() const {return Graveyard;}
	const QList<CardData>& GetExile() const {return Exile;}
	const QList<CardData>& GetOwnedCards() const {return OwnedCards;}
	const QList<CardData>& GetControlledCards() const {return ControlledCards;}
	void SetPlayerName(const QString& a){PlayerName=a;}
	const QString& GetPlayerName() const {return PlayerName;}
	void SetAvatar(const QPixmap& a){Avatar=a;}
	const QPixmap& GetAvatar() const {return Avatar;}
	void SetPrimaryOrdering(int a){if (a>=CardData::ByName && a <=CardData::ByColor) PrimaryOrdering=a;}
	int GetPrimaryOrdering() const {return PrimaryOrdering;}
	void SetSecondaryOrdering(int a){if (a>=CardData::ByName && a <=CardData::ByColor) SecondaryOrdering=a;}
	int GetSecondaryOrdering() const {return SecondaryOrdering;}
	MagiQPlayer(QObject* parent=0);
	void SetLife(int a){Life=a; emit LifeChanged(Life);}
	void AddLife(int a){Life+=a; emit LifeChanged(Life);}
	int GetLife() const {return Life;}
	void ResetManaPool();
	void AddManaPool(int key, int number){ManaPool[key]+=number;}
	void SetManaPool(int key, int number){ManaPool[key]=number;}
	void SetManaPool(const QMap<int,int>& a) {ManaPool=a;}
	const QMap<int,int>& GetManaPool() const {return ManaPool;}
	void ShuffleLibrary();
	void DrawCard();
	void SortHand();
	void SetCanPlayMana(bool a){CanPlayMana=a;}
	bool GetCanPlayMana() const {return CanPlayMana;}
	MagiQPlayer(const MagiQPlayer& a,QObject* parent=0);
	const MagiQPlayer& operator=(const MagiQPlayer& a);
};
#endif