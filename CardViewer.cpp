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
#include <QPainter>
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
	QSize OriginalSize(ImageToDispaly.size());
	if (!ImageToDispaly.isNull()){
		QTransform Transformer;
		Transformer.rotate(CardRotation);
		ImageToDispaly=ImageToDispaly.transformed(Transformer);
		if (RepresentNumber>1){
			QPainter NumberPaint(&ImageToDispaly);
			NumberPaint.setFont(QFont("Arial",height()/10,QFont::Bold));
			NumberPaint.drawText(ImageToDispaly.rect(),Qt::AlignLeft,QString("%1").arg(RepresentNumber));
		}
		QPixmap EnlagedImageToDispaly(
			qMax(ImageToDispaly.width(),OriginalSize.width()),
			qMax(ImageToDispaly.height(),OriginalSize.height())
			);
		EnlagedImageToDispaly.fill(QColor(0,0,0,0));
		PrefSize=EnlagedImageToDispaly.size();
		QPainter EnlagedPainter(&EnlagedImageToDispaly);
		EnlagedPainter.drawPixmap(
			QPoint(
			(EnlagedImageToDispaly.width()-ImageToDispaly.width())/2,
			(EnlagedImageToDispaly.height()-ImageToDispaly.height())/2
			)
			,ImageToDispaly,ImageToDispaly.rect());
		Displayer->setPixmap(EnlagedImageToDispaly);
		Shader->setMask(EnlagedImageToDispaly.scaled(Displayer->size()).mask());
	}
	setStyleSheet(StyleSheets::CardViewerCSS);
}
void CardViewer::resizeEvent(QResizeEvent* event){
	Shader->setGeometry(0,0,width(),height());
}
void CardViewer::TapAnimationStart(){
	QPropertyAnimation* animTap= new QPropertyAnimation(this,"CardRotation",parent());
	animTap->setDuration(TapAnimationDuration);
	animTap->setEasingCurve(QEasingCurve::Linear);
	animTap->setKeyValueAt(0.0,0);
	animTap->setKeyValueAt(1.0,90);
	animTap->start(QAbstractAnimation::DeleteWhenStopped);
}
void CardViewer::UnTapAnimationStart(){
	QPropertyAnimation* animTap= new QPropertyAnimation(this,"CardRotation",parent());
	animTap->setDuration(TapAnimationDuration);
	animTap->setEasingCurve(QEasingCurve::Linear);
	animTap->setKeyValueAt(1.0,0);
	animTap->setKeyValueAt(0.0,90);
	animTap->start(QAbstractAnimation::DeleteWhenStopped);
}
void CardViewer::mousePressEvent(QMouseEvent* event){
	if(!CanBeZoom && event->button() == Qt::RightButton){
		emit RequireZoom(NULL);
		return;
	}
	if (event->button() == Qt::RightButton){
		emit RequireZoom(CardToDisplay);
	}
	else if (event->button() == Qt::LeftButton && CanBeClick){
		emit clicked(CardToDisplay->GetCardID());
	}
}
void CardViewer::SetShadable(bool a){
	if (a) Shader->show();
	else Shader->hide();
}
void CardViewer::mouseDoubleClickEvent(QMouseEvent* e){
	if (!CardToDisplay || !CanBeClick) return;
	if (e->button() == Qt::LeftButton){
		emit DoubleClicked(CardToDisplay->GetCardID());
	}
}