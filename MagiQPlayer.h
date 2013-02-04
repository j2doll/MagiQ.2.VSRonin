#ifndef PLAYER_H
#define PLAYER_H
#include <QObject>
#include <QMap>
#include <QPixmap>
#include "CardData.h"
#include "Deck.h"
class Card;
class MagiQPlayer : public QObject{
	Q_OBJECT
private:
	bool HasAcceptedHand;
	bool HasFinishedTimer;
	int PlayerID;
	bool ReadyToStartMatch;
	QPixmap Avatar;
	QString PlayerName;
	QColor PlayerColor; //Used in chat
	int Life;
	int PrimaryOrdering;
	int SecondaryOrdering;
	bool CanPlayMana;
	QMap<int,int> ManaPool;
	QList<CardData> Library;
	QList<CardData> Hand;
	QList<CardData> Graveyard;
	QList<CardData> Exile;
	QList<CardData> Sideboard;
	QList<CardData> ControlledCards;
signals:
	void LifeChanged(int l);
	void CardDrawn();
	void NoMoreCardsToDraw();
	void RequireUpdateAspect();
public:
	void SetControlledCards() {ControlledCards.clear();}
	void SetControlledCards(const QList<CardData>& a){ControlledCards.clear(); ControlledCards=a;}
	void AddControlledCard(const CardData& a){ControlledCards.append(a);}
	void SetControlledCards(const CardData& a){SetControlledCards(); AddControlledCard(a);}
	CardData RemoveFromHand(int CardID);
	void SetHasFinishedTimer(bool a){HasFinishedTimer=a;}
	bool GetHasFinishedTimer() const {return HasFinishedTimer;}
	void SetHand(const QList<CardData>& a){Hand.clear(); Hand=a;}
	void SetHand(){Hand.clear();}
	void SetHand(const CardData& a){SetHand(); AddHand(a);}
	void AddHand(const CardData& a){Hand.append(a);}
	void SetHasAcceptedHand(bool a){HasAcceptedHand=a;}
	bool GetHasAcceptedHand() const {return HasAcceptedHand;}
	void SetPlayerID(int a){PlayerID=a;}
	int GetPlayerID() const {return PlayerID;}
	void SetReadyToStartMatch(bool a){ReadyToStartMatch=a;}
	bool GetReadyToStartMatch() const {return ReadyToStartMatch;}
	void SetPlayerColor(const QColor& a){PlayerColor=a;}
	void SetPlayerColor(const QString& a);
	const QColor& GetPlayerColor() const {return PlayerColor;}
	void SetLibrary();
	void SetLibrary(const QList<CardData>& a);
	void SetLibrary(const CardDeck& a);
	QList<CardData>& GetLibrary() {return Library;}
	void AddLibrary(const CardData& a){Library.append(a);}
	void SetSideboard(const QList<CardData>& a){Sideboard.clear(); Sideboard=a;}
	void SetSideboard(){Sideboard.clear();}
	void SetSideboard(const CardDeck& a){SetLibrary(a);}
	const QList<CardData>& GetSideboard() const {return Sideboard;}
	const QList<CardData>& GetHand() const {return Hand;}
	const QList<CardData>& GetGraveyard() const {return Graveyard;}
	const QList<CardData>& GetExile() const {return Exile;}
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
	const CardData& DrawCard();
	void SortHand();
	void DiscardRandom(){Graveyard.append(Hand.takeAt(qrand()%Hand.size()));}
	void HandToBottomLibrary(){while(!Hand.isEmpty()) Library.append(Hand.takeAt(0));}
	void SetCanPlayMana(bool a){CanPlayMana=a;}
	bool GetCanPlayMana() const {return CanPlayMana;}
	MagiQPlayer(const MagiQPlayer& a,QObject* parent=0);
	const MagiQPlayer& operator=(const MagiQPlayer& a);
};
#endif