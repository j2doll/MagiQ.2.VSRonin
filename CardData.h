#ifndef CARDDATA_H
#define CARDDATA_H
#include "EffectData.h"
#include "CostantsDefinition.h"
#include <QList>
#include <QMap>
#include <QString>
#include <QPixmap>
#include <QScopedPointer>
#include <QDataStream>
#include <QStringList>
class CardData{
public:
	CardData();
	CardData(const CardData& a);
	
	unsigned int GetCardID() const {return CardID;}
	const QString& GetCardName() const {return CardName;}
	const QList<int>& GetCardColor() const {return CardColor;}
	const QMap<int,int>& GetCardCost() const {return CardCost;}
	const QList<int>& GetCardType() const {return CardType;}
	const QList<int>& GetCardSubType() const {return CardSubType;}
	const QList<int>& GetAvailableEditions() const {return AvailableEditions;}
	const QList<QPixmap>& GetAvailableImages() const {return AvailableImages;}
	const QStringList& GetImagesTitles() const {return ImagesTitles;}
	const QList<int>& GetAvailableBackgrounds() const {return AvailableBackgrounds;}
	int GetCardEdition() const {return CardEdition;}
	int GetCardBackground() const {return CardBackground;}
	const QString& GetCardFlavorText() const {return CardFlavorText;}
	bool GetHasPT() const {return HasPT;}
	int GetCardPower() const {return CardPower;}
	int GetCardToughness() const {return CardToughness;}
	int GetCardRarity() const {return CardRarity;}
	int GetCardImage() const {return CardImage;}
	bool GetCertified() const {return Certified;}
	int GetCertifiedCardID() const {return CertifiedCardID;}
	bool GetHasManaCost() const {return HasManaCost;}
	int GetHasFlipped() const {return HasFlipped;}
	const QScopedPointer<CardData>& GetFlippedCard() const {return FlippedCard;}
	const QList<EffectData>& GetEffects() const {return Effects;}
	int GetSortingMethod() const {return SortingMethod;}
	enum {Random,ByName,ByManaCost,ByType,ByColor};
	int GetConvertedManaCost() const;
	
	void SetCardID(unsigned int a) const {CardID=a;}
	void SetCardName(const QString& a){CardName=a;}
	void SetCardColor(const int& a){CardColor.clear(); if(a>=Constants::CardColor::Colorless && a<=Constants::CardColor::Green) CardColor.append(a);}
	void SetCardColor(const QList<int>& a){CardColor.clear(); CardColor=a;}
	void AddCardColor(const int& a){if(a>=Constants::CardColor::Colorless && a<=Constants::CardColor::Green) CardColor.append(a);}
	void SetCardCost(int key, int cost);
	void AddCardCost(int key, int cost);
	void SetCardCost();
	void SetCardCost(const QMap<int,int>& a){SetCardCost(); CardCost=a;}
	void SetCardType(){CardType.clear();}
	void SetCardType(const int& a){CardType.clear(); CardType.append(a);}
	void AddCardType(const int& a){if(a>=0 && a<Constants::CardTypes::END) CardType.append(a);}
	void SetCardType(const QList<int>& a){CardType.clear(); CardType=a;}
	void SetCardSubType(){CardSubType.clear();}
	void SetCardSubType(const int& a){CardSubType.clear(); CardSubType.append(a);}
	void AddCardSubType(const int& a){if(a>=0 && a<Constants::CardSubTypes::END) CardSubType.append(a);}
	void SetCardSubType(const QList<int>& a){CardSubType.clear(); CardSubType=a;}
	void SetAvailableEditions(){AvailableEditions.clear(); AvailableEditions.append(Constants::Editions::NONE);}
	void AddAvailableEditions(const int& a);
	void SetAvailableEditions(const int& a){AvailableEditions.clear(); AvailableEditions.append(a);}
	void SetAvailableEditions(const QList<int>& a){AvailableEditions.clear(); AvailableEditions=a;}
	void SetAvailableImages(const QPixmap& a);
	void SetAvailableImages();
	void AddAvailableImages(const QPixmap& a);
	void SetAvailableImages(const QList<QPixmap>& a);
	void SetImagesTitles(){ImagesTitles.clear();}
	void SetImagesTitles(const QStringList& a){ImagesTitles.clear(); ImagesTitles=a;}
	void AddImageTitle(const QString& a){ImagesTitles.append(a);}
	void SetImagesTitles(const QString& a){SetImagesTitles(); AddImageTitle(a);}
	void SetAvailableBackgrounds(){AvailableBackgrounds.clear(); CardBackground=Constants::CardBacksrounds::Colorless;}
	void AddAvailableBackground(const int& a){if (a>=0 && a<Constants::CardBacksrounds::END) AvailableBackgrounds.append(a);}
	void SetAvailableBackgrounds(const int& a){AvailableBackgrounds.clear(); AvailableBackgrounds.append(a);}
	void SetAvailableBackgrounds(const QList<int>& a){AvailableBackgrounds.clear(); AvailableBackgrounds=a;}
	void SetCardEdition(int a){if(AvailableEditions.contains(a)) CardEdition=a;}
	void SetCardBackground(int a){if(AvailableBackgrounds.contains(a)) CardBackground=a;}
	void SetCardFlavorText(const QString& a){CardFlavorText=a;}
	void SetHasPT(bool a){HasPT=a;}
	void SetCardPower(int a) {CardPower=a;}
	void SetCardToughness(int a) {CardToughness=a;}
	void SetCardrarity(int a){if (a<Constants::CardRarities::Common || a>Constants::CardRarities::MythicRare) return; CardRarity=a;}
	void SetCardImage(int a){if(a<0 || a>=AvailableImages.size()) return; CardImage=a;}
	void SetCertified(bool a){Certified=a;}
	void SetCertifiedCardID(int a){CertifiedCardID=a;}
	void SetHasManaCost(bool a){HasManaCost=a;}
	void SetHasFlipped(int a){if (a>=Constants::CardFlipMode::AllreadyFlipped && a<=Constants::CardFlipMode::HasFlip) HasFlipped=a;};
	void SetFlippedCard(CardData* a){FlippedCard.reset(a);}
	void SetEffects(){Effects.clear();}
	void SetEffects(const QList<EffectData>& a){Effects.clear(); Effects=a;}
	void SetEffects(const EffectData& a){SetEffects(); AddEffect(a);}
	void AddEffect(const EffectData& a){Effects.append(a);}
	void SetSortingMethod(int a) const {if(a>=Random && a<=ByColor) SortingMethod=a;}
	
	bool operator<(const CardData& a) const;
	const CardData& operator=(const CardData& a);
	CardData ToNoImage() const;
private:
	mutable unsigned int CardID;
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
	QScopedPointer<CardData> FlippedCard;
	QList<EffectData> Effects;
	mutable int SortingMethod;
};
QDataStream &operator<<(QDataStream &out, const CardData &card);
QDataStream &operator>>(QDataStream &in, CardData &card);
#endif