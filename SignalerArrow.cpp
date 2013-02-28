#include "SignalerArrow.h"
#include <QPainter>
#include <QPixmap>
#include <qmath.h>
#include <QImage>
#define SINGLE_START_ARROW
SignalerArrow::SignalerArrow(QWidget* parent)
	:QWidget(parent)
	,ArrowColor(237,28,36,ArrowAlphaComponent)
	,ArrowImage(120,40)
	,From(0,0)
	,To(0,0)
	,Modulus(0)
	,Phase(0)
	,ArrowAlphaComponent(70)
{
	setAttribute(Qt::WA_TransparentForMouseEvents);
	setFocusPolicy(Qt::NoFocus);
	ArrowPoligon
#ifdef SINGLE_START_ARROW
		<< QPoint(0,InitialHei/2)
#endif
#ifndef SINGLE_START_ARROW
		<< QPoint(0,InitialHei/4)
#endif
		<< QPoint(InitialWid*2/3,InitialHei/4)
		<< QPoint(InitialWid*2/3,0)
		<< QPoint(InitialWid,InitialHei/2)
		<< QPoint(InitialWid*2/3,InitialHei)
		<< QPoint(InitialWid*2/3,InitialHei*3/4)
#ifndef SINGLE_START_ARROW
		<< QPoint(0,InitialHei*3/4)
#endif
		;
}
void SignalerArrow::UpdateGeometry(){
	QPoint TopLeftWid=From;
	if(To.x()<From.x())
		TopLeftWid.rx()-=From.x()-To.x();
	if(To.y()<From.y())
		TopLeftWid.ry()-=From.y()-To.y();
	qreal Wid=static_cast<qreal>(From.x()-To.x());
	qreal Hei=static_cast<qreal>(From.y()-To.y());
	Modulus=qSqrt((Wid*Wid)+(Hei*Hei));
	Phase=qAtan(Hei/Wid);

	/*QImage StartingPoint(ArrowImage.size(),QImage::Format_Mono);
	StartingPoint.setColor(0,0);
	StartingPoint.setColor(1,1);
	StartingPoint.fill(0);
	StartingPoint.setPixel(0,InitialHei/2,1);
	QTransform Transformer;
	Transformer.rotateRadians(Phase);
	if(To.x()<From.x()) Transformer.rotate(180);
	StartingPoint=StartingPoint.scaled(Modulus,InitialHei).transformed(Transformer);
	bool NotFoundYet=true;
	for(int XOffset=0;XOffset<ArrowImage.width() && NotFoundYet;XOffset++){
		for(int YOffset=0;YOffset<ArrowImage.height() && NotFoundYet;YOffset++){
			if(StartingPoint.pixel(XOffset,YOffset)==1){
				TopLeftWid.rx()-=XOffset;
				TopLeftWid.ry()-=YOffset;
				NotFoundYet=false;
			}
		}
	}*/

	move(TopLeftWid);
	resize(qAbs(From.x()-To.x()),qAbs(From.y()-To.y()));
}
void SignalerArrow::paintEvent(QPaintEvent *event){
	Q_UNUSED(event)
	QPainter WidPainter(this);
	ArrowImage.fill(QColor(0,0,0,0));
	if(Modulus>0.0){
		QPainter ArrowDrawer(&ArrowImage);
		ArrowDrawer.setPen(Qt::NoPen);
		ArrowDrawer.setBrush(ArrowColor);
		ArrowDrawer.drawPolygon(ArrowPoligon);
		QTransform Transformer;
		Transformer.rotateRadians(Phase);
		if(To.x()<From.x()) Transformer.rotate(180);
		WidPainter.drawPixmap(rect(),
			ArrowImage.scaled(Modulus,InitialHei).transformed(Transformer)
			);
	}
	else {WidPainter.drawPixmap(rect(),ArrowImage);}
}
void SignalerArrow::SetArrowColor(const int& a){
	switch (a){
	case ArrowRed: ArrowColor.setRgb(237,28,36,ArrowAlphaComponent); break;
	case ArrowYellow: ArrowColor.setRgb(255,242,0,ArrowAlphaComponent); break;
	default: return;
	}
}
void SignalerArrow::SetArrowColor(const QString& a){
	ArrowColor.setNamedColor(a);
	if(!ArrowColor.isValid()) return SetArrowColor(ArrowRed);
	ArrowColor.setAlpha(ArrowAlphaComponent);
}
void SignalerArrow::SetArrowAlphaComponent(unsigned short a){
	if (a>255) a=255;
	ArrowAlphaComponent=a;
}