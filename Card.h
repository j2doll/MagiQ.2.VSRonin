#ifndef CARD_H
#define CARD_H
#include "CostantsDefinition.h"
#include <QWidget>
#include <QList>
#include <QMap>
#include <QPixmap>
#include <QDataStream>
class ManaCostLabel;
class QFrame;
class QLabel;
class QScrollArea;
class QPushButton;
class PowerToughnesLabel;
class Card : public QWidget{
	Q_OBJECT
	Q_PROPERTY(int CardRarity READ GetCardRarity)
	Q_PROPERTY(int CardBackground READ GetCardBackground)
private:
	enum {TextFontSize=20};
	enum {PTFontSize=12};
// Visual Elements
	QFrame* Background;
	QFrame* Rear;
	QLabel* NameLabel;
	ManaCostLabel* CostLabel;
	QLabel* ImageLabel;
	QLabel* TypeLabel;
	QLabel* EditionLabel;
	PowerToughnesLabel* PTLabel;
	QScrollArea* EffectsArea;
	QList<QPushButton*> EffectButtons;
	QLabel* FlavorTextLabel;
	QPixmap PTBox;
	QPixmap PTMask;
// Card Properties (this section is serialized)
	QString CardName;
	QList<int> CardColor;
	QMap<int,int> CardCost;
	QList<int> CardCostModifiers;
	QList<int> CardType;
	QList<int> CardSubType;
	QList<int> AvailableEditions;
	QList<QPixmap> AvailableImages;
	QList<int> AvailableBackgrounds;
	int CardEdition;
	int CardBackground;
	QString CardFlavorText;
	bool HasPT;
	int CardPower;
	QList<int> CardPowerModifiers;
	int CardToughness;
	QList<int> CardToughnessModifiers;
	enum {StarPowerToughness=-2147483648};
	int CardRarity;
	int CardImage;
	bool ManaSource;
// Game Properties
	bool Covered;
	bool Tapped;
	QMap<int,int> Counters;


	QString CreateManaCostString() const;
	void ResetCardCost();
	void TestStuff();
public:
	const QList<int>& GetCardCostModifiers() const {return CardCostModifiers;}
	void SetCardCostModifiers(int a){CardCostModifiers.clear(); CardCostModifiers.append(a);}
	void SetCardCostModifiers(const QList<int>& a){CardCostModifiers.clear(); CardCostModifiers=a;}
	void AddCardCostModifiers(int a){CardCostModifiers.append(a);}
	enum {ModifierReduce=-1,ModifierIncrement=1};
	int GetCardPower() const {return CardPower;}
	int GetCardToughness() const {return CardToughness;}
	void SetCardPower(int a) {CardPower=a;}
	void SetCardToughness(int a) {CardToughness=a;}
	const QList<int>& GetCardToughnessModifiers() const {return CardToughnessModifiers;}
	const QList<int>& GetCardPowerModifiers() const {return CardPowerModifiers;}
	void SetCardPowerModifiers(int a){CardPowerModifiers.clear(); CardPowerModifiers.append(a);}
	void SetCardToughnessModifiers(int a){CardToughnessModifiers.clear(); CardToughnessModifiers.append(a);}
	void SetCardPowerModifiers(const QList<int>& a){CardPowerModifiers.clear(); CardPowerModifiers=a;}
	void SetCardToughnessModifiers(const QList<int>& a){CardToughnessModifiers.clear(); CardToughnessModifiers=a;}
	void AddCardPowerModifier(int a){CardPowerModifiers.append(a);}
	void AddCardToughnessModifier(int a){CardToughnessModifiers.append(a);}
	enum {CardVersion=0};
	bool IsManaSource() const {return ManaSource;}
	bool IsTapped() const {return Tapped;}
	void SetTapped(bool a){Tapped=a;}
	void SetManaSource(bool a){ManaSource=a;}
	Card(QWidget* parent=0);
	void SetCardType(const int& a){CardType.clear(); CardType.append(a);}
	void SetCardType(const QList<int>& a){CardType.clear(); CardType=a;}
	const QList<int>& GetCardType() const {return CardType;}
	void SetCardSubType(const int& a){CardSubType.clear(); CardSubType.append(a);}
	void SetCardSubType(const QList<int>& a){CardSubType.clear(); CardSubType=a;}
	const QList<int>& GetCardSubType() const {return CardSubType;}
	void SetEdition(int a){if(a<0 || a>=AvailableEditions.size()) return; CardEdition=AvailableEditions.at(a);}
	int GetEdition() const {return CardEdition;}
	void SetCardColor(const int& a){CardColor.clear(); CardColor.append(a);}
	void SetCardColor(const QList<int>& a){CardColor.clear(); CardColor=a;}
	const QList<int>& GetCardColor() const {return CardColor;}
	void SetCardCost(int key, int cost);
	void SetCardCost(const QMap<int,int>& a){ResetCardCost(); CardCost=a;}
	const QMap<int,int>& GetCardCost() const {return CardCost;}
	void SetCardName(const QString& a){if(!a.isEmpty()) CardName=a;}
	QString GetCardName() const {return CardName;}
	QString GetCardFlavorText() const {return CardFlavorText;}
	void SetCardFlavorText(const QString& a){if(!a.isEmpty()) CardFlavorText=a;}
	int GetCardRarity() const {return CardRarity;}
	void SetCardrarity(int a){if (a<Constants::CardRarities::Common || a>Constants::CardRarities::MythicRare) return; CardRarity=a;}
	const QList<int>& GetAvailableEditions() const {return AvailableEditions;}
	const QList<int>& GetAvailableBackgrounds() const {return AvailableBackgrounds;}
	int GetCardBackground() const {return CardBackground;}
	const QList<QPixmap>& GetAvailableImages() const {return AvailableImages;}
	void SetCardBackground(int a){if(a<0 || a>=AvailableBackgrounds.size()) return; CardBackground=AvailableBackgrounds.at(a);}
	void SetCardImage(int a){if(a<0 || a>=AvailableImages.size()) return; CardImage=a;}
	int GetCardImage() const {return CardImage;}
	void SetAvailableEditions(const int& a){AvailableEditions.clear(); AvailableEditions.append(a);}
	void SetAvailableEditions(const QList<int>& a){AvailableEditions.clear(); AvailableEditions=a;}
	void SetAvailableBackgrounds(const int& a){AvailableBackgrounds.clear(); AvailableBackgrounds.append(a);}
	void SetAvailableBackgrounds(const QList<int>& a){AvailableBackgrounds.clear(); AvailableBackgrounds=a;}
	void SetAvailableImages(const QPixmap& a);
	void SetAvailableImages(const QList<QPixmap>& a);
	bool IsLand() const;
	bool IsCovered() const {return Covered;}
	bool GetHasPT() const {return HasPT;}
	void SetCovered(bool a){Covered=a;}
	void SetHasPT(bool a){HasPT=a;}
public slots:
	void UpdateAspect();
protected:
	void resizeEvent(QResizeEvent* event);
};
QDataStream &operator<<(QDataStream &out, const Card &card);
QDataStream &operator>>(QDataStream &in, Card &card);
#endif