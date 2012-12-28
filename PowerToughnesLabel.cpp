#include "PowerToughnesLabel.h"
#include <QLabel>
#include <QPainter>
#include <QFont>
#include <QHBoxLayout>
#include <QStringList>
#include <QRegExp>
#include <QMessageBox>
#include <QStyle>
#include "CostantsDefinition.h"
PowerToughnesLabel::PowerToughnesLabel(QWidget* parent/* =0 */)
	:QWidget(parent)
{
	MainLabel=new QLabel(this);
	MainLabel->setObjectName("MainLabel");
	MainLabel->setScaledContents(true);
	MainLabel->setAlignment(Qt::AlignCenter);
	QHBoxLayout* MainLayout=new QHBoxLayout(this);
	MainLayout->addWidget(MainLabel);
}
void PowerToughnesLabel::SetSelectedBase(int a){
	if(a>=Constants::PTBoxTypes::CBox && a<=Constants::PTBoxTypes::GoldBox) SelectedBase=a;
	switch(SelectedBase){
	case Constants::PTBoxTypes::WBox:
		LabelBase=QPixmap(":/PTLabelBases/WBox.png").mask(); break;
	case Constants::PTBoxTypes::UBox:
		LabelBase=QPixmap(":/PTLabelBases/UBox.png").mask(); break;
	case Constants::PTBoxTypes::BBox:
		LabelBase=QPixmap(":/PTLabelBases/BBox.png").mask(); break;
	case Constants::PTBoxTypes::RBox:
		LabelBase=QPixmap(":/PTLabelBases/RBox.png").mask(); break;
	case Constants::PTBoxTypes::GBox:
		LabelBase=QPixmap(":/PTLabelBases/GBox.png").mask(); break;
	case Constants::PTBoxTypes::GoldBox:
		LabelBase=QPixmap(":/PTLabelBases/GoldBox.png").mask(); break;
	default:
		LabelBase=QPixmap(":/PTLabelBases/CBox.png").mask();
	}
	UpadateAspect();
}
void PowerToughnesLabel::UpadateAspect(){
	MainLabel->setMask(LabelBase.scaled(width(),height()).mask());
	if (PTString=="") {MainLabel->setText(""); return;}
	QStringList PandT=PTString.split('/');
	QString Temp(PandT.at(0));
	QString LabelText("");
	int ContPlus=0;
	int ContMinus=0;
	if (QRegExp("[\\d\\*]+[\\+-]*").exactMatch(Temp) && PandT.size()==2){
		ContPlus=Temp.count('+');			
		ContMinus=Temp.count('-');
		Temp.replace('+',"");
		Temp.replace('-',"");
		if (ContPlus>ContMinus)
			LabelText.append(
				"<FONT COLOR='#00ff00'><b>"+Temp+"</b></FONT><b>/</b>"
			);
		else if (ContPlus<ContMinus)
			LabelText.append(
				"<FONT COLOR='#ff0000'><b>"+Temp+"</b></FONT><b>/</b>"
			);
		else LabelText.append("<b>"+Temp+"/</b>");
	}
	else {
		MainLabel->setText("");
		QMessageBox::critical(this,tr("Wrong P/T Code"),tr("Error: Unable to Interpret Card Power and Toughness"));
	}
	Temp=PandT.at(1);
	if (QRegExp("[\\d\\*]+[\\+-]*").exactMatch(Temp)){
		ContPlus=Temp.count('+');			
		ContMinus=Temp.count('-');
		Temp.replace('+',"");
		Temp.replace('-',"");
		if (ContPlus>ContMinus)
			LabelText.append(
			"<FONT COLOR='#00ff00'><b>"+Temp+"</b></FONT>"
			);
		else if (ContPlus<ContMinus)
			LabelText.append(
			"<FONT COLOR='#ff0000'><b>"+Temp+"</b></FONT>"
			);
		else LabelText.append("<b>"+Temp+"</b>");
	}
	else {
		MainLabel->setText("");
		QMessageBox::critical(this,tr("Wrong P/T Code"),tr("Error: Unable to Interpret Card Power and Toughness"));
	}
	MainLabel->setText(LabelText);
	style()->unpolish(this);
	style()->polish(this);
}
void PowerToughnesLabel::resizeEvent(QResizeEvent* event){
	MainLabel->setMask(LabelBase.scaled(width(),height()).mask());
}