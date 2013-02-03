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
#include "Effect.h"
#include "PowerToughnesLabel.h"
#include <QTableWidget>
#include <QHeaderView>
#include "CardData.h"
Card::Card(QWidget* parent)
	:QWidget(parent)
	,CardEdition(0)
	,CardPower(0)
	,CardToughness(0)
	,CardImage(0)
	,PTBox(":/CardImage/PTBox.png")
	,PTMask(":/CardImage/PTBoxMask.png")
	,Covered(false)
	,HasPT(false)
	,Certified(false)
	,CardName("")
	,HasFlipped(Constants::CardFlipMode::NoFlip)
	,FlippedCard(NULL)
	,HasManaCost(true)
	,CardBackground(Constants::CardBacksrounds::Colorless)
	,CardRarity(Constants::CardRarities::Common)
	,ShowFlavorText(true)
	,CertifiedCardID(-1)
	,CardID(0)
	,IsNull(false)
	,Activable(false)
{
	PrepareUi();
	AvailableEditions.append(Constants::Editions::NONE);
	ResetCardCost();
	UpdateAspect();
}
void Card::PrepareUi(){
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
	EffectsTable=new QTableWidget(this);
	EffectsTable->setShowGrid(false);
	EffectsTable->setObjectName("EffectsTable");
	EffectsTable->setColumnCount(1);
	EffectsTable->setRowCount(0);
	EffectsTable->verticalHeader()->setVisible(false);
	EffectsTable->horizontalHeader()->setVisible(false);
	EffectsTable->horizontalHeader()->setStretchLastSection(true);
	EffectsTable->verticalHeader()->setStretchLastSection(true);
	EffectsTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	EffectsTable->setSortingEnabled(false);
	EffectsTable->setSelectionMode(QAbstractItemView::NoSelection);
	EffectsTable->setFocusPolicy(Qt::NoFocus);
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
	EffectsTable->setGeometry(18*width()/200,176*height()/279,163*width()/200,74*height()/279);
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
		EffectsTable->hide();
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
		EffectsTable->show();
	}
	QPixmap NamePixmap(646,57);
	NamePixmap.fill(QColor(0,0,0,0));
	QPainter GhostWriter(&NamePixmap);
	GhostWriter.setFont(QFont("Arial",TextFontSize,QFont::Bold));
	GhostWriter.drawText(NamePixmap.rect(),Qt::AlignLeft,CardName);
	NameLabel->setPixmap(NamePixmap);
	if (!HasManaCost) CostLabel->hide();
	CostLabel->SetCostString(CreateManaCostString());
	CostLabel->setGeometry(
		(181-(11*CostLabel->GetNumberOfSymbols()))*width()/200,
		17*height()/279,
		(11*CostLabel->GetNumberOfSymbols())*width()/200,
		11*height()/279
		);
	if(AvailableImages.isEmpty() || CardImage<-1 || CardImage>=AvailableImages.size())
		ImageLabel->setPixmap(QPixmap(":/CardImage/DefaultImage.png"));
	else if(CardImage==-1)
		ImageLabel->setPixmap(AvailableImages.at(qrand()%AvailableImages.size()));
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
			PTLabel->SetCountPT();
			switch(CardBackground){
			case Constants::CardBacksrounds::White:
			case Constants::CardBacksrounds::LandWhite:
				PTLabel->SetSelectedBase(Constants::PTBoxTypes::WBox); break;
			case Constants::CardBacksrounds::LandBlue:
			case Constants::CardBacksrounds::Blue:
				PTLabel->SetSelectedBase(Constants::PTBoxTypes::UBox); break;
			case Constants::CardBacksrounds::Black:
			case Constants::CardBacksrounds::LandBlack:
				PTLabel->SetSelectedBase(Constants::PTBoxTypes::BBox); break;
			case Constants::CardBacksrounds::Red:
			case Constants::CardBacksrounds::LandRed:
				PTLabel->SetSelectedBase(Constants::PTBoxTypes::RBox); break;
			case Constants::CardBacksrounds::Green:
			case Constants::CardBacksrounds::LandGreen:
				PTLabel->SetSelectedBase(Constants::PTBoxTypes::GBox); break;
			case Constants::CardBacksrounds::Gold:
			case Constants::CardBacksrounds::LandGold:
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
			PTLabel->raise();
		}
	}
	else PTLabel->hide();
	QString FlavorAdj(CardFlavorText);
	FlavorAdj.replace(QRegExp("<[ /]*i *>"),"");
	//**************************************************
	// Inefficient
	QList<Effect*> CopyEffects;
	foreach(Effect* eff,Effects){
		CopyEffects.append(new Effect(*eff,this));
		if (eff->GetHiddenEffect()) {CopyEffects.last()->hide();}
		eff->deleteLater();
	}
	EffectsTable->setRowCount(0);
	Effects=CopyEffects;
	FlavorTextLabel=new QLabel(this);
	FlavorTextLabel->setObjectName("FlavorTextLabel");
	FlavorTextLabel->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
	FlavorTextLabel->setWordWrap(true);
	FlavorTextLabel->setScaledContents(true);
	//**************************************************/
	if (CardType.contains(Constants::CardTypes::Basic) && CardType.contains(Constants::CardTypes::Land)){
		if (CardSubType.contains(Constants::CardSubTypes::Plain)) FlavorTextLabel->setPixmap(QPixmap(":/CardImage/PlainSymbol.png"));
		else if (CardSubType.contains(Constants::CardSubTypes::Island)) FlavorTextLabel->setPixmap(QPixmap(":/CardImage/IslandSymbol.png"));
		else if (CardSubType.contains(Constants::CardSubTypes::Swamp)) FlavorTextLabel->setPixmap(QPixmap(":/CardImage/SwampSymbol.png"));
		else if (CardSubType.contains(Constants::CardSubTypes::Mountain)) FlavorTextLabel->setPixmap(QPixmap(":/CardImage/MountainSymbol.png"));
		else if (CardSubType.contains(Constants::CardSubTypes::Forest)) FlavorTextLabel->setPixmap(QPixmap(":/CardImage/ForestSymbol.png"));
		EffectsTable->insertRow(0);
		EffectsTable->setItem(0,0,&QTableWidgetItem());
		EffectsTable->setCellWidget(0,0,FlavorTextLabel);
		EffectsTable->setRowHeight(0,EffectsTable->height());
	}
	else{
		int RequiredHeight=0;
		if(!FlavorAdj.isEmpty()) FlavorTextLabel->setText("<br><i>"+FlavorAdj+"</i><br>");
		else FlavorTextLabel->setText("");
		QRegExp tagMatcher("<.+>");
		tagMatcher.setMinimal(true);
		foreach(Effect* eff,Effects){
			if (eff->GetHiddenEffect()) continue;
			eff->UpdateAspect();
			EffectsTable->insertRow(EffectsTable->rowCount());
			EffectsTable->setCellWidget(EffectsTable->rowCount()-1,0,eff);
			EffectsTable->setRowHeight(EffectsTable->rowCount()-1,eff->GetMinimumHeight()+4);
			RequiredHeight+=eff->GetMinimumHeight()+4;
		}
		EffectsTable->insertRow(EffectsTable->rowCount());
		EffectsTable->setItem(EffectsTable->rowCount()-1,0,&QTableWidgetItem());
		EffectsTable->setCellWidget(EffectsTable->rowCount()-1,0,FlavorTextLabel);
		EffectsTable->setRowHeight(EffectsTable->rowCount()-1,qMax(FlavorTextLabel->sizeHint().height(),EffectsTable->height()-RequiredHeight));
	}
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
	out << qint32(Constants::CardVersion)
		<< quint32(card.GetCardID())
		<< card.GetCardName()
		<< card.GetCardColor()
		<< card.GetCardCost()
		<< card.GetCardType()
		<< card.GetCardSubType()
		<< card.GetAvailableEditions()
		<< card.GetAvailableImages()
		<< card.GetImagesTitles()
		<< card.GetAvailableBackgrounds()
		<< qint32(card.GetEdition())
		<< qint32(card.GetCardBackground())
		<< card.GetCardFlavorText()
		<< card.GetHasPT()
		<< qint32(card.GetCardPower())
		<< qint32(card.GetCardToughness())
		<< qint32(card.GetCardRarity())
		<< qint32(card.GetCardImage())
		<< card.GetCertified()
		<< qint32(card.GetCertifiedCardID())
		<< card.GetHasManaCost()
		<< qint32(card.GetEffects().size())
		<< qint32(card.GetHasFlipped())
	;
	foreach(Effect* eff,card.GetEffects()){
		out << *eff;
	}
	if (card.GetHasFlipped()==Constants::CardFlipMode::HasFlip)
		out << *(card.GetFlippedCard());
	return out;
}
QDataStream &operator>>(QDataStream &input, Card &card){
	qint32 Numbers;
	quint32 UnsignedNumbers;
	qint32 NumberOfEffects;
	Effect effect;
	QString Strings;
	QList<int> IntLists;
	QMap<int,int> IntMap;
	QList<QPixmap> ImageLists;
	QStringList StrList;
	bool Booleans;
	input >> Numbers;
	if (Numbers!=Constants::CardVersion){
		QMessageBox::critical(0,QObject::tr("Wrong File Type"),QObject::tr("Error: Unable to Read the File\nMake sure it's a valid MagiQ Card File"));
		return input;
	}
	input >> UnsignedNumbers;
	card.SetCardID(UnsignedNumbers);
	input >> Strings;
	card.SetCardName(Strings);
	input >> IntLists;
	card.SetCardColor(IntLists);
	input >> IntMap;
	card.SetCardCost(IntMap);
	IntLists.clear();
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
	input >> StrList;
	card.SetImagesTitles(StrList);
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
	input >> Numbers;
	card.SetCardToughness(Numbers);
	input >> Numbers;
	card.SetCardrarity(Numbers);
	input >> Numbers;
	card.SetCardImage(Numbers);
	input >> Booleans;
	card.SetCertified(Booleans);
	input >> Numbers;
	card.SetCertifiedCardID(Numbers);
	input >> Booleans;
	card.SetHasManaCost(Booleans);
	input >> NumberOfEffects;
	input >> Numbers;
	card.SetHasFlipped(Numbers);
	card.SetEffects();
	for (int i=0;i<NumberOfEffects;i++){
		input >> effect;
		card.AddEffect(effect);
	}
	if (Numbers==Constants::CardFlipMode::HasFlip){
		Card* TempCard;
		if (card.parent()){
			if(card.parent()->isWidgetType())
				TempCard=new Card(static_cast<QWidget*>(card.parent()));
		}
		else
			TempCard=new Card;
		input >> (*TempCard);
		TempCard->SetFlippedCard(&card);
		card.SetFlippedCard(TempCard);
		TempCard->UpdateAspect();
	}
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
Card& Card::operator=(const Card& a){
	Activable=a.Activable;
	IsNull=a.IsNull;
	CardName=a.CardName;
	CardID=a.CardID;
	CardColor=a.CardColor;
	CardCost=a.CardCost;
	CardCostModifiers=a.CardCostModifiers;
	CardType=a.CardType;
	CardSubType=a.CardSubType;
	AvailableEditions=a.AvailableEditions;
	AvailableImages=a.AvailableImages;
	ImagesTitles=a.ImagesTitles;
	AvailableBackgrounds=a.AvailableBackgrounds;
	CardEdition=a.CardEdition;
	CardBackground=a.CardBackground;
	CardFlavorText=a.CardFlavorText;
	HasPT=a.HasPT;
	CardPower=a.CardPower;
	CardPowerModifiers=a.CardPowerModifiers;
	CardToughness=a.CardToughness;
	CardToughnessModifiers=a.CardToughnessModifiers;
	CardRarity=a.CardRarity;
	CardImage=a.CardImage;
	Certified=a.Certified;
	CertifiedCardID=a.CertifiedCardID;
	HasManaCost=a.HasManaCost;
	HasFlipped=a.HasFlipped;
	FlippedCard=a.FlippedCard;
	SetEffects();
	foreach(Effect* eff,a.Effects)
		AddEffect(*eff);
	UpdateAspect();
	return *this;
}
Card::Card(const Card& a,QWidget* parent):QWidget(parent)
	,CardEdition(a.CardEdition)
	,CardPower(a.CardPower)
	,CardToughness(a.CardToughness)
	,CardImage(a.CardImage)
	,PTBox(":/CardImage/PTBox.png")
	,PTMask(":/CardImage/PTBoxMask.png")
	,Covered(false)
	,HasPT(a.HasPT)
	,Certified(a.Certified)
	,CardName(a.CardName)
	,HasFlipped(a.HasFlipped)
	,FlippedCard(NULL)
	,HasManaCost(a.HasManaCost)
	,CardColor(a.CardColor)
	,CardCost(a.CardCost)
	,CardCostModifiers(a.CardCostModifiers)
	,CardType(a.CardType)
	,CardSubType(a.CardSubType)
	,AvailableEditions(a.AvailableEditions)
	,AvailableBackgrounds(a.AvailableBackgrounds)
	,AvailableImages(a.AvailableImages)
	,CardBackground(a.CardBackground)
	,CardFlavorText(a.CardFlavorText)
	,CardPowerModifiers(a.CardPowerModifiers)
	,CardToughnessModifiers(a.CardToughnessModifiers)
	,CardRarity(a.CardRarity)
	,ShowFlavorText(a.ShowFlavorText)
	,CertifiedCardID(a.CertifiedCardID)
	,CardID(a.CardID)
	,IsNull(a.IsNull)
	,Activable(a.Activable)
{
	PrepareUi();
	if (HasFlipped==Constants::CardFlipMode::HasFlip){
		FlippedCard=new Card(*a.FlippedCard,parent);
		FlippedCard->SetFlippedCard(this);
	}
	foreach(Effect* eff,a.Effects)
		AddEffect(*eff);
	UpdateAspect();
}
Card::Card(const CardData& a,QWidget* parent):QWidget(parent)
	,CardEdition(a.GetCardEdition())
	,CardPower(a.GetCardPower())
	,CardToughness(a.GetCardToughness())
	,CardImage(a.GetCardImage())
	,PTBox(":/CardImage/PTBox.png")
	,PTMask(":/CardImage/PTBoxMask.png")
	,Covered(false)
	,HasPT(a.GetHasPT())
	,Certified(a.GetCertified())
	,CardName(a.GetCardName())
	,HasFlipped(a.GetHasFlipped())
	,FlippedCard(NULL)
	,HasManaCost(a.GetHasManaCost())
	,CardColor(a.GetCardColor())
	,CardCost(a.GetCardCost())
	,CardType(a.GetCardType())
	,CardSubType(a.GetCardSubType())
	,AvailableEditions(a.GetAvailableEditions())
	,AvailableBackgrounds(a.GetAvailableBackgrounds())
	,AvailableImages(a.GetAvailableImages())
	,CardBackground(a.GetCardBackground())
	,CardFlavorText(a.GetCardFlavorText())
	,CardRarity(a.GetCardRarity())
	,CertifiedCardID(a.GetCertifiedCardID())
	,CardID(a.GetCardID())
	,IsNull(a.GetIsNull())
	,Activable(a.GetActivable())
	,ShowFlavorText(true)
{
	PrepareUi();
	if (HasFlipped==Constants::CardFlipMode::HasFlip){
		FlippedCard=new Card(*a.GetFlippedCard(),parent);
		FlippedCard->SetFlippedCard(this);
	}
	foreach(EffectData eff,a.GetEffects())
		AddEffect(Effect(eff));
	UpdateAspect();
}
int Card::GetConvertedManaCost() const{
	int Result=0;
	for (int i=0;i<Constants::ManaCosts::END;i++){
		if (i>=Constants::ManaCosts::C2W && i<=Constants::ManaCosts::C2G)
			Result+=2*CardCost[i];
		else Result+=CardCost[i];
	}
	return Result;
}
void Card::SetEffects(){
	foreach(Effect* eff,Effects)
		eff->deleteLater();
	EffectsTable->setRowCount(0);
	Effects.clear();
}
void Card::SetEffects(const QList<Effect*>& a){
	SetEffects();
	foreach(Effect* eff,a){
		Effects.append(new Effect(*eff,this));
	}
}
void Card::AddEffect(Effect& a){
	Effects.append(new Effect(a,this));
}
CardData Card::ExtractData() const{
	CardData Result;
	Result.SetCardEdition(CardEdition);
	Result.SetCardPower(CardPower);
	Result.SetCardToughness(CardToughness);
	Result.SetCardImage(CardImage);
	Result.SetHasPT(HasPT);
	Result.SetCertified(Certified);
	Result.SetCertifiedCardID(CertifiedCardID);
	Result.SetCardName(CardName);
	Result.SetHasFlipped(HasFlipped);
	Result.SetHasManaCost(HasManaCost);
	Result.SetCardColor(CardColor);
	Result.SetCardCost(CardCost);
	Result.SetCardType(CardType);
	Result.SetCardSubType(CardSubType);
	Result.SetAvailableEditions(AvailableEditions);
	Result.SetAvailableBackgrounds(AvailableBackgrounds);
	Result.SetAvailableImages(AvailableImages);
	Result.SetCardBackground(CardBackground);
	Result.SetCardFlavorText(CardFlavorText);
	Result.SetCardrarity(CardRarity);
	Result.SetCertifiedCardID(CertifiedCardID);
	Result.SetCardID(CardID);
	Result.SetIsNull(IsNull);
	Result.SetActivable(Activable);
	if(HasFlipped==Constants::CardFlipMode::HasFlip)
		Result.SetFlippedCard(&(FlippedCard->ExtractData()));
	//TODO add effects
	return Result;
}

