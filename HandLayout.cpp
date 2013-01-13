#include "HandLayout.h"
#include <QLayoutItem>
#include <QWidget>
HandLayout::~HandLayout(){
	QLayoutItem *item;
	while ((item = takeAt(0)))
		delete item;
}
void HandLayout::setGeometry(const QRect &rect){
	if (items.isEmpty()) return;
	int left, top, right, bottom;
	getContentsMargins(&left, &top, &right, &bottom);
	QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
	int TotalWid=0;
	foreach(QLayoutItem *item,items)
		TotalWid+=item->minimumSize().width();
	int AvailableWid=effectiveRect.width()-spacing()*(count()-1);
	int CurrentX=effectiveRect.x();
	if (TotalWid>AvailableWid){
		//Lay Items One over the other
		foreach(QLayoutItem *item,items){
			item->setGeometry(QRect(CurrentX,effectiveRect.y(),item->minimumSize().width(),effectiveRect.height()));
			item->widget()->raise();
			CurrentX+=(effectiveRect.width()-items.last()->minimumSize().width())/(count()-1);
		}
	}
	else{
		//Lay Items one next to the other
		foreach(QLayoutItem *item,items){
			item->setGeometry(QRect(CurrentX,effectiveRect.y(),AvailableWid/count(),effectiveRect.height()));
			CurrentX+=(AvailableWid/count())+spacing();
		}
	}
}
QSize HandLayout::minimumSize() const{
	if (items.isEmpty()) return QSize(0,0);
	int MaxMinWid=items.at(0)->minimumSize().width();
	int MaxMinHei=items.at(0)->minimumSize().height();
	for (QList<QLayoutItem*>::const_iterator i=items.begin()+1;i!=items.end();i++){
		if ((*i)->minimumSize().width()>MaxMinWid) MaxMinWid=(*i)->minimumSize().width();
		if ((*i)->minimumSize().height()>MaxMinHei) MaxMinHei=(*i)->minimumSize().height();
	}
	return QSize(
		qMax(MaxMinWid,items.last()->minimumSize().width()+(spacing()*(count()-1)))
		,MaxMinHei);
}
QSize HandLayout::sizeHint() const{
	if (items.isEmpty()) return QSize(0,0);
	int MaxSizeHintHei=items.at(0)->sizeHint().height();
	int SumSizeWid=items.at(0)->sizeHint().width();
	for (QList<QLayoutItem*>::const_iterator i=items.begin()+1;i!=items.end();i++){
		if ((*i)->sizeHint().height()>MaxSizeHintHei) MaxSizeHintHei=(*i)->sizeHint().height();
		SumSizeWid+=(*i)->sizeHint().width();
	}
	return QSize(SumSizeWid+(spacing()*(count()-1)),MaxSizeHintHei);
}