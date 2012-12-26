#ifndef CARD_H
#define CARD_H
#include "CostantsDefinition.h"
#include <QWidget>
#include <QList>
class ManaCostLabel;
class QPixmap;
class QFrame;
class QLabel;
class QScrollArea;
class QPushButton;
class SizeSliders;
class Card : public QWidget{
	Q_OBJECT
	Q_PROPERTY(int CardRarity READ GetCardRarity WRITE SetCardrarity)
	Q_PROPERTY(int CardBackground READ GetCardBackground WRITE SetCardBackground)
private:
	enum {TextFontSize=20};
	enum {Common,Uncommon,Rare,MythicRare};
	QFrame* Background;
	QFrame* Rear;
	QLabel* NameLabel;
	ManaCostLabel* CostLabel;
	QLabel* ImageLabel;
	QLabel* TypeLabel;
	QLabel* EditionLabel;
	QLabel* PTLabel;
	QScrollArea* EffectsArea;
	QList<QPushButton*> EffectButtons;
	QLabel* FlavorTextLabel;

	QString CardName;
	QList<int> CardColor;
	QList<int> CardCost;
	QList<int> CardType;
	QList<int> CardSubType;
	QList<int> AvailableEditions;
	QList<QPixmap*> AvailableImages;
	QList<int> AvailableBackgrounds;
	int CardEdition;
	int CardBackground;
	QString CardFlavorText;
	int CardPower;
	int CardToughness;
	int CardRarity;
	QPixmap* CardImage;

	void TestStuff();
public:
	Card(QWidget* parent=0);
	~Card();
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
	void SetCardCost(const int& a){CardCost.clear(); CardCost.append(a);}
	void SetCardCost(const QList<int>& a){CardCost.clear(); CardCost=a;}
	const QList<int>& GetCardCost() const {return CardCost;}
	void SetCardName(const QString& a){if(!a.isEmpty()) CardName=a;}
	QString GetCardName() const {return CardName;}
	QString GetCardFlavorText() const {return CardFlavorText;}
	void SetCardFlavorText(const QString& a){if(!a.isEmpty()) CardFlavorText=a;}
	int GetCardRarity() const {return CardRarity;}
	void SetCardrarity(int a){if (a<Common || a>MythicRare) return; CardRarity=a;}
	const QList<int>& GetAvailableEditions() const {return AvailableEditions;}
	const QList<int>& GetAvailableBackgrounds() const {return AvailableBackgrounds;}
	int GetCardBackground() const {return CardBackground;}
	const QList<QPixmap*>& GetAvailableImages() const {return AvailableImages;}
	void SetCardBackground(int a){if(a<0 || a>=AvailableBackgrounds.size()) return; CardBackground=AvailableBackgrounds.at(a);}
	void SetCardImage(int a){if(a<0 || a>=AvailableImages.size()) return; CardImage=AvailableImages.at(a);}
	const QPixmap& GetCardImage() const {return *CardImage;}
	void SetAvailableEditions(const int& a){AvailableEditions.clear(); AvailableEditions.append(a);}
	void SetAvailableEditions(const QList<int>& a){AvailableEditions.clear(); AvailableEditions=a;}
	void SetAvailableBackgrounds(const int& a){AvailableBackgrounds.clear(); AvailableBackgrounds.append(a);}
	void SetAvailableBackgrounds(const QList<int>& a){AvailableBackgrounds.clear(); AvailableBackgrounds=a;}
	void SetAvailableImages(const QPixmap& a);
	void SetAvailableImages(const QList<QPixmap*>& a);
protected:
	void resizeEvent(QResizeEvent* event);
};
#endif