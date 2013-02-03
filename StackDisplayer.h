#ifndef STACKDISPLAYER_H
#define STACKDISPLAYER_H
#include <QWidget>
#include <QStack>
#include "EffectData.h"
class QTableWidget;
class StackDispalyer : public QWidget{
	Q_OBJECT
private:
	QTableWidget* StackList;
	QStack<EffectData> EffStack;
public:
	StackDispalyer(QWidget* parent=0);
public slots:
	void AddEffect(const EffectData& a);
	void Resolve();
	void EmptyStack();
};
#endif