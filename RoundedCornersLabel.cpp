#include "RoundedCornersLabel.h"
#include <QPainter>
//#include <QPixmap>
RoundedCornersLabel::RoundedCornersLabel(QWidget* parent)
	:QLabel(parent)
	,Radious(0)
	,ImageToShow(90,90)
{ImageToShow.fill(Qt::gray);}
void RoundedCornersLabel::paintEvent(QPaintEvent *e){
	Q_UNUSED(e)
	QBrush brush(ImageToShow.scaled(width(),height()));
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(brush);
	painter.drawRoundedRect(0, 0, width(), height(), Radious, Radious);
}