#ifndef HANDLAYOUT_H
#define HANDLAYOUT_H
#include <QLayout>
#include <QList>
class QLayoutItem;
class QWidget;
class HandLayout : public QLayout{
public:
	HandLayout(QWidget *parent): QLayout(parent) {}
	//HandLayout(QLayout *parent): QLayout(parent) {}
	HandLayout() : QLayout(0) {}
	~HandLayout();
	void addItem(QLayoutItem *item){items.append(item);}
	QSize sizeHint() const;
	QSize minimumSize() const;
	int count() const{return items.size();};
	QLayoutItem *itemAt(int index) const{return items.value(index);}
	QLayoutItem *takeAt(int index){return (index>=0 && index<items.size()) ? items.takeAt(index) : 0;}
	void setGeometry(const QRect &rect);
private:
	QList<QLayoutItem*> items;
};
#endif