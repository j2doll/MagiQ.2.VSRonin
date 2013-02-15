#include <QLayoutItem>
#include <QWidget>
#include "ControlledLayout.h"
#include "CardViewer.h"
#include "Card.h"
ControlledLayout::~ControlledLayout(){
	QLayoutItem *item;
	while ((item = takeAt(0)))
		delete item;
}
void ControlledLayout::setGeometry(const QRect &rect){
	if (items.isEmpty()) return;
	int left, top, right, bottom;
	getContentsMargins(&left, &top, &right, &bottom);
	QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
	int TotalWid=0;
	int CountTapped=0;
	foreach(QLayoutItem *item,items){
		if(static_cast<CardViewer*>(item->widget())->GetCardToDisplay()->IsTapped()){
			TotalWid+=item->minimumSize().height();
			++CountTapped;
		}
		else
			TotalWid+=item->minimumSize().width();
	}
	int AvailableWid=effectiveRect.width()-spacing()*(count()-1);
	int CurrentX=effectiveRect.x();
	if (TotalWid>AvailableWid){
		//Lay Items One over the other
		int IncrementOnX=0;
		if(count()>1){
			if(static_cast<CardViewer*>(items.last()->widget())->GetCardToDisplay()->IsTapped())
				IncrementOnX=(effectiveRect.width()-items.last()->minimumSize().height())/(count()-1);
			else
				IncrementOnX=(effectiveRect.width()-items.last()->minimumSize().width())/(count()-1);
		}
		foreach(QLayoutItem *item,items){
			if(static_cast<CardViewer*>(item->widget())->GetCardToDisplay()->IsTapped())
				item->setGeometry(QRect(CurrentX,effectiveRect.y(),item->minimumSize().height(),effectiveRect.height()));
			else
				item->setGeometry(QRect(CurrentX,effectiveRect.y(),item->minimumSize().width(),effectiveRect.height()));
			item->widget()->raise();
			CurrentX+=IncrementOnX;
		}
	}
	else{
		//Lay Items one next to the other
		double WidForUntapped=static_cast<double>(AvailableWid)/(1.0+(0.4*static_cast<double>(CountTapped)/static_cast<double>(count())));
		foreach(QLayoutItem *item,items){
			if(static_cast<CardViewer*>(item->widget())->GetCardToDisplay()->IsTapped()){
				item->setGeometry(QRect(CurrentX,effectiveRect.y(),static_cast<int>(1.4*WidForUntapped),effectiveRect.height()));
				CurrentX+=static_cast<int>(1.4*WidForUntapped)+spacing();
			}
			else{
				item->setGeometry(QRect(CurrentX,effectiveRect.y(),static_cast<int>(WidForUntapped),effectiveRect.height()));
				CurrentX+=static_cast<int>(WidForUntapped)+spacing();
			}
		}
	}
}
QSize ControlledLayout::minimumSize() const{
	if (items.isEmpty()) return QSize(0,0);
	int left, top, right, bottom;
	getContentsMargins(&left, &top, &right, &bottom);
	
	for (QList<QLayoutItem*>::const_iterator i=items.begin();i!=items.end();i++){
		if(static_cast<CardViewer*>((*i)->widget())->GetCardToDisplay()->IsTapped()){
			if(static_cast<CardViewer*>(items.last()->widget())->GetCardToDisplay()->IsTapped()){
				return QSize(
					items.last()->minimumSize().height()+(spacing()*(count()-1))+left+right
					,items.last()->minimumSize().height()+top+bottom);
			}
			else{
				return QSize(
					qMax((*i)->minimumSize().height(),items.last()->minimumSize().width()+(spacing()*(count()-1)))+left+right
					,items.last()->minimumSize().height()+top+bottom);
			}
		}
		
	}
	return QSize(
		items.last()->minimumSize().width()+(spacing()*(count()-1))+left+right
		,items.last()->minimumSize().height()+top+bottom);
}
QSize ControlledLayout::sizeHint() const{
	if (items.isEmpty()) return QSize(0,0);
	int left, top, right, bottom;
	getContentsMargins(&left, &top, &right, &bottom);
	int TotalWid=0;
	foreach(QLayoutItem *item,items){
		if(static_cast<CardViewer*>(item->widget())->GetCardToDisplay()->IsTapped()){
			TotalWid+=item->minimumSize().height();
		}
		else
			TotalWid+=item->minimumSize().width();
	}
	return QSize(TotalWid+(spacing()*(count()-1))+left+right,items.last()->minimumSize().height()+top+bottom);
}