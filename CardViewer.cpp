#include "CardViewer.h"
#include "Card.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QTransform>
#include <QPainter>
#include "StyleSheets.h"
#include <QPropertyAnimation>
#include <QMouseEvent>
CardViewer::CardViewer(QWidget* parent)
	:QWidget(parent)
	,CardToDisplay(NULL)
	,CardRotation(0)
	,RepresentNumber(1)
{
	setMinimumSize(60,84);
	Displayer=new QLabel(this);
	Displayer->setObjectName("Displayer");
	Displayer->setScaledContents(true);
	QHBoxLayout* MainLay=new QHBoxLayout(this);
	MainLay->setSpacing(0);
	MainLay->setMargin(0);
	MainLay->addWidget(Displayer);
}
void CardViewer::SetCardRotation(int a){
	while (a<0) a+=360;
	while (a>360) a-=360;
	CardRotation=a;
	UpdateAspect();
}
void CardViewer::UpdateAspect(){
	QPixmap ImageToDispaly;
	if (CardToDisplay){
		bool rehide=CardToDisplay->isHidden();
		CardToDisplay->show();
		ImageToDispaly=QPixmap::grabWidget(CardToDisplay);
		if (rehide) CardToDisplay->hide();
	}
	else ImageToDispaly.load(":/CardImage/CBase.png");
	QTransform Transformer;
	//Transformer.translate(ImageToDispaly.rect().center().x(),ImageToDispaly.rect().center().y());
	Transformer.rotate(CardRotation);
	ImageToDispaly=ImageToDispaly.transformed(Transformer);
	if (RepresentNumber>1){
		QPainter NumberPaint(&ImageToDispaly);
		NumberPaint.setFont(QFont("Arial",height()/10,QFont::Bold));
		NumberPaint.drawText(ImageToDispaly.rect(),Qt::AlignLeft,QString("%1").arg(RepresentNumber));
	}
	PrefSize=ImageToDispaly.size();
	Displayer->setPixmap(ImageToDispaly);
}
void CardViewer::TapAnimationStart(){
	QPropertyAnimation* animTap= new QPropertyAnimation(this,"CardRotation",parent());
	animTap->setDuration(3000);
	animTap->setEasingCurve(QEasingCurve::Linear);
	animTap->setKeyValueAt(0.0,0);
	animTap->setKeyValueAt(1.0,90);
	animTap->start(QAbstractAnimation::DeleteWhenStopped);
}