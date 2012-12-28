#include "ManaCostLabel.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QPainter>
#include <QMessageBox>
ManaCostLabel::ManaCostLabel(QWidget* parent,const QString& Cost)
	:QWidget(parent)
	,CostString(Cost)
	,NumberOfSymbols(0)
	,ConvertedManaCost(0)
	,WSymbol(":/ManaSymbols/WMana.png")
	,USymbol(":/ManaSymbols/UMana.png")
	,BSymbol(":/ManaSymbols/BMana.png")
	,RSymbol(":/ManaSymbols/RMana.png")
	,GSymbol(":/ManaSymbols/GMana.png")
	,CSymbol(":/ManaSymbols/CMana.png")
	,CWSymbol(":/ManaSymbols/CWMana.png")
	,CUSymbol(":/ManaSymbols/CUMana.png")
	,CBSymbol(":/ManaSymbols/CBMana.png")
	,CRSymbol(":/ManaSymbols/CRMana.png")
	,CGSymbol(":/ManaSymbols/CGMana.png")
	,WUSymbol(":/ManaSymbols/WUMana.png")
	,WBSymbol(":/ManaSymbols/WBMana.png")
	,WRSymbol(":/ManaSymbols/WRMana.png")
	,WGSymbol(":/ManaSymbols/WGMana.png")
	,UBSymbol(":/ManaSymbols/UBMana.png")
	,URSymbol(":/ManaSymbols/URMana.png")
	,UGSymbol(":/ManaSymbols/UGMana.png")
	,BRSymbol(":/ManaSymbols/BRMana.png")
	,BGSymbol(":/ManaSymbols/BGMana.png")
	,RGSymbol(":/ManaSymbols/RGMana.png")
{
	MainLayout=new QHBoxLayout(this);
	MainLayout->setMargin(0);
	MainLayout->setSpacing(0);
	SetCostString(Cost);
}
ManaCostLabel::ManaCostLabel(QWidget* parent/* =0 */)
	:QWidget(parent)
	,NumberOfSymbols(0)
	,ConvertedManaCost(0)
	,WSymbol(":/ManaSymbols/WMana.png")
	,USymbol(":/ManaSymbols/UMana.png")
	,BSymbol(":/ManaSymbols/BMana.png")
	,RSymbol(":/ManaSymbols/RMana.png")
	,GSymbol(":/ManaSymbols/GMana.png")
	,CSymbol(":/ManaSymbols/CMana.png")
	,CWSymbol(":/ManaSymbols/CWMana.png")
	,CUSymbol(":/ManaSymbols/CUMana.png")
	,CBSymbol(":/ManaSymbols/CBMana.png")
	,CRSymbol(":/ManaSymbols/CRMana.png")
	,CGSymbol(":/ManaSymbols/CGMana.png")
	,WUSymbol(":/ManaSymbols/WUMana.png")
	,WBSymbol(":/ManaSymbols/WBMana.png")
	,WRSymbol(":/ManaSymbols/WRMana.png")
	,WGSymbol(":/ManaSymbols/WGMana.png")
	,UBSymbol(":/ManaSymbols/UBMana.png")
	,URSymbol(":/ManaSymbols/URMana.png")
	,UGSymbol(":/ManaSymbols/UGMana.png")
	,BRSymbol(":/ManaSymbols/BRMana.png")
	,BGSymbol(":/ManaSymbols/BGMana.png")
	,RGSymbol(":/ManaSymbols/RGMana.png")
{
	MainLayout=new QHBoxLayout(this);
	MainLayout->setMargin(0);
	MainLayout->setSpacing(0);
}
void ManaCostLabel::SetCostString(const QString& Cost){
	NumberOfSymbols=0;
	ConvertedManaCost=0;
	if(!SymbolsLabels.isEmpty()){
		for(QList<QLabel*>::iterator i=SymbolsLabels.begin();i!=SymbolsLabels.end();i++)
			(*i)->deleteLater();
		SymbolsLabels.clear();
	}
	if(Cost.isEmpty()) return;
	CostString=Cost.toUpper();
	QStringList Components=CostString.split(',',QString::SkipEmptyParts);
	Components=SortedManaList(Components);
	for (QStringList::const_iterator i=Components.begin();i!=Components.end();i++){
		if (QRegExp("\\d+[\\+-]*").exactMatch(*i)){
			ConvertedManaCost+=i->count('-');
			ConvertedManaCost-=i->count('+');
			QPixmap TextPixmap(CSymbol);
			QPainter TextPainter(&TextPixmap);
			SymbolsLabels.append(new QLabel(this));
			TextPainter.setFont(QFont ("Arial",ManaFontSize,QFont::Bold));
			if (i->count('-')>i->count('+'))
				TextPainter.setPen(QPen(QColor(Qt::darkGreen)));
			else if (i->count('-')<i->count('+'))
				TextPainter.setPen(QPen(QColor(Qt::darkRed)));
			else TextPainter.setPen(QPen(QColor(Qt::black)));
			QString TextToPrint(*i);
			TextToPrint.replace('-',"");
			TextToPrint.replace('+',"");
			TextPainter.drawText(TextPixmap.rect(),Qt::AlignCenter,TextToPrint);
			SymbolsLabels.last()->setPixmap(TextPixmap);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost+=TextToPrint.toInt();
		}
		else if (QRegExp("[XYZ]").exactMatch(*i)){
			SymbolsLabels.append(new QLabel(this));
			QPixmap TextPixmap(CSymbol);
			QPainter TextPainter(&TextPixmap);
			TextPainter.setFont(QFont ("Arial",ManaFontSize,QFont::Bold));
			TextPainter.drawText(TextPixmap.rect(),Qt::AlignCenter,*i);
			SymbolsLabels.last()->setPixmap(TextPixmap);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
		}
		else if(QRegExp("\\d+W").exactMatch(*i)){
			QString TextToPrint(*i);
			TextToPrint.replace('W',"");
			SymbolsLabels.append(new QLabel(this));
			QPixmap TextPixmap(CWSymbol);
			QPainter TextPainter(&TextPixmap);
			TextPainter.setFont(QFont ("Arial",ManaFontSize/2,QFont::Bold));
			TextPainter.translate(TranslateX,TranslateY);
			TextPainter.drawText(TextPixmap.rect(),Qt::AlignCenter,TextToPrint);
			SymbolsLabels.last()->setPixmap(TextPixmap);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost+=TextToPrint.toInt();
		}
		else if(QRegExp("\\d+U").exactMatch(*i)){
			QString TextToPrint(*i);
			TextToPrint.replace('U',"");
			SymbolsLabels.append(new QLabel(this));
			QPixmap TextPixmap(CUSymbol);
			QPainter TextPainter(&TextPixmap);
			TextPainter.setFont(QFont ("Arial",ManaFontSize/2,QFont::Bold));
			TextPainter.translate(TranslateX,TranslateY);
			TextPainter.drawText(TextPixmap.rect(),Qt::AlignCenter,TextToPrint);
			SymbolsLabels.last()->setPixmap(TextPixmap);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost+=TextToPrint.toInt();
		}
		else if(QRegExp("\\d+B").exactMatch(*i)){
			QString TextToPrint(*i);
			TextToPrint.replace('B',"");
			SymbolsLabels.append(new QLabel(this));
			QPixmap TextPixmap(CBSymbol);
			QPainter TextPainter(&TextPixmap);
			TextPainter.setFont(QFont ("Arial",ManaFontSize/2,QFont::Bold));
			TextPainter.translate(TranslateX,TranslateY);
			TextPainter.drawText(TextPixmap.rect(),Qt::AlignCenter,TextToPrint);
			SymbolsLabels.last()->setPixmap(TextPixmap);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost+=TextToPrint.toInt();
		}
		else if(QRegExp("\\d+R").exactMatch(*i)){
			QString TextToPrint(*i);
			TextToPrint.replace('R',"");
			SymbolsLabels.append(new QLabel(this));
			QPixmap TextPixmap(CRSymbol);
			QPainter TextPainter(&TextPixmap);
			TextPainter.setFont(QFont ("Arial",ManaFontSize/2,QFont::Bold));
			TextPainter.translate(TranslateX,TranslateY);
			TextPainter.drawText(TextPixmap.rect(),Qt::AlignCenter,TextToPrint);
			SymbolsLabels.last()->setPixmap(TextPixmap);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost+=TextToPrint.toInt();
		}
		else if(QRegExp("\\d+G").exactMatch(*i)){
			QString TextToPrint(*i);
			TextToPrint.replace('G',"");
			SymbolsLabels.append(new QLabel(this));
			QPixmap TextPixmap(CGSymbol);
			QPainter TextPainter(&TextPixmap);
			TextPainter.setFont(QFont ("Arial",ManaFontSize/2,QFont::Bold));
			TextPainter.translate(TranslateX,TranslateY);
			TextPainter.drawText(TextPixmap.rect(),Qt::AlignCenter,TextToPrint);
			SymbolsLabels.last()->setPixmap(TextPixmap);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost+=TextToPrint.toInt();
		}
		else if(*i=="WU" || *i=="UW"){
			SymbolsLabels.append(new QLabel(this));
			SymbolsLabels.last()->setPixmap(WUSymbol);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost++;
		}
		else if(*i=="WB" || *i=="BW"){
			SymbolsLabels.append(new QLabel(this));
			SymbolsLabels.last()->setPixmap(WBSymbol);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost++;
		}
		else if(*i=="WR" || *i=="RW"){
			SymbolsLabels.append(new QLabel(this));
			SymbolsLabels.last()->setPixmap(WRSymbol);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost++;
		}
		else if(*i=="WG" || *i=="GW"){
			SymbolsLabels.append(new QLabel(this));
			SymbolsLabels.last()->setPixmap(WGSymbol);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost++;
		}
		else if(*i=="UB" || *i=="BU"){
			SymbolsLabels.append(new QLabel(this));
			SymbolsLabels.last()->setPixmap(UBSymbol);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost++;
		}
		else if(*i=="UR" || *i=="RU"){
			SymbolsLabels.append(new QLabel(this));
			SymbolsLabels.last()->setPixmap(URSymbol);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost++;
		}
		else if(*i=="UG" || *i=="GU"){
			SymbolsLabels.append(new QLabel(this));
			SymbolsLabels.last()->setPixmap(UGSymbol);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost++;
		}
		else if(*i=="BR" || *i=="RB"){
			SymbolsLabels.append(new QLabel(this));
			SymbolsLabels.last()->setPixmap(BRSymbol);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost++;
		}
		else if(*i=="BG" || *i=="GB"){
			SymbolsLabels.append(new QLabel(this));
			SymbolsLabels.last()->setPixmap(BGSymbol);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost++;
		}
		else if(*i=="RG" || *i=="GR"){
			SymbolsLabels.append(new QLabel(this));
			SymbolsLabels.last()->setPixmap(RGSymbol);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost++;
		}
		else if(*i=="W"){
			SymbolsLabels.append(new QLabel(this));
			SymbolsLabels.last()->setPixmap(WSymbol);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost++;
		}
		else if(*i=="U"){
			SymbolsLabels.append(new QLabel(this));
			SymbolsLabels.last()->setPixmap(USymbol);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost++;
		}
		else if(*i=="B"){
			SymbolsLabels.append(new QLabel(this));
			SymbolsLabels.last()->setPixmap(BSymbol);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost++;
		}
		else if(*i=="R"){
			SymbolsLabels.append(new QLabel(this));
			SymbolsLabels.last()->setPixmap(RSymbol);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost++;
		}
		else if(*i=="G"){
			SymbolsLabels.append(new QLabel(this));
			SymbolsLabels.last()->setPixmap(GSymbol);
			SymbolsLabels.last()->setScaledContents(true);
			MainLayout->addWidget(SymbolsLabels.last());
			NumberOfSymbols++;
			ConvertedManaCost++;
		}
		else{
			QMessageBox::critical(this,tr("Wrong Mana Code"),tr("Error: Unable to Interpret Card Mana Cost"));
			if(!SymbolsLabels.isEmpty()){
				for(QList<QLabel*>::iterator i=SymbolsLabels.begin();i!=SymbolsLabels.end();i++)
					(*i)->deleteLater();
			}
			SymbolsLabels.clear();
			ConvertedManaCost=0;
			NumberOfSymbols=0;
		}
	}
	//QMessageBox::information(this,"Converted Mana Cost",QString("%1").arg(ConvertedManaCost));
}
void ManaCostLabel::ClearCost(){
	if(!SymbolsLabels.isEmpty()){
		for(QList<QLabel*>::iterator i=SymbolsLabels.begin();i!=SymbolsLabels.end();i++)
			(*i)->deleteLater();
	}
	SymbolsLabels.clear();
	ConvertedManaCost=0;
	NumberOfSymbols=0;
}
QStringList ManaCostLabel::SortedManaList(const QStringList& Base){
	QStringList Result;
	for (QStringList::const_iterator i=Base.begin();i!=Base.end();i++){
		if(QRegExp("\\d+[\\+-]*").exactMatch(*i))
			Result.append(*i);
	}
	for (QStringList::const_iterator i=Base.begin();i!=Base.end();i++){
		if(QRegExp("[XYZ]").exactMatch(*i))
			Result.append(*i);
	}
	for (QStringList::const_iterator i=Base.begin();i!=Base.end();i++){
		if(QRegExp("\\d+[\\+-]*[WUBRG]+").exactMatch(*i))
			Result.append(*i);
	}
	for (QStringList::const_iterator i=Base.begin();i!=Base.end();i++){
		if((*i)=="WU" || (*i)=="UW")
			Result.append(*i);
	}
	for (QStringList::const_iterator i=Base.begin();i!=Base.end();i++){
		if((*i)=="WB"|| (*i)=="BW")
			Result.append(*i);
	}
	for (QStringList::const_iterator i=Base.begin();i!=Base.end();i++){
		if((*i)=="WR"|| (*i)=="RW")
			Result.append(*i);
	}
	for (QStringList::const_iterator i=Base.begin();i!=Base.end();i++){
		if((*i)=="WG"|| (*i)=="GW")
			Result.append(*i);
	}
	for (QStringList::const_iterator i=Base.begin();i!=Base.end();i++){
		if((*i)=="UB"|| (*i)=="BU")
			Result.append(*i);
	}
	for (QStringList::const_iterator i=Base.begin();i!=Base.end();i++){
		if((*i)=="UR"|| (*i)=="RU")
			Result.append(*i);
	}
	for (QStringList::const_iterator i=Base.begin();i!=Base.end();i++){
		if((*i)=="UG"|| (*i)=="GU")
			Result.append(*i);
	}
	for (QStringList::const_iterator i=Base.begin();i!=Base.end();i++){
		if((*i)=="BR"|| (*i)=="RB")
			Result.append(*i);
	}
	for (QStringList::const_iterator i=Base.begin();i!=Base.end();i++){
		if((*i)=="BG"|| (*i)=="GB")
			Result.append(*i);
	}
	for (QStringList::const_iterator i=Base.begin();i!=Base.end();i++){
		if((*i)=="RG"|| (*i)=="GR")
			Result.append(*i);
	}
	for (QStringList::const_iterator i=Base.begin();i!=Base.end();i++){
		if((*i)=="W")
			Result.append(*i);
	}
	for (QStringList::const_iterator i=Base.begin();i!=Base.end();i++){
		if((*i)=="U")
			Result.append(*i);
	}
	for (QStringList::const_iterator i=Base.begin();i!=Base.end();i++){
		if((*i)=="B")
			Result.append(*i);
	}
	for (QStringList::const_iterator i=Base.begin();i!=Base.end();i++){
		if((*i)=="R")
			Result.append(*i);
	}
	for (QStringList::const_iterator i=Base.begin();i!=Base.end();i++){
		if((*i)=="G")
			Result.append(*i);
	}
	return Result;
}