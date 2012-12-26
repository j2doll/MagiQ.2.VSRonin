#include "Card.h"
#include "ManaCostLabel.h"
#include "StyleSheets.h"
#include <QBitmap>
#include <QFrame>
#include <QLabel>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QPainter>
Card::Card(QWidget* parent/* =0 */)
	:QWidget(parent)
	,CardEdition(0)
	,CardPower(-1)
	,CardToughness(-1)
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
	PTLabel=new QLabel(this);
	PTLabel->setObjectName("PTLabel");
	PTLabel->setScaledContents(true);

	setStyleSheet(StyleSheets::GlobalCSS);
	TestStuff();
}
void Card::TestStuff(){
	CardBackground=0;
	CardRarity=1;
	QPixmap NamePixmap(646,57);
	NamePixmap.fill(QColor(0,0,0,0));
	QPainter GhostWriter(&NamePixmap);
	GhostWriter.setFont(QFont("Arial",TextFontSize,QFont::Bold));
	GhostWriter.drawText(NamePixmap.rect(),Qt::AlignLeft,"Gemhide Sliver");
	NameLabel->setPixmap(NamePixmap);
	CostLabel->SetCostString("1,g");
	ImageLabel->setPixmap(QPixmap("D:\\Giochi\\Magic Workstation\\Pics\\TSP\\Gemhide Sliver.jpg"));
	NamePixmap.fill(QColor(0,0,0,0));
	GhostWriter.drawText(NamePixmap.rect(),Qt::AlignLeft,"Creature - Sliver");
	TypeLabel->setPixmap(NamePixmap);
	EditionLabel->setMask(QPixmap(":/Editions/TSP.png").mask()); //TODO Resizing
	CardEdition=0;
	NamePixmap.fill(QColor(0,0,0,0));
	GhostWriter.drawText(NamePixmap.rect(),Qt::AlignCenter,"1/1");
	PTLabel->setPixmap(NamePixmap);
	setMinimumSize(200,279);
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
	PTLabel->setGeometry(46*width()/200,246*height()/279,241*width()/200,22*height()/279);
	EditionLabel->setGeometry(
		(179-Constants::EditionSymbolSyze[CardEdition].width()*(10*height()/279)/Constants::EditionSymbolSyze[CardEdition].height())*width()/200
		,159*height()/279
		,(Constants::EditionSymbolSyze[CardEdition].width()*(10*height()/279)/Constants::EditionSymbolSyze[CardEdition].height())*width()/200
		,10*height()/279
	);
	ImageLabel->setGeometry(17*width()/200,33*height()/279,166*width()/200,123*height()/279);
}
void Card::SetAvailableImages(const QPixmap& a){
	if (!AvailableImages.isEmpty()){
		for (QList<QPixmap*>::iterator i=AvailableImages.begin();i!=AvailableImages.end();i++)
			delete (*i);
		AvailableImages.clear();
	}
	AvailableImages.append(new QPixmap(a));
}
void Card::SetAvailableImages(const QList<QPixmap*>& a){
	if (!AvailableImages.isEmpty()){
		for (QList<QPixmap*>::iterator i=AvailableImages.begin();i!=AvailableImages.end();i++)
			delete (*i);
		AvailableImages.clear();
	}
	AvailableImages=a;
}
Card::~Card(){
	if (!AvailableImages.isEmpty()){
		for (QList<QPixmap*>::iterator i=AvailableImages.begin();i!=AvailableImages.end();i++)
			delete (*i);
	}
}