#include "SignalerArrow.h"
#include <QPainter>
#include <QPixmap>
#include <qmath.h>
//#define SINGLE_START_ARROW
SignalerArrow::SignalerArrow(QWidget* parent)
	:QWidget(parent)
	,ArrowColor(237,28,36,ArrowAlphaComponent)
	,From(0,0)
	,To(0,0)
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
	move(TopLeftWid);
	resize(qAbs(From.x()-To.x()),qAbs(From.y()-To.y()));
}
void SignalerArrow::paintEvent(QPaintEvent *event){
	Q_UNUSED(event)
	QPixmap ArrowImage(120,40);
	ArrowImage.fill(QColor(0,0,0,0));
	QPainter ArrowDrawer(&ArrowImage);
	ArrowDrawer.setPen(Qt::NoPen);
	ArrowDrawer.setBrush(ArrowColor);
	ArrowDrawer.drawPolygon(ArrowPoligon);
	double Wid=static_cast<double>(From.x()-To.x());
	double Hei=static_cast<double>(From.y()-To.y());
	QTransform Transformer;
	Transformer.rotateRadians(qAtan(Hei/Wid));
	if(To.x()<From.x()) Transformer.rotate(180);
	QPixmap ImageToDraw=ArrowImage
		.scaled(qSqrt((Wid*Wid)+(Hei*Hei)),InitialHei)
		.transformed(Transformer)
	;
	QPainter WidPainter(this);
	WidPainter.drawPixmap(rect(),ImageToDraw);
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