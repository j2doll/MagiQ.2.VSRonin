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
Card::Card(QWidget* parent/* =0 */)
	:QWidget(parent)
	,CardEdition(0)
	,CardPower(-1)
	,CardToughness(-1)
	,PTBox(":/CardImage/PTBox.png")
	,PTMask(":/CardImage/PTBoxMask.png")
	,Covered(false)
	,HasPT(false)
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
	PTLabel=new PowerToughnesLabel(this);
	PTLabel->setObjectName("PTLabel");

	setStyleSheet(StyleSheets::GlobalCSS);
	TestStuff();
}
void Card::TestStuff(){
	CardBackground=Constants::CardBacksrounds::Green;
	CardRarity=Constants::CardRarities::Common;
	CardName="Gemhide Sliver";
	CardCost.append('1');
	CardCost.append('g');
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
	EditionLabel->setMask(QPixmap(":/Editions/TSP.png").scaled(
		(Constants::EditionSymbolSyze[CardEdition].width()*(10*height()/279)/Constants::EditionSymbolSyze[CardEdition].height())*width()/200
		,10*height()/279
		).mask());
	ImageLabel->setGeometry(17*width()/200,33*height()/279,166*width()/200,123*height()/279);
}
void Card::SetAvailableImages(const QPixmap& a){
	AvailableImages.clear();
	AvailableImages.append(a);
}
void Card::SetAvailableImages(const QList<QPixmap>& a){
	AvailableImages.clear();
	AvailableImages=a;
}
bool Card::IsLand() const {return CardType.contains(Constants::CardTypes::Land);}
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
	CostLabel->SetCostString(CreateManaCostString());
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
	if (HasPT){
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
	else PTLabel->hide();
	style()->unpolish(this);
	style()->polish(this);
}
QString Card::CreateManaCostString() const{
	QString Result="";
	for (QList<QChar>::const_iterator i=CardCost.begin();i!=CardCost.end();i++){
		if (i!=CardCost.begin()) Result.append(',');
		Result.append(*i);
	}
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
	;
	return out;
}
QDataStream &operator>>(QDataStream &input, Card &card){
	qint32 Numbers;
	QString Strings;
	QList<int> IntLists;
	QList<QChar> CharLists;
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
	input >> CharLists;
	card.SetCardCost(CharLists);
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
	card.UpdateAspect();
	return input;
}