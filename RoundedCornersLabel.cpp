#include "RoundedCornersLabel.h"
#include <QPainter>
RoundedCornersLabel::RoundedCornersLabel(QWidget* parent)
	:QWidget(parent)
	,Radious(0)
	,ImageToShow(10,10)
{ImageToShow.fill(Qt::gray);}
void RoundedCornersLabel::paintEvent(QPaintEvent *e){
	Q_UNUSED(e)
	QBrush brush(ImageToShow.scaled(width(),height()));
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(brush);
	painter.drawRoundedRect(0, 0, width(), height(), Radious, Radious);
}
void RoundedCornersLabel::SetImageToShow(const QPixmap& a){
	if (a.isNull()){
		ImageToShow=QPixmap(10,10);
		ImageToShow.fill(Qt::gray);
	}
	else ImageToShow=a;
}