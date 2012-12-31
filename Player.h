#ifndef PLAYER_H
#define PLAYER_H
#include <QObject>
#include <QMap>
class Card;
class MagiQPlayer : public QObject{
	Q_OBJECT
private:
	int Life;
	int PrimaryOrdering;
	int SecondaryOrdering;
	bool CanPlayMana;
	QMap<int,int> ManaPool;
	QList<Card*> Library;
	QList<Card*> Hand;
	QList<Card*> Graveyard;
	QList<Card*> Exile;
	QList<Card*> OwnedCards;
	QList<Card*> ControlledCards;
signals:
	void LifeChanged(int l);
	void CardDrawn();
	void NoMoreCardsToDraw();
	void RequireUpdateAspect();
public:
	enum {ByName,ByManaCost,ByType,ByColor};
	void SetPrimaryOrdering(int a){if (a>=ByName && a <=ByColor) PrimaryOrdering=a;}
	int GetPrimaryOrdering() const {return PrimaryOrdering;}
	void SetSecondaryOrdering(int a){if (a>=ByName && a <=ByColor) SecondaryOrdering=a;}
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

	bool RandomOrder(const Card*& a,const Card*& b) const;
	bool NameOrder(const Card*& a,const Card*& b) const;
	bool ManaCostOrder(const Card*& a,const Card*& b) const;
	bool TypeOrder(const Card*& a,const Card*& b) const;
	bool ColorOrder(const Card*& a,const Card*& b) const;
}
#endif