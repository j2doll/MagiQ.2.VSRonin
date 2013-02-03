#include "CardViewer.h"
#include "Card.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QTransform>
#include <QPainter>
#include "StyleSheets.h"
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QFrame>
#include <QBitmap>
CardViewer::CardViewer(QWidget* parent)
	:QWidget(parent)
	,CardToDisplay(NULL)
	,CardRotation(0)
	,RepresentNumber(1)
	,CanBeZoom(true)
	,CanBeClick(false)
{
	setMinimumSize(60,84);
	Displayer=new QLabel(this);
	Displayer->setObjectName("Displayer");
	Displayer->setScaledContents(true);
	QHBoxLayout* MainLay=new QHBoxLayout(this);
	MainLay->setSpacing(0);
	MainLay->setMargin(0);
	MainLay->addWidget(Displayer);
	Shader=new QFrame(this);
	Shader->setObjectName("Shader");
	Shader->raise();
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
		ImageToDispaly=QPixmap::grabWidget(CardToDisplay);
		setToolTip(CardToDisplay->GetCardName());
	}
	else{
		ImageToDispaly.load(":/CardImage/CBase.png");
		setToolTip("");
	}
	if (!ImageToDispaly.isNull()){
		QTransform Transformer;
		Transformer.rotate(CardRotation);
		ImageToDispaly=ImageToDispaly.transformed(Transformer);
		if (RepresentNumber>1){
			QPainter NumberPaint(&ImageToDispaly);
			NumberPaint.setFont(QFont("Arial",height()/10,QFont::Bold));
			NumberPaint.drawText(ImageToDispaly.rect(),Qt::AlignLeft,QString("%1").arg(RepresentNumber));
		}
		PrefSize=ImageToDispaly.size();
		Displayer->setPixmap(ImageToDispaly);
		Shader->setMask(ImageToDispaly.scaled(Displayer->size()).mask());
	}
	setStyleSheet(StyleSheets::CardViewerCSS);
}
void CardViewer::resizeEvent(QResizeEvent* event){
	Shader->setGeometry(0,0,width(),height());
}
void CardViewer::TapAnimationStart(){
	QPropertyAnimation* animTap= new QPropertyAnimation(this,"CardRotation",parent());
	animTap->setDuration(3000);
	animTap->setEasingCurve(QEasingCurve::Linear);
	animTap->setKeyValueAt(0.0,0);
	animTap->setKeyValueAt(1.0,90);
	animTap->start(QAbstractAnimation::DeleteWhenStopped);
}
void CardViewer::mousePressEvent(QMouseEvent* event){
	if(!CanBeZoom){
		emit RequireZoom(NULL);
		return;
	}
	if (event->button() == Qt::RightButton){
		emit RequireZoom(CardToDisplay);
	}
}
void CardViewer::SetShadable(bool a){
	if (a) Shader->show();
	else Shader->hide();
}
void CardViewer::mouseDoubleClickEvent(QMouseEvent* e){
	if (e->button() == Qt::LeftButton){
		emit DoubleClicked();
	}
}