#ifndef CARD_H
#define CARD_H
#include "CostantsDefinition.h"
#include "CardData.h"
#include <QWidget>
#include <QList>
#include <QMap>
#include <QPixmap>
#include <QDataStream>
#include <QStringList>
class ManaCostLabel;
class QFrame;
class QLabel;
class QScrollArea;
class QPushButton;
class PowerToughnesLabel;
class MagiQPlayer;
class Effect;
class QTableWidget;
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
	QTableWidget* EffectsTable;
	QLabel* FlavorTextLabel;
	QPixmap PTBox;
	QPixmap PTMask;
// Card Properties (this section is serialized)
	QString CardName;
	QList<int> CardColor;
	QMap<int,int> CardCost;
	QList<int> CardType;
	QList<int> CardSubType;
	QList<int> AvailableEditions;
	QList<QPixmap> AvailableImages;
	QStringList ImagesTitles;
	QList<int> AvailableBackgrounds;
	int CardEdition;
	int CardBackground;
	QString CardFlavorText;
	bool HasPT;
	int CardPower;
	int CardToughness;
	int CardRarity;
	int CardImage;
	bool Certified;
	int CertifiedCardID;
	bool HasManaCost;
	int HasFlipped;
	Card* FlippedCard;
	QList<Effect*> Effects;
	mutable unsigned int CardID;
// Game Properties
	QList<int> CardToughnessModifiers;
	QList<int> CardPowerModifiers;
	QList<int> CardCostModifiers;
	MagiQPlayer* Owner;
	MagiQPlayer* Controller;
	bool Covered;
	bool Tapped;
	QMap<int,int> Counters;
	bool ShowFlavorText;
	bool IsNull;

// Functions
	QString CreateManaCostString() const;
	void ResetCardCost();
	void PrepareUi();
