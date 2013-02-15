#ifndef CONTROLLEDLAYOUT_H
#define CONTROLLEDLAYOUT_H
#include <QLayout>
#include <QList>
class QLayoutItem;
class QWidget;
class ControlledLayout : public QLayout{
public:
	ControlledLayout(QWidget *parent=0): QLayout(parent){}
	~ControlledLayout();
	void addItem(QLayoutItem *item){items.append(item);}
	QSize sizeHint() const;
	QSize minimumSize() const;
	int count() const{return items.size();};
	QLayoutItem *itemAt(int index) const{return items.value(index);}
	QLayoutItem *takeAt(int index){return (index>=0 && index<items.size()) ? items.takeAt(index) : NULL;}
	void setGeometry(const QRect &rect);
private:
	QList<QLayoutItem*> items;
};
#endif