#include "Card.h"
#include "ManaCostLabel.h"
#include "StyleSheets.h"
#include <QBitmap>
#include <QFrame>
#include <QLabel>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QPainter>
#include <QStyle>
#include <QMessageBox>
#include "PowerToughnesLabel.h"
Card::Card(QWidget* parent)
	:QWidget(parent)
	,CardEdition(0)
	,CardPower(-1)
	,CardToughness(-1)
	,CardImage(0)
	,PTBox(":/CardImage/PTBox.png")
	,PTMask(":/CardImage/PTBoxMask.png")
	,Covered(false)
	,HasPT(false)
	,ManaSource(false)
	,Certified(false)
	,CardName("")
{
	setObjectName("Card");
	Background=new QFrame(this);
	Background->setObjectName("Background");
	Rear=new QFrame(this);
	Rear->setObjectName("Rear");
	QHBoxLayout* BackLayout=new QHBoxLayout(this);
	BackLayout->setMargin(0);
	BackLayout->setSpacing(0);
	BackLayout->addWidget(Background);
	BackLayout->addWidget(Rear);
	Rear->hide();
	NameLabel=new QLabel(this);
	NameLabel->setObjectName("NameLabel");
	NameLabel->setScaledContents(true);
	CostLabel=new ManaCostLabel(this);
	CostLabel->setObjectName("CostLabel");
	ImageLabel=new QLabel(this);
	ImageLabel->setScaledContents(true);
	ImageLabel->setObjectName("ImageLabel");
	TypeLabel=new QLabel(this);
	TypeLabel->setObjectName("TypeLabel");
	TypeLabel->setScaledContents(true);
	EditionLabel=new QLabel(this);
	EditionLabel->setObjectName("EditionLabel");
	EditionLabel->setScaledContents(true);
	AvailableEditions.append(Constants::Editions::NONE);
	PTLabel=new PowerToughnesLabel(this);
	PTLabel->setObjectName("PTLabel");
	ResetCardCost();
	CardBackground=Constants::CardBacksrounds::Colorless;
	CardRarity=Constants::CardRarities::Common;
	setMinimumSize(200,279);
	setStyleSheet(StyleSheets::CardCSS); //Test
	UpdateAspect();
	//TestStuff(); //Test
}
void Card::TestStuff(){
	CardBackground=Constants::CardBacksrounds::Green;
	CardRarity=Constants::CardRarities::Common;
	CardName="Gemhide Sliver";
	CardCost[Constants::ManaCosts::Colorless]=1;
	CardCost[Constants::ManaCosts::G]=1;
	AvailableImages.append(QPixmap("D:\\Giochi\\Magic Workstation\\Pics\\TSP\\Gemhide Sliver.jpg"));
	CardImage=0;
	CardType.append(Constants::CardTypes::Creature);
	CardSubType.append(Constants::CardSubTypes::Sliver);
	CardEdition=0;
	HasPT=true;
	CardPower=1;
	CardToughness=1;
	setMinimumSize(200,279);
	UpdateAspect();
}
void Card::resizeEvent(QResizeEvent* event){
	NameLabel->setGeometry(19*width()/200,16*height()/279,241*width()/200,22*height()/279);
	CostLabel->setGeometry(
		(181-(11*CostLabel->GetNumberOfSymbols()))*width()/200,
		17*height()/279,
		(11*CostLabel->GetNumberOfSymbols())*width()/200,
		11*height()/279
	);
	TypeLabel->setGeometry(19*width()/200,158*height()/279,241*width()/200,22*height()/279);
	PTLabel->setGeometry(131*width()/200,239*height()/279,65*width()/200,38*height()/279);
	EditionLabel->setGeometry(
		(179-Constants::EditionSymbolSyze[CardEdition].width()*(10*height()/279)/Constants::EditionSymbolSyze[CardEdition].height())*width()/200
		,159*height()/279
		,(Constants::EditionSymbolSyze[CardEdition].width()*(10*height()/279)/Constants::EditionSymbolSyze[CardEdition].height())*width()/200
		,10*height()/279
	);
	EditionLabel->setMask(QPixmap(Constants::EditionSymbolsIcons[CardEdition]).scaled(
		(Constants::EditionSymbolSyze[CardEdition].width()*(10*height()/279)/Constants::EditionSymbolSyze[CardEdition].height())*width()/200
		,10*height()/279
		).mask());
	ImageLabel->setGeometry(17*width()/200,33*height()/279,166*width()/200,123*height()/279);
}
void Card::SetAvailableImages(const QPixmap& a){
	AvailableImages.clear();
	AvailableImages.append(a);
}
void Card::SetAvailableImages(){AvailableImages.clear();}
void Card::AddAvailableImages(const QPixmap& a){AvailableImages.append(a);}
void Card::SetAvailableImages(const QList<QPixmap>& a){
	AvailableImages.clear();
	AvailableImages=a;
}
bool Card::IsLand() const {return CardType.contains(Constants::CardTypes::Land) && !CardType.contains(Constants::CardTypes::Creature);}
void Card::UpdateAspect(){
	if (Covered){
		Background->hide();
		Rear->show();
		NameLabel->hide();
		CostLabel->hide();
		ImageLabel->hide();
		TypeLabel->hide();
		EditionLabel->hide();
		PTLabel->hide();
		setStyleSheet(StyleSheets::CardCSS);
		return;
	}
	else{
		Background->show();
		Rear->hide();
		NameLabel->show();
		CostLabel->show();
		ImageLabel->show();
		TypeLabel->show();
		EditionLabel->show();
		PTLabel->show();
	}
	QPixmap NamePixmap(646,57);
	NamePixmap.fill(QColor(0,0,0,0));
	QPainter GhostWriter(&NamePixmap);
	GhostWriter.setFont(QFont("Arial",TextFontSize,QFont::Bold));
	GhostWriter.drawText(NamePixmap.rect(),Qt::AlignLeft,CardName);
	NameLabel->setPixmap(NamePixmap);
	if (IsLand()) CostLabel->hide();
	CostLabel->SetCostString(CreateManaCostString());
	CostLabel->setGeometry(
		(181-(11*CostLabel->GetNumberOfSymbols()))*width()/200,
		17*height()/279,
		(11*CostLabel->GetNumberOfSymbols())*width()/200,
		11*height()/279
		);
	if(AvailableImages.isEmpty() || CardImage<0 || CardImage>AvailableImages.size())
		ImageLabel->setPixmap(QPixmap(":/CardImage/DefaultImage.png"));
	else
		ImageLabel->setPixmap(AvailableImages.at(CardImage));
	NamePixmap.fill(QColor(0,0,0,0));
	QString TextToPrint("");
	for (QList<int>::const_iterator i=CardType.begin();i!=CardType.end();i++){
		if(i!=CardType.begin()) TextToPrint.append(' ');
		if (*i>=0) TextToPrint.append(Constants::CardTypeNames[*i]);
	}
	if (!CardSubType.isEmpty())TextToPrint.append(" - ");
	for (QList<int>::const_iterator i=CardSubType.begin();i!=CardSubType.end();i++){
		if(i!=CardSubType.begin()) TextToPrint.append(' ');
		if (*i>=0) TextToPrint.append(Constants::CardSubTypesNames[*i]);
	}
	GhostWriter.drawText(NamePixmap.rect(),Qt::AlignLeft,TextToPrint);
	TypeLabel->setPixmap(NamePixmap);
	if (AvailableEditions.contains(Constants::Editions::NONE)) EditionLabel->hide();
	else EditionLabel->show();
	EditionLabel->setMask(QPixmap(Constants::EditionSymbolsIcons[CardEdition]).scaled(
		(Constants::EditionSymbolSyze[CardEdition].width()*(10*height()/279)/Constants::EditionSymbolSyze[CardEdition].height())*width()/200
		,10*height()/279
		).mask());
	if (HasPT){
		if (CardType.contains(Constants::CardTypes::Planeswalker)){
			PTLabel->SetCountLoyalty();
			PTLabel->SetLoyalty(CardPower);
		}
		else{
			switch(CardBackground){
			case Constants::CardBacksrounds::White:
				PTLabel->SetSelectedBase(Constants::PTBoxTypes::WBox); break;
			case Constants::CardBacksrounds::Blue:
				PTLabel->SetSelectedBase(Constants::PTBoxTypes::UBox); break;
			case Constants::CardBacksrounds::Black:
				PTLabel->SetSelectedBase(Constants::PTBoxTypes::BBox); break;
			case Constants::CardBacksrounds::Red:
				PTLabel->SetSelectedBase(Constants::PTBoxTypes::RBox); break;
			case Constants::CardBacksrounds::Green:
				PTLabel->SetSelectedBase(Constants::PTBoxTypes::GBox); break;
			case Constants::CardBacksrounds::Gold:
				PTLabel->SetSelectedBase(Constants::PTBoxTypes::GoldBox); break;
			default:
				PTLabel->SetSelectedBase(Constants::PTBoxTypes::CBox);
			}
			QString PTString("");
			if (CardPower==StarPowerToughness) PTString.append('*');
			else{
				for (QList<int>::const_iterator i=CardPowerModifiers.begin();i!=CardPowerModifiers.end();i++){
					if (*i<0) PTString.append('-');
					else PTString.append('+');
				}
				PTString.prepend(QString("%1").arg(CardPower+PTString.count('+')-PTString.count('-')));
			}
			PTString.append('/');
			if (CardToughness==StarPowerToughness) PTString.append('*');
			else{
				QString Temp("");
				for (QList<int>::const_iterator i=CardToughnessModifiers.begin();i!=CardToughnessModifiers.end();i++){
					if (*i<0) Temp.append('-');
					else Temp.append('+');
				}
				Temp.prepend(QString("%1").arg(CardToughness+PTString.count('+')-PTString.count('-')));
				PTString.append(Temp);
			}
			PTLabel->SetPTString(PTString);
			PTLabel->show();
		}
	}
	else PTLabel->hide();
	setStyleSheet(StyleSheets::CardCSS);
}
QString Card::CreateManaCostString() const{
	QString Result="";
	if (CardCost[Constants::ManaCosts::Colorless]!=0 || !CardCostModifiers.isEmpty()){
		int Temp=CardCost[Constants::ManaCosts::Colorless];
		for (QList<int>::const_iterator i=CardCostModifiers.begin();i!=CardCostModifiers.end();i++){
			if (*i<0) {Temp--; Result.append('-');}
			else {Temp++; Result.append('+');}
		}
		if (Temp<0) Result.prepend('0');
		else Result.prepend(QString("%1").arg(Temp));
	}
	for (int i=Constants::ManaCosts::Colorless+1;i<Constants::ManaCosts::C2W;i++){
		if (CardCost[i]>0){
			for (int j=0;j<CardCost[i];j++){
				if (!Result.isEmpty()) Result.append(',');
				Result.append(Constants::ManaCostsNames[i-1]);
			}
		}
	}
	for (int i=Constants::ManaCosts::C2W+1;i<Constants::ManaCosts::END;i++){
		if (CardCost[i]>0){
			if (!Result.isEmpty()) Result.append(',');
			Result.append(QString("2%1").arg(Constants::ManaCostsNames[i-1-Constants::ManaCosts::C2W]));
		}
	}
	if (Result.isEmpty()) Result="0";
	return Result;
}
QDataStream &operator<<(QDataStream &out, const Card &card)
{
	out << qint32(Card::CardVersion)
		<< card.GetCardName()
		<< card.GetCardColor()
		<< card.GetCardCost()
		<< card.GetCardCostModifiers()
		<< card.GetCardType()
		<< card.GetCardSubType()
		<< card.GetAvailableEditions()
		<< card.GetAvailableImages()
		<< card.GetAvailableBackgrounds()
		<< qint32(card.GetEdition())
		<< qint32(card.GetCardBackground())
		<< card.GetCardFlavorText()
		<< card.GetHasPT()
		<< qint32(card.GetCardPower())
		<< card.GetCardPowerModifiers()
		<< qint32(card.GetCardToughness())
		<< card.GetCardToughnessModifiers()
		<< qint32(card.GetCardRarity())
		<< qint32(card.GetCardImage())
		<< card.IsManaSource()
		<< card.GetCertified()
	;
	return out;
}
QDataStream &operator>>(QDataStream &input, Card &card){
	qint32 Numbers;
	QString Strings;
	QList<int> IntLists;
	QMap<int,int> IntMap;
	QList<QPixmap> ImageLists;
	bool Booleans;
	input >> Numbers;
	if (Numbers!=Card::CardVersion){
		QMessageBox::critical(0,QObject::tr("Wrong File Type"),QObject::tr("Error: Unable to Read the File\nMake sure it's a valid MagiQ Card File"));
		return input;
	}
	input >> Strings;
	card.SetCardName(Strings);
	input >> IntLists;
	card.SetCardColor(IntLists);
	input >> IntMap;
	card.SetCardCost(IntMap);
	IntLists.clear();
	input >> IntLists;
	card.SetCardCostModifiers(IntLists);
	IntLists.clear();
	input >> IntLists;
	card.SetCardType(IntLists);
	IntLists.clear();
	input >> IntLists;
	card.SetCardSubType(IntLists);
	IntLists.clear();
	input >> IntLists;
	card.SetAvailableEditions(IntLists);
	input >> ImageLists;
	card.SetAvailableImages(ImageLists);
	IntLists.clear();
	input >> IntLists;
	card.SetAvailableBackgrounds(IntLists);
	input >> Numbers;
	card.SetEdition(Numbers);
	input >> Numbers;
	card.SetCardBackground(Numbers);
	input >> Strings;
	card.SetCardFlavorText(Strings);
	input >> Booleans;
	card.SetHasPT(Booleans);
	input >> Numbers;
	card.SetCardPower(Numbers);
	IntLists.clear();
	input >> IntLists;
	card.SetCardPowerModifiers(IntLists);
	input >> Numbers;
	card.SetCardToughness(Numbers);
	IntLists.clear();
	input >> IntLists;
	card.SetCardToughnessModifiers(IntLists);
	input >> Numbers;
	card.SetCardrarity(Numbers);
	input >> Numbers;
	card.SetCardImage(Numbers);
	input >> Booleans;
	card.SetManaSource(Booleans);
	input >> Booleans;
	card.SetCertified(Booleans);
	card.UpdateAspect();
	return input;
}
void Card::ResetCardCost(){
	if (!CardCost.isEmpty())
		CardCost.clear();
	for (int i=Constants::ManaCosts::Colorless;i<Constants::ManaCosts::END;i++)
		CardCost.insert(i,0);
}
void Card::SetCardCost(int key, int cost)
{
	ResetCardCost(); 
	if (key<Constants::ManaCosts::Colorless || key>=Constants::ManaCosts::END){
		QMessageBox::critical(this,QObject::tr("Wrong Mana Type"),QObject::tr("Error: Unable to Set the Mana Cost.\nMake Sure the Mana Type Key is Valid"));
		return;
	}
	CardCost[key]=cost;
}
void Card::AddCardCost(int key, int cost){
	if (key<Constants::ManaCosts::Colorless || key>=Constants::ManaCosts::END){
		QMessageBox::critical(this,QObject::tr("Wrong Mana Type"),QObject::tr("Error: Unable to Set the Mana Cost.\nMake Sure the Mana Type Key is Valid"));
		return;
	}
	CardCost[key]+=cost;
}
void Card::AddAvailableEditions(const int& a){
	if (a>=0 && a<Constants::Editions::END){
		AvailableEditions.append(a);
		if (AvailableEditions.contains(Constants::Editions::NONE))
			AvailableEditions.erase(AvailableEditions.begin()+AvailableEditions.indexOf(Constants::Editions::NONE));
		if(AvailableEditions.size()==1)
			CardEdition=AvailableEditions.at(0);
	}
}