public:
	void SetIsNull(bool a) {IsNull=a;}
	bool GetIsNull() const {return IsNull;}
	unsigned int GetCardID() const {return CardID;}
	void SetCardID(unsigned int a) const {CardID=a;}
	int GetCertifiedCardID() const {return CertifiedCardID;}
	void SetCertifiedCardID(int a){CertifiedCardID=a;}
	void SetImagesTitles(){ImagesTitles.clear();}
	void SetImagesTitles(const QStringList& a){ImagesTitles.clear(); ImagesTitles=a;}
	void AddImageTitle(const QString& a){ImagesTitles.append(a);}
	void SetImagesTitles(const QString& a){SetImagesTitles(); AddImageTitle(a);}
	const QStringList& GetImagesTitles() const {return ImagesTitles;}
	void SetShowFlavorText(bool a){ShowFlavorText=a;}
	bool GetShowFlavorText() const {return ShowFlavorText;}
	const QList<Effect*>& GetEffects() const {return Effects;}
	void SetEffects();
	void SetEffects(const QList<Effect*>& a);
	void SetEffects(Effect& a){SetEffects(); AddEffect(a);}
	void AddEffect(Effect& a);
	MagiQPlayer* GetOwner() const{return Owner;}
	MagiQPlayer* GetController() const{return Controller;}
	void SetOwner(MagiQPlayer* a){Owner=a;}
	void SetController(MagiQPlayer* a){Controller=a;}
	int GetConvertedManaCost() const;
	Card(const Card& a,QWidget* parent=0);
	Card(const CardData& a,QWidget* parent=0);
	Card& operator=(const Card& a);
	enum {StarPowerToughness=-2147483648};
	void SetHasFlipped(int a){if (a>=Constants::CardFlipMode::AllreadyFlipped && a<=Constants::CardFlipMode::HasFlip) HasFlipped=a;};
	int GetHasFlipped() const {return HasFlipped;}
	void SetFlippedCard(Card* a){FlippedCard=a;}
	Card* GetFlippedCard() const {return FlippedCard;}
	void SetHasManaCost(bool a){HasManaCost=a;}
	bool GetHasManaCost() const {return HasManaCost;}
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
	bool IsTapped() const {return Tapped;}
	void SetTapped(bool a){Tapped=a;}
	Card(QWidget* parent=0);
	void SetCardType(){CardType.clear();}
	void SetCardType(const int& a){CardType.clear(); CardType.append(a);}
	void AddCardType(const int& a){if(a>=0 && a<Constants::CardTypes::END) CardType.append(a);}
	void SetCardType(const QList<int>& a){CardType.clear(); CardType=a;}
	const QList<int>& GetCardType() const {return CardType;}
	void SetCardSubType(){CardSubType.clear();}
	void SetCardSubType(const int& a){CardSubType.clear(); CardSubType.append(a);}
	void AddCardSubType(const int& a){if(a>=0 && a<Constants::CardSubTypes::END) CardSubType.append(a);}
	void SetCardSubType(const QList<int>& a){CardSubType.clear(); CardSubType=a;}
	const QList<int>& GetCardSubType() const {return CardSubType;}
	void SetEdition(int a){if(AvailableEditions.contains(a)) CardEdition=a;}
	int GetEdition() const {return CardEdition;}
	void SetCardColor(const int& a){CardColor.clear(); if(a>=Constants::CardColor::Colorless && a<=Constants::CardColor::Green) CardColor.append(a);}
	void SetCardColor(const QList<int>& a){CardColor.clear(); CardColor=a;}
	void AddCardColor(const int& a){if(a>=Constants::CardColor::Colorless && a<=Constants::CardColor::Green) CardColor.append(a);}
	const QList<int>& GetCardColor() const {return CardColor;}
	void SetCardCost(int key, int cost);
	void AddCardCost(int key, int cost);
	void SetCardCost(){ResetCardCost();}
	void SetCardCost(const QMap<int,int>& a){ResetCardCost(); CardCost=a;}
	const QMap<int,int>& GetCardCost() const {return CardCost;}
	void SetCardName(const QString& a){CardName=a;}
	QString GetCardName() const {return CardName;}
	QString GetCardFlavorText() const {return CardFlavorText;}
	void SetCardFlavorText(const QString& a){CardFlavorText=a;}
	int GetCardRarity() const {return CardRarity;}
	void SetCardrarity(int a){if (a<Constants::CardRarities::Common || a>Constants::CardRarities::MythicRare) return; CardRarity=a;}
	const QList<int>& GetAvailableEditions() const {return AvailableEditions;}
	const QList<int>& GetAvailableBackgrounds() const {return AvailableBackgrounds;}
	int GetCardBackground() const {return CardBackground;}
	const QList<QPixmap>& GetAvailableImages() const {return AvailableImages;}
	void SetCardBackground(int a){if(AvailableBackgrounds.contains(a)) CardBackground=a;}
	void SetCardImage(int a){if(a<0 || a>=AvailableImages.size()) return; CardImage=a;}
	int GetCardImage() const {return CardImage;}
	void SetAvailableEditions(){AvailableEditions.clear(); AvailableEditions.append(Constants::Editions::NONE);}
	void AddAvailableEditions(const int& a);
	void SetAvailableEditions(const int& a){AvailableEditions.clear(); AvailableEditions.append(a);}
	void SetAvailableEditions(const QList<int>& a){AvailableEditions.clear(); AvailableEditions=a;}
	void SetAvailableBackgrounds(){AvailableBackgrounds.clear(); CardBackground=Constants::CardBacksrounds::Colorless;}
	void AddAvailableBackground(const int& a){if (a>=0 && a<Constants::CardBacksrounds::END) AvailableBackgrounds.append(a);}
	void SetAvailableBackgrounds(const int& a){AvailableBackgrounds.clear(); AvailableBackgrounds.append(a);}
	void SetAvailableBackgrounds(const QList<int>& a){AvailableBackgrounds.clear(); AvailableBackgrounds=a;}
	void SetAvailableImages(const QPixmap& a);
	void SetAvailableImages();
	void AddAvailableImages(const QPixmap& a);
	void SetAvailableImages(const QList<QPixmap>& a);
	bool IsLand() const;
	bool IsCovered() const {return Covered;}
	bool GetHasPT() const {return HasPT;}
	void SetCovered(bool a){Covered=a;}
	void SetHasPT(bool a){HasPT=a;}
	bool GetCertified() const {return Certified;}
	void SetCertified(bool a){Certified=a;}
	CardData ExtractData() const;
public slots:
	void UpdateAspect();
protected:
	void resizeEvent(QResizeEvent* event);
};
QDataStream &operator<<(QDataStream &out, const Card &card);
QDataStream &operator>>(QDataStream &in, Card &card);
#endif