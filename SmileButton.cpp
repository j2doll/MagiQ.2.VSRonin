#include "SmileButton.h"
#include <QtGui>
SmileButton::SmileButton(QWidget* parent, int id)
:QPushButton(parent), Identit(id)
{

}
void SmileButton::mousePressEvent(QMouseEvent *event){
	if (this->isEnabled() && event->button()==Qt::LeftButton){
		emit clicked(Identit);
	}
